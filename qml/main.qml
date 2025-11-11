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
        function onVideoSelected(filePath) {
            console.log("📼 Video selected:", filePath)
            appState.selectVideo(filePath)
            statusHandler.showInfo("載入影片: " + filePath.split('/').pop())
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
