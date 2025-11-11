import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import "./components"

/*
  main.qml - Based on main_1029 (fully working version)
  Key principle: Sub-components directly access global C++ context properties
  (cameraController, videoPlayer, videoComposer, recorder, appController)
  WITHOUT property passing - this avoids null binding issues.
  
  Each sub-component directly uses: target: cameraController, etc.
*/

ApplicationWindow {
    id: appWindow
    visible: true
    width: 1280
    height: 800
    title: "CoachUI - 攝影機預覽與錄製"
    
    // AppState - fully QML-based
    AppState { id: appState }

    // 狀態處理器
    StatusHandler { id: statusHandler }

    // No header
    header: null

    // ===== MAIN LAYOUT =====
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12

        CameraControlPanel {
            id: cameraControlPanel
            Layout.fillWidth: true
            Layout.preferredHeight: 170
            Layout.minimumHeight: 150
            statusHandler: statusHandler

            onRecordingStarted: {
                statusHandler.showInfo("開始錄製…")
            }

            onRecordingStopped: {
                statusHandler.showInfo("錄製已停止")
            }

            onTestVideoRequested: {
                if (videoPlayer && typeof videoPlayer.loadVideo === "function") {
                    statusHandler.showInfo("載入測試影片…")
                    videoPlayer.loadVideo("/home/mxpt2/coachui/test_video.mp4")
                }
            }

            onVideoListRequested: {
                videoListPopup.open()
            }
        }

        // 主要內容區：影片播放 + 關鍵幀面板
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 12

            // 左側：影片區域
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 640
                spacing: 12

                VideoDisplayArea {
                    id: videoDisplayArea
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 320
                }

                VideoPlayerControl {
                    id: videoPlayerControl
                    Layout.fillWidth: true
                    Layout.preferredHeight: 110
                    videoDisplayArea: videoDisplayArea
                }
            }

            // 右側：關鍵幀面板
            KeyframePanel {
                id: keyframePanel
                Layout.preferredWidth: 120
                Layout.fillHeight: true
                Layout.minimumWidth: 100
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            radius: 4
            color: "#f5f5f5"
            border.color: "#d0d0d0"
            border.width: 1

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 12
                text: statusHandler.currentStatus
                color: statusHandler.currentColor
            }
        }
    }

    // ===== VIDEO LIST POPUP =====
    VideoListPopup {
        id: videoListPopup

        Component.onCompleted: {
            console.log("✅ VideoListPopup loaded")
            console.log("   Global appController:", typeof appController)
        }
    }

    // ===== LISTEN TO VIDEO LIST POPUP SIGNALS =====
    Connections {
        target: videoListPopup
        
        // 單選模式
        function onVideoSelected(filePath) {
            console.log("📼 Video selected:", filePath)
            appState.selectVideo(filePath)
            statusHandler.showInfo("載入影片: " + filePath.split('/').pop())
        }
        
        // 多選模式 - 並排合成
        function onVideosSelected(filePaths) {
            console.log("🎬 多選影片並排合成:", filePaths)
            statusHandler.showInfo("正在合成 " + filePaths.length + " 個影片...")
            
            // 生成臨時輸出路徑
            var timestamp = Date.now()
            var outputPath = "/home/mxpt2/Videos/composed_sidebyside_" + timestamp + ".mp4"
            
            // 呼叫 VideoComposer 進行並排合成（使用 ffmpeg 版本）
            if (videoComposer && typeof videoComposer.composeSideBySideFFmpeg === 'function') {
                var success = videoComposer.composeSideBySideFFmpeg(filePaths, outputPath)
                
                if (success) {
                    console.log("✅ 並排合成成功:", outputPath)
                    statusHandler.showInfo("合成完成！正在載入...")
                    
                    // 載入合成後的影片
                    if (videoPlayer && typeof videoPlayer.loadVideo === 'function') {
                        videoPlayer.loadVideo(outputPath)
                    }
                } else {
                    console.log("❌ 並排合成失敗")
                    statusHandler.showError("合成失敗，請檢查影片格式")
                }
            } else {
                console.log("❌ videoComposer.composeSideBySide 不可用")
                statusHandler.showError("合成功能不可用")
            }
        }
        
        // 多選模式 - 對齊關鍵幀並排合成
        function onVideosSelectedAligned(filePaths, keyframeName) {
            console.log("🎯 對齊關鍵幀並排合成:", filePaths, "關鍵幀:", keyframeName)
            statusHandler.showInfo("正在對齊關鍵幀「" + keyframeName + "」並合成 " + filePaths.length + " 個影片...")
            
            // 生成臨時輸出路徑
            var timestamp = Date.now()
            var outputPath = "/home/mxpt2/Videos/composed_aligned_" + timestamp + ".mp4"
            
            // 呼叫 VideoComposer 進行對齊關鍵幀合成（使用 ffmpeg 版本）
            if (videoComposer && typeof videoComposer.composeSideBySideAlignedFFmpeg === 'function') {
                var success = videoComposer.composeSideBySideAlignedFFmpeg(filePaths, outputPath, keyframeName, keyframeManager)
                
                if (success) {
                    console.log("✅ 對齊關鍵幀合成成功:", outputPath)
                    statusHandler.showInfo("合成完成！正在載入...")
                    
                    // 載入合成後的影片
                    if (videoPlayer && typeof videoPlayer.loadVideo === 'function') {
                        videoPlayer.loadVideo(outputPath)
                    }
                } else {
                    console.log("❌ 對齊關鍵幀合成失敗")
                    statusHandler.showError("合成失敗，請檢查關鍵幀設定")
                }
            } else {
                console.log("❌ videoComposer.composeSideBySideAligned 不可用")
                statusHandler.showError("對齊合成功能不可用")
            }
        }
    }

    // 快照瀏覽器彈窗
    Popup {
        id: snapshotBrowser
        width: 700
        height: 500
        modal: true
        anchors.centerIn: Overlay.overlay
        
        Rectangle {
            anchors.fill: parent
            color: "#f0f0f0"
            border.color: "#d0d0d0"
            border.width: 1
            radius: 5
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10
                
                RowLayout {
                    Layout.fillWidth: true
                    
                    Text {
                        text: "快照瀏覽器"
                        font.pixelSize: 16
                        font.bold: true
                    }
                    
                    Item { Layout.fillWidth: true }
                    
                    Text {
                        text: "共 " + snapshotGrid.count + " 個快照"
                        font.pixelSize: 12
                        color: "#666"
                    }
                }
                
                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    
                    GridView {
                        id: snapshotGrid
                        cellWidth: 150
                        cellHeight: 150
                        model: ListModel { id: snapshotModel }
                        
                        delegate: Rectangle {
                            width: 140
                            height: 140
                            color: "#fff"
                            border.color: "#ccc"
                            border.width: 1
                            radius: 5
                            
                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 5
                                spacing: 5
                                
                                Image {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    source: model.filePath
                                    fillMode: Image.PreserveAspectFit
                                    asynchronous: true
                                    
                                    Text {
                                        anchors.centerIn: parent
                                        text: "載入中..."
                                        color: "#999"
                                        visible: parent.status === Image.Loading
                                    }
                                    
                                    Text {
                                        anchors.centerIn: parent
                                        text: "載入失敗"
                                        color: "#f44"
                                        visible: parent.status === Image.Error
                                    }
                                }
                                
                                Text {
                                    Layout.fillWidth: true
                                    text: model.fileName
                                    font.pixelSize: 10
                                    elide: Text.ElideMiddle
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }
                            
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    console.log("Displaying snapshot:", model.filePath)
                                    if (videoDisplayArea) {
                                        videoDisplayArea.showSnapshot(model.filePath)
                                        snapshotBrowser.close()
                                    }
                                }
                            }
                        }
                    }
                }
                
                Button {
                    text: "關閉"
                    Layout.alignment: Qt.AlignRight
                    onClicked: snapshotBrowser.close()
                }
            }
        }
        
        function refreshSnapshots() {
            console.log("Refreshing snapshots from /home/mxpt2/coachui/photo...")
            snapshotModel.clear()
            
            var photoDir = "/home/mxpt2/coachui/photo"
            
            // 使用 videoPlayer.getImageFiles 掃描目錄
            if (typeof videoPlayer !== 'undefined' && videoPlayer && 
                typeof videoPlayer.getImageFiles === 'function') {
                var files = videoPlayer.getImageFiles(photoDir)
                console.log("Found PNG files:", files.length)
                
                for (var i = 0; i < files.length; i++) {
                    var fileName = files[i]
                    var fullPath = "file://" + photoDir + "/" + fileName
                    console.log("Adding snapshot:", fileName)
                    snapshotModel.append({
                        fileName: fileName,
                        filePath: fullPath
                    })
                }
                
                console.log("Total snapshots:", snapshotModel.count)
            } else {
                console.error("videoPlayer.getImageFiles not available")
            }
        }
        
        onOpened: {
            refreshSnapshots()
        }
    }

    // ===== ROUTE VIDEO SELECTIONS TO VIDEOPLAYER =====
    Connections {
        target: appState
        function onVideoSelected(filePath) {
            if (videoPlayer && typeof videoPlayer.loadVideo === 'function') {
                console.log("🎬 Loading video:", filePath)
                videoPlayer.loadVideo(filePath)
            }
        }
    }

    // ===== RECORDER EVENTS =====
    Connections {
        target: recorder
        function onRecordingFinished(filePath) {
            statusHandler.showSuccess("錄製完成: " + filePath.split('/').pop())

            if (videoPlayer && typeof videoPlayer.loadVideo === 'function') {
                videoPlayer.loadVideo(filePath)
            }

            if (typeof appController !== 'undefined' && appController &&
                typeof appController.refreshVideoList === 'function') {
                appController.refreshVideoList()
            }
        }
    }

    // ===== APP INITIALIZATION =====
    Component.onCompleted: {
        console.log("")
        console.log("========== CoachUI INITIALIZED ==========")
        console.log("Window: " + width + " x " + height)
        console.log("Global C++ Context Properties:")
        console.log("  ✓ cameraController:", typeof cameraController !== 'undefined' ? "OK" : "MISSING")
        console.log("  ✓ videoPlayer:", typeof videoPlayer !== 'undefined' ? "OK" : "MISSING")
        console.log("  ✓ videoComposer:", typeof videoComposer !== 'undefined' ? "OK" : "MISSING")
        console.log("  ✓ recorder:", typeof recorder !== 'undefined' ? "OK" : "MISSING")
        console.log("  ✓ appController:", typeof appController !== 'undefined' ? "OK" : "MISSING")
        console.log("==========================================")
        console.log("")
    }
}
