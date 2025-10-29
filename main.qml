import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

ApplicationWindow {
    id: appWindow
    // Start the application in full screen by default on launch
    visibility: Window.FullScreen
    visible: true
    width: 800
    height: 600
    title: "CoachUI - 攝影機預覽與錄製"
    
    // 繪圖控制變數
    property bool drawingEnabled: false
    property string currentDrawingColor: "#FF0000"
    property real currentLineWidth: 3
    
    // 合成進度變數
    property bool compositionInProgress: false
    property int compositionCurrentFrame: 0
    property int compositionTotalFrames: 0
    
    // 導出進度變數
    property bool exportInProgress: false
    property int exportCurrentFrame: 0
    property int exportTotalFrames: 0
    
    // 強制刷新計數器
    property int refreshCounter: 0
    


    // CameraController is provided by C++ as a context property (cameraController)
    Connections {
        target: cameraController

        onCameraError: {
            errorText.text = error
            errorText.visible = true
            errorTimer.restart()
        }

        onPreviewStarted: {
            statusText.text = "預覽已啟動"
            statusText.color = "green"
        }

        onPreviewStopped: {
            statusText.text = "預覽已停止"
            statusText.color = "orange"
        }

        onFrameRateChanged: {
            var maxVal = frameRate > 0 ? Math.floor(1000000 / frameRate) : 10000
            var safeTo = Math.max(50, Math.floor(maxVal / 50) * 50)
            if (safeTo % 50 !== 0) safeTo = Math.ceil(safeTo / 50) * 50
            exposureSlider.to = safeTo
            console.log("Frame rate changed to", frameRate, "fps, max exposure:", exposureSlider.to, "μs")
        }
    }

    // Ensure initial preview status is reflected after QML loads
    Component.onCompleted: {
        if (typeof cameraController !== 'undefined' && cameraController.isPreviewActive) {
            statusText.text = "預覽已啟動"
            statusText.color = "green"
        }
        if (typeof videoPlayer !== 'undefined') console.log("C++ VideoPlayer available in QML")
    }

    // 全螢幕快捷鍵（F11）
    Shortcut {
        sequence: "F11"
        onActivated: {
            if (appWindow.visibility === Window.FullScreen) appWindow.visibility = Window.Windowed
            else appWindow.visibility = Window.FullScreen
        }
    }

    // 全螢幕切換按鈕（右上角）
    Button {
        id: fullscreenButton
        text: appWindow.visibility === Window.FullScreen ? "退出全螢幕" : "全螢幕"
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 8
        z: 999
        onClicked: {
            if (appWindow.visibility === Window.FullScreen) appWindow.visibility = Window.Windowed
            else appWindow.visibility = Window.FullScreen
        }
    }

    // VideoPlayer is provided by C++ as a context property (videoPlayer)
    Connections {
        target: videoPlayer

        onVideoLoaded: {
            statusText.text = "影片已載入，GStreamer 播放中..."
            statusText.color = "blue"
            console.log("Video loaded successfully, GStreamer playback started")
        }

        onPlaybackError: function(errorMessage) {
            statusText.text = "播放錯誤: " + errorMessage
            statusText.color = "red"
            console.log("Playback error:", errorMessage)
        }
    }

    
    // VideoComposer is provided by C++ as a context property (videoComposer)
    Connections {
        target: videoComposer

        onVideoLoaded: function(filename) {
            statusText.text = "影片已載入: " + filename
            statusText.color = "green"
            console.log("VideoComposer loaded:", filename)
        }

        onVideoLoadError: function(error) {
            statusText.text = error
            statusText.color = "red"
            console.log("VideoComposer error:", error)
        }

        onStatusChanged: {
            console.log("VideoComposer status:", status)
        }

        onCompositionStarted: {
            compositionInProgress = true
            compositionCurrentFrame = 0
            compositionTotalFrames = 0
            statusText.text = "開始合成影片..."
            statusText.color = "blue"
            console.log("Composition started, compositionInProgress:", compositionInProgress)
        }

        onCompositionProgress: function(current, total) {
            compositionCurrentFrame = current
            compositionTotalFrames = total
            console.log("Composition progress:", current, "/", total)
        }

        onCompositionCompleted: function(frameCount) {
            compositionInProgress = false
            compositionCurrentFrame = frameCount
            compositionTotalFrames = frameCount
            statusText.text = "合成完成！開始自動導出..."
            statusText.color = "green"

            // 要求 C++ 進行導出（C++ 端會觸發 export events）
            var timestamp = Math.floor(Date.now() / 1000)
            var outputPath = "/home/mxpt2/Videos/composed_" + timestamp + ".mp4"
            videoComposer.exportComposedVideo(outputPath)
        }

        onCompositionError: function(error) {
            compositionInProgress = false
            compositionCurrentFrame = 0
            compositionTotalFrames = 0
            statusText.text = "合成錯誤: " + error
            statusText.color = "red"
        }

        onExportStarted: {
            exportInProgress = true
            exportCurrentFrame = 0
            exportTotalFrames = 0
            refreshCounter++
            statusText.text = "開始導出影片..."
            statusText.color = "blue"
            console.log("Export started, exportInProgress:", exportInProgress)
        }

        onExportProgress: function(current, total) {
            exportCurrentFrame = current
            exportTotalFrames = total
            statusText.text = "導出進度: " + current + "/" + total + " (" + Math.round(current/total*100) + "%)"
            statusText.color = "blue"
            console.log("Export progress:", current, "/", total)
        }

        onExportCompleted: function(filePath) {
            exportInProgress = false
            exportCurrentFrame = exportTotalFrames
            refreshCounter++
            statusText.text = "影片已導出並載入播放器: " + filePath.split('/').pop()
            statusText.color = "green"

            // 請 C++ 的 videoPlayer 處理載入與播放
            videoPlayer.loadVideo(filePath)
        }

        onExportError: function(error) {
            exportInProgress = false
            exportCurrentFrame = 0
            exportTotalFrames = 0
            statusText.text = "導出錯誤: " + error
            statusText.color = "red"
        }
    }
    
    Connections {
        target: recorder
        function onRecordingFinished(filePath) {
            // 錄製完成後自動載入影片
            cameraController.stopPreview()
            
            console.log("Recording finished, file:", filePath)
            
            // 刷新影片列表，讓新檔案出現在選單中
            refreshVideoList()
            
            // 檢查檔案格式並選擇適當的播放方法
            if (filePath.endsWith(".h264")) {
                console.log("Attempting to play H.264 file directly")
                videoPlayer.playH264Direct(filePath)
                statusText.text = "錄製完成，正在載入 H.264 影片..."
            } else if (filePath.endsWith(".mp4")) {
                console.log("Loading MP4 file")
                videoPlayer.loadVideo(filePath)
                statusText.text = "錄製完成，MP4 影片已載入"
            } else {
                console.log("Loading video file:", filePath)
                videoPlayer.loadVideo(filePath)
                statusText.text = "錄製完成，影片已載入"
            }
            
            statusText.color = "green"
        }
    }
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10
        
        // 控制面板
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 140
            color: "#f0f0f0"
            border.color: "#d0d0d0"
            border.width: 1
            radius: 5
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 8
                
                // 攝影機控制行
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    
                    Text {
                        text: "攝影機:"
                        Layout.alignment: Qt.AlignVCenter
                    }
                    
                    ComboBox {
                        id: cameraCombo
                        model: cameraController ? cameraController.availableCameras : []
                        Layout.preferredWidth: 150
                        
                        onCurrentTextChanged: {
                            if (currentText) {
                                cameraController.setCurrentCamera(currentText)
                            }
                        }
                    }
                    
                    Button {
                        text: cameraController && cameraController.isPreviewActive ? "停止預覽" : "開始預覽"
                        Layout.preferredWidth: 100
                        
                        onClicked: {
                                if (cameraController && cameraController.isPreviewActive) {
                                    cameraController.stopPreview()
                                } else if (cameraController) {
                                    cameraController.startPreview()
                                }
                        }
                    }
                    
                    Button {
                        text: "重新整理"
                        Layout.preferredWidth: 90
                        onClicked: cameraController ? cameraController.refreshCameras() : undefined
                    }
                    
                    Slider {
                        id: exposureSlider
                        Layout.preferredWidth: 200
                        from: 50           // 50μs 最短
                        to: {
                            var maxVal = cameraController ? Math.floor(1000000 / cameraController.frameRate) : 10000
                            var safeTo = Math.max(50, Math.floor(maxVal / 50) * 50)
                            // 確保 safeTo 為 stepSize 的整數倍且 >= from
                            return safeTo
                        }
                        value: cameraController ? cameraController.exposureValue : 4000
                        stepSize: 50
                        onValueChanged: {
                            if (cameraController && Math.abs(value - cameraController.exposureValue) > 25) {
                                cameraController.setExposureValue(Math.round(value))
                            }
                        }
                    }
                    
                    Text {
                        text: cameraController ? (cameraController.exposureValue / 1000).toFixed(2) + "ms" : "-- ms"
                        Layout.preferredWidth: 55
                        Layout.alignment: Qt.AlignVCenter
                        font.family: "monospace"
                    }
                    
                    Button {
                        text: "自動"
                        Layout.preferredWidth: 60
                        onClicked: cameraController ? cameraController.setExposureValue(4000) : undefined   // 重置為4ms (適合120fps)
                    }
                    
                    Item {
                        Layout.fillWidth: true
                    }
                }
                
                // 錄製和播放控制行
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    
                    Text {
                        text: "錄製:"
                        Layout.alignment: Qt.AlignVCenter
                    }
                    
                    ComboBox {
                        id: frameRateCombo
                        Layout.preferredWidth: 80
                        model: ["30fps", "60fps", "90fps", "120fps"]
                        currentIndex: 3  // 默認選擇 120fps
                        
                        onCurrentTextChanged: {
                            var fps = parseInt(currentText.replace("fps", ""))
                            cameraController.setFrameRate(fps)
                            console.log("Frame rate changed to:", fps)
                        }
                    }
                    
                    Button {
                        text: "開始錄製"
                        Layout.preferredWidth: 90
                        onClicked: {
                            if (cameraController.isPreviewActive) {
                                cameraController.stopPreview()
                            }
                            Qt.callLater(function() {
                                var fps = cameraController.frameRate
                                recorder.startRecording("/home/mxpt2/Videos/test_" + Date.now() + ".h264", fps)
                            })
                        }
                    }

                    Button {
                        text: "停止錄製"
                        Layout.preferredWidth: 90
                        onClicked: recorder.stopRecording()
                    }
                    
                    Button {
                        text: "載入影片"
                        Layout.preferredWidth: 90
                        onClicked: {
                            videoListPopup.open()
                        }
                    }
                    

                    
                    Button {
                        text: "測試播放"
                        Layout.preferredWidth: 90
                        onClicked: {
                            console.log("Loading test video...")
                            videoPlayer.loadVideo("/home/mxpt2/coachui/test_video.mp4")
                        }
                    }
                    

                    
                    Item {
                        Layout.fillWidth: true
                    }
                }
                
                // 播放控制區域
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    color: "#e0e0e0"
                    radius: 5
                    visible: videoPlayer.currentFile !== ""
                    
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 5
                        spacing: 10
                        
                        // 播放控制按鈕和速度控制
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 5
                            
                            Button {
                                text: videoPlayer.isPlaying ? "暫停" : "播放"
                                Layout.preferredWidth: 50
                                Layout.preferredHeight: 30
                                font.pixelSize: 12
                                onClicked: videoPlayer.isPlaying ? videoPlayer.pause() : videoPlayer.play()
                            }
                            
                            Button {
                                text: "停止"
                                Layout.preferredWidth: 50
                                Layout.preferredHeight: 30
                                font.pixelSize: 12
                                onClicked: videoPlayer.stop()
                            }
                            
                            // 速度控制按鈕
                            Text {
                                text: "速度:"
                                Layout.alignment: Qt.AlignVCenter
                                font.pixelSize: 11
                            }
                            
                            Button {
                                text: "0.25x"
                                Layout.preferredWidth: 35
                                Layout.preferredHeight: 28
                                font.pixelSize: 9
                                font.bold: Math.abs(videoPlayer.playbackRate - 0.25) < 0.01
                                onClicked: videoPlayer.setPlaybackRate(0.25)
                            }
                            
                            Button {
                                text: "0.5x"
                                Layout.preferredWidth: 32
                                Layout.preferredHeight: 28
                                font.pixelSize: 9
                                font.bold: Math.abs(videoPlayer.playbackRate - 0.5) < 0.01
                                onClicked: videoPlayer.setPlaybackRate(0.5)
                            }
                            
                            Button {
                                text: "1.0x"
                                Layout.preferredWidth: 32
                                Layout.preferredHeight: 28
                                font.pixelSize: 9
                                font.bold: Math.abs(videoPlayer.playbackRate - 1.0) < 0.01
                                onClicked: videoPlayer.setPlaybackRate(1.0)
                            }
                            
                            Button {
                                text: "1.5x"
                                Layout.preferredWidth: 32
                                Layout.preferredHeight: 28
                                font.pixelSize: 9
                                font.bold: Math.abs(videoPlayer.playbackRate - 1.5) < 0.01
                                onClicked: videoPlayer.setPlaybackRate(1.5)
                            }
                            
                            Button {
                                text: "2x"
                                Layout.preferredWidth: 28
                                Layout.preferredHeight: 28
                                font.pixelSize: 9
                                font.bold: Math.abs(videoPlayer.playbackRate - 2.0) < 0.01
                                onClicked: videoPlayer.setPlaybackRate(2.0)
                            }
                            
                            Text {
                                text: videoPlayer.playbackRate.toFixed(2) + "x"
                                Layout.alignment: Qt.AlignVCenter
                                font.pixelSize: 10
                                Layout.preferredWidth: 40
                            }
                            
                            // 分隔線
                            Rectangle {
                                width: 1
                                height: 20
                                color: "#aaa"
                                Layout.alignment: Qt.AlignVCenter
                            }
                            
                            // 並排顯示控制
                            Button {
                                text: videoPlayer.isSideBySideMode ? "單一" : "並排"
                                Layout.preferredWidth: 35
                                Layout.preferredHeight: 28
                                font.pixelSize: 9
                                enabled: videoComposer.selectedVideoCount >= 2
                                background: Rectangle {
                                    color: videoPlayer.isSideBySideMode ? "#FF9800" : 
                                           (videoComposer.selectedVideoCount >= 2 ? "#4CAF50" : "#e0e0e0")
                                    radius: 4
                                }
                                onClicked: {
                                    console.log("Side-by-side button clicked, selected videos:", videoComposer.selectedVideoCount)
                                    if (videoPlayer.isSideBySideMode) {
                                        videoPlayer.disableSideBySideMode()
                                    } else {
                                        var paths = videoComposer.getSelectedVideoPaths()
                                        console.log("Retrieved video paths:", paths)
                                        if (paths.length >= 2) {
                                            videoPlayer.enableSideBySideMode(paths, true)
                                        }
                                    }
                                }
                            }
                            
                            // 調試：顯示選中影片數
                            Text {
                                text: "(" + videoComposer.selectedVideoCount + ")"
                                Layout.alignment: Qt.AlignVCenter
                                font.pixelSize: 8
                                color: videoComposer.selectedVideoCount >= 2 ? "green" : "gray"
                                visible: true
                            }
                            
                            Button {
                                text: videoPlayer.sideBySideHorizontal ? "橫" : "直"
                                Layout.preferredWidth: 25
                                Layout.preferredHeight: 28
                                font.pixelSize: 9
                                visible: videoPlayer.isSideBySideMode
                                enabled: videoPlayer.isSideBySideMode
                                onClicked: {
                                    var paths = videoComposer.getSelectedVideoPaths()
                                    if (paths.length >= 2) {
                                        videoPlayer.enableSideBySideMode(paths, !videoPlayer.sideBySideHorizontal)
                                    }
                                }
                            }
                            
                            // 分隔線
                            Rectangle {
                                width: 1
                                height: 20
                                color: "#aaa"
                                Layout.alignment: Qt.AlignVCenter
                                visible: videoPlayer.isSideBySideMode
                            }
                            
                            // 繪圖控制按鈕
                            Button {
                                text: drawingEnabled ? "繪圖" : "繪圖"
                                Layout.preferredWidth: 40
                                Layout.preferredHeight: 28
                                font.pixelSize: 9
                                enabled: videoPlayer.currentFile !== ""
                                background: Rectangle {
                                    color: drawingEnabled ? "#4CAF50" : "#e0e0e0"
                                    radius: 4
                                }
                                onClicked: {
                                    drawingEnabled = !drawingEnabled
                                    console.log("繪圖模式:", drawingEnabled ? "開啟" : "關閉")
                                }
                            }
                            
                            Button {
                                text: "清除"
                                Layout.preferredWidth: 35
                                Layout.preferredHeight: 28
                                font.pixelSize: 9
                                enabled: videoPlayer.currentFile !== ""
                                onClicked: {
                                    if (drawingCanvas) {
                                        drawingCanvas.clearDrawing()
                                    }
                                }
                            }
                            
                            Button {
                                text: "撤銷"
                                Layout.preferredWidth: 35
                                Layout.preferredHeight: 28
                                font.pixelSize: 9
                                enabled: videoPlayer.currentFile !== ""
                                onClicked: {
                                    if (drawingCanvas) {
                                        drawingCanvas.undoLastLine()
                                    }
                                }
                            }
                        }
                        

                    }
                }
                
                Text {
                    id: statusText
                    text: "就緒"
                    color: "black"
                    Layout.alignment: Qt.AlignVCenter
                }
            }
        }
        
        // 視訊預覽區域
        Rectangle {
            id: videoContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1a1a1a"
            border.color: "#333333"
            border.width: 2
            radius: 5
            
            Rectangle {
                anchors.fill: parent
                anchors.margins: 2
                color: "#2a2a2a"
                border.color: "#555555"
                border.width: 1
                
                // 視訊輸出區域 - 單一或並排模式
                Item {
                    anchors.fill: parent
                    anchors.margins: 5
                    visible: videoPlayer.currentFile !== "" || videoPlayer.isSideBySideMode

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton
                        onDoubleClicked: {
                            if (appWindow.visibility === Window.FullScreen) appWindow.visibility = Window.Windowed
                            else appWindow.visibility = Window.FullScreen
                        }
                    }
                    
                    // 單一視訊輸出
                    VideoOutput {
                        id: videoOutput
                        anchors.fill: parent
                        visible: !videoPlayer.isSideBySideMode && videoPlayer.currentFile !== ""
                        
                        Component.onCompleted: {
                            // 連接 VideoPlayer 的 videoSink
                            if (videoPlayer) {
                                videoPlayer.setVideoSink(videoSink)
                                console.log("VideoOutput connected to VideoPlayer")
                            }
                        }
                    }
                    
                    // 並排視訊輸出 - 簡化版本
                    Item {
                        id: sideBySideContainer
                        anchors.fill: parent
                        visible: videoPlayer.isSideBySideMode
                        
                        property var videoSources: videoPlayer.sideBySideVideos || []
                        property bool horizontal: videoPlayer.sideBySideHorizontal
                        
                        // 左側或上方視頻
                        Rectangle {
                            id: video1Container
                            width: sideBySideContainer.horizontal ? parent.width / 2 : parent.width
                            height: sideBySideContainer.horizontal ? parent.height : parent.height / 2
                            x: 0
                            y: 0
                            color: "#1a1a1a"
                            border.color: "#4CAF50"
                            border.width: 2
                            
                            VideoOutput {
                                id: videoOutput1
                                anchors.fill: parent
                                anchors.margins: 2
                            }
                            
                            MediaPlayer {
                                id: player1
                                source: sideBySideContainer.videoSources.length > 0 ? 
                                       "file://" + sideBySideContainer.videoSources[0] : ""
                                audioOutput: AudioOutput { }
                                videoOutput: videoOutput1
                                
                                Component.onCompleted: {
                                    if (source !== "") {
                                        play()
                                    }
                                }
                                
                                onSourceChanged: function() {
                                    if (source !== "") {
                                        play()
                                    }
                                }
                                
                                // 同步duration到主VideoPlayer
                                onDurationChanged: {
                                    // 使用第一個視頻的duration作為主duration
                                    if (sideBySideContainer.videoSources.length > 0) {
                                        videoPlayer.updateSideBySideDuration(duration)
                                        console.log("Player1 duration synced:", duration)
                                    }
                                }
                                
                                // 只有主視頻驅動position更新，避免衝突
                                onPositionChanged: {
                                    if (!sideBySideContainer.syncInProgress) {
                                        videoPlayer.updateSideBySidePosition(position)
                                    }
                                }
                            }
                            
                            Rectangle {
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.right: parent.right
                                height: 25
                                color: "black"
                                opacity: 0.7
                                
                                Text {
                                    anchors.fill: parent
                                    anchors.margins: 4
                                    text: "影片1: " + (sideBySideContainer.videoSources.length > 0 ? 
                                                  sideBySideContainer.videoSources[0].split('/').pop() : "")
                                    color: "white"
                                    font.pixelSize: 10
                                    elide: Text.ElideMiddle
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                        
                        // 右側或下方視頻
                        Rectangle {
                            id: video2Container
                            width: sideBySideContainer.horizontal ? parent.width / 2 : parent.width
                            height: sideBySideContainer.horizontal ? parent.height : parent.height / 2
                            x: sideBySideContainer.horizontal ? parent.width / 2 : 0
                            y: sideBySideContainer.horizontal ? 0 : parent.height / 2
                            color: "#1a1a1a"
                            border.color: "#2196F3"
                            border.width: 2
                            
                            VideoOutput {
                                id: videoOutput2
                                anchors.fill: parent
                                anchors.margins: 2
                            }
                            
                            MediaPlayer {
                                id: player2
                                source: sideBySideContainer.videoSources.length > 1 ? 
                                       "file://" + sideBySideContainer.videoSources[1] : ""
                                // 第二個視頻靜音，避免音頻衝突
                                audioOutput: AudioOutput { volume: 0 }
                                videoOutput: videoOutput2
                                
                                Component.onCompleted: {
                                    if (source !== "") {
                                        play()
                                    }
                                }
                                
                                onSourceChanged: function() {
                                    if (source !== "") {
                                        play()
                                    }
                                }
                            }
                            
                            Rectangle {
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.right: parent.right
                                height: 25
                                color: "black"
                                opacity: 0.7
                                
                                Text {
                                    anchors.fill: parent
                                    anchors.margins: 4
                                    text: "影片2: " + (sideBySideContainer.videoSources.length > 1 ? 
                                                  sideBySideContainer.videoSources[1].split('/').pop() : "")
                                    color: "white"
                                    font.pixelSize: 10
                                    elide: Text.ElideMiddle
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                        
                        // 同步播放控制
                        Connections {
                            target: videoPlayer
                            function onIsPlayingChanged() {
                                if (videoPlayer.isPlaying) {
                                    player1.play()
                                    player2.play()
                                } else {
                                    // 暫停或停止
                                    player1.pause()
                                    player2.pause()
                                }
                            }
                        }
                        
                        // 同步播放速度
                        Connections {
                            target: videoPlayer
                            function onPlaybackRateChanged() {
                                player1.playbackRate = videoPlayer.playbackRate
                                player2.playbackRate = videoPlayer.playbackRate
                            }
                        }
                        
                        // 主從同步架構 - 優化預覽模式
                        property bool syncInProgress: false
                        
                        Connections {
                            target: videoPlayer
                            function onPositionChanged() {
                                if (sideBySideContainer.syncInProgress) return
                                
                                sideBySideContainer.syncInProgress = true
                                
                                var targetPosition = videoPlayer.position
                                
                                // 停止檢查
                                if (targetPosition === 0 && !videoPlayer.isPlaying) {
                                    player1.stop()
                                    player2.stop()
                                } else {
                                    // 流暢的實時預覽同步
                                    if (player1.duration > 0 && targetPosition <= player1.duration) {
                                        // 在預覽模式下，直接設置position不會觸發播放
                                        player1.position = targetPosition
                                    }
                                    
                                    // 從視頻按比例同步，保持流暢預覽
                                    if (player2.duration > 0 && player1.duration > 0) {
                                        var ratio = player2.duration / player1.duration
                                        var slavePosition = Math.min(targetPosition * ratio, player2.duration)
                                        player2.position = slavePosition
                                    }
                                }
                                
                                sideBySideContainer.syncInProgress = false
                            }
                        }
                    }
                }
                
                // 繪圖層 - 覆蓋在影片上方
                Item {
                    anchors.fill: parent
                    anchors.margins: 5
                    visible: videoPlayer.currentFile !== "" && drawingEnabled
                    
                    Canvas {
                        id: drawingCanvas
                        anchors.fill: parent
                        
                        property var lines: []
                        property var currentLine: []
                        property bool isDrawing: false
                        property string drawingColor: currentDrawingColor
                        property real lineWidth: currentLineWidth
                        
                        MouseArea {
                            anchors.fill: parent
                            
                            onPressed: {
                                if (drawingEnabled) {
                                    drawingCanvas.isDrawing = true
                                    drawingCanvas.currentLine = [Qt.point(mouse.x, mouse.y)]
                                    console.log("開始繪圖 at:", mouse.x, mouse.y)
                                }
                            }
                            
                            onPositionChanged: {
                                if (drawingCanvas.isDrawing && drawingEnabled) {
                                    drawingCanvas.currentLine.push(Qt.point(mouse.x, mouse.y))
                                    drawingCanvas.requestPaint()
                                }
                            }
                            
                            onReleased: {
                                if (drawingCanvas.isDrawing && drawingEnabled) {
                                    drawingCanvas.isDrawing = false
                                    if (drawingCanvas.currentLine.length > 1) {
                                        var newLine = {
                                            points: [...drawingCanvas.currentLine],
                                            color: drawingCanvas.drawingColor,
                                            width: drawingCanvas.lineWidth
                                        }
                                        drawingCanvas.lines.push(newLine)
                                        console.log("完成線條，共", newLine.points.length, "點")
                                    }
                                    drawingCanvas.currentLine = []
                                    drawingCanvas.requestPaint()
                                }
                            }
                        }
                        
                        onPaint: {
                            var ctx = getContext("2d")
                            ctx.clearRect(0, 0, width, height)
                            
                            // 繪製完成的線條
                            for (var i = 0; i < lines.length; i++) {
                                var line = lines[i]
                                if (line.points && line.points.length > 1) {
                                    ctx.strokeStyle = line.color || drawingColor
                                    ctx.lineWidth = line.width || lineWidth
                                    ctx.lineCap = "round"
                                    ctx.lineJoin = "round"
                                    
                                    ctx.beginPath()
                                    ctx.moveTo(line.points[0].x, line.points[0].y)
                                    for (var j = 1; j < line.points.length; j++) {
                                        ctx.lineTo(line.points[j].x, line.points[j].y)
                                    }
                                    ctx.stroke()
                                }
                            }
                            
                            // 繪製當前正在畫的線條
                            if (currentLine.length > 1) {
                                ctx.strokeStyle = drawingColor
                                ctx.lineWidth = lineWidth
                                ctx.lineCap = "round"
                                ctx.lineJoin = "round"
                                
                                ctx.beginPath()
                                ctx.moveTo(currentLine[0].x, currentLine[0].y)
                                for (var k = 1; k < currentLine.length; k++) {
                                    ctx.lineTo(currentLine[k].x, currentLine[k].y)
                                }
                                ctx.stroke()
                            }
                        }
                        
                        // 清除所有繪圖的函數
                        function clearDrawing() {
                            lines = []
                            currentLine = []
                            requestPaint()
                            console.log("清除所有繪圖")
                        }
                        
                        // 撤銷上一條線的函數
                        function undoLastLine() {
                            if (lines.length > 0) {
                                lines.pop()
                                requestPaint()
                                console.log("撤銷上一條線，剩餘", lines.length, "條線")
                            }
                        }
                    }
                }
                
                // 播放狀態覆蓋層
                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 35
                    color: "black"
                    opacity: 0.8
                        radius: 3
                        visible: videoPlayer && videoPlayer.currentFile !== ""
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 10
                        
                        Text {
                           text: videoPlayer && videoPlayer.isPlaying ? "播放中" : "暫停"
                           color: videoPlayer && videoPlayer.isPlaying ? "#4CAF50" : "#FFC107"
                            font.pixelSize: 12
                            Layout.alignment: Qt.AlignVCenter
                        }
                        
                        Text {
                           text: videoPlayer && videoPlayer.currentFile ? videoPlayer.currentFile.split('/').pop() : ""
                            color: "white"
                            font.pixelSize: 12
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter
                            elide: Text.ElideMiddle
                        }
                        
                        Text {
                           text: videoPlayer && videoPlayer.playbackRate ? videoPlayer.playbackRate.toFixed(1) + "x" : "1.0x"
                            color: "#2196F3"
                            font.pixelSize: 12
                            font.bold: true
                            Layout.alignment: Qt.AlignVCenter
                        }
                    }
                }
                
                // 預覽/待機狀態顯示
                Column {
                    anchors.centerIn: parent
                    spacing: 10
                    visible: videoPlayer && videoPlayer.currentFile === ""
                    
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: cameraController && cameraController.isPreviewActive ? "攝影機預覽" : "影片播放器"
                        color: "white"
                        font.pixelSize: 24
                    }
                    
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: cameraController.isPreviewActive ? 
                        "預覽視窗已在獨立窗口顯示" : 
                        "載入影片以開始播放"
                    color: cameraController && cameraController.isPreviewActive ? "#4CAF50" : "#FFC107"
                        font.pixelSize: 16
                        horizontalAlignment: Text.AlignHCenter
                    }
                    
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: {
                            if (videoPlayer.currentFile === "") {
                                return "點擊「載入影片」選擇檔案"
                            } else if (videoPlayer.currentFile.includes("composed_")) {
                                return "🎬 播放合成結果"
                            } else {
                                return "📹 播放原始影片"
                            }
                        }
                        color: {
                            if (videoPlayer.currentFile === "") {
                                return "#888888"
                            } else if (videoPlayer.currentFile.includes("composed_")) {
                                return "#4CAF50"  // 綠色表示合成結果
                            } else {
                                return "#2196F3"  // 藍色表示原始影片
                            }
                        }
                        font.pixelSize: 12
                        font.bold: videoPlayer.currentFile.includes("composed_")
                    }
                }
            }
        }
        
        // 進度條區域
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: "#f5f5f5"
            border.color: "#d0d0d0"
            border.width: 1
            radius: 5
            visible: videoPlayer.currentFile !== ""
            
            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10
                
                Slider {
                    id: positionSlider
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    from: 0
                    to: videoPlayer.duration
                    value: videoPlayer.position

                    property bool userDragging: false
                    property real pendingValue: 0
                    property bool wasPlayingBeforeDrag: false

                    // 高頻率更新Timer，實現流暢預覽
                    Timer {
                        id: seekTimer
                        interval: 10  // 10ms更新，更流暢的預覽
                        onTriggered: {
                            if (positionSlider.userDragging) {
                                // 使用預覽模式，避免觸發播放狀態變化
                                videoPlayer.setPreviewPosition(positionSlider.pendingValue)
                            }
                        }
                    }

                    onPressedChanged: {
                        if (pressed) {
                            // 開始拖動
                            userDragging = true
                            wasPlayingBeforeDrag = videoPlayer.isPlaying

                            // 暫停播放，避免畫面抖動
                            if (videoPlayer.isPlaying) {
                                videoPlayer.pause()
                            }

                            seekTimer.stop()
                        } else {
                            // 拖動結束
                            userDragging = false
                            seekTimer.stop()

                            // 設置最終位置
                            videoPlayer.setPosition(value)

                            // 恢復播放狀態
                            if (wasPlayingBeforeDrag) {
                                videoPlayer.play()
                            }
                        }
                    }

                    // 實時更新 - 高頻率節流
                    onValueChanged: {
                        if (userDragging) {
                            pendingValue = value
                            seekTimer.restart()
                        }
                    }

                    // 防止播放時進度更新覆蓋用戶拖動
                    Connections {
                        target: videoPlayer
                        function onPositionChanged() {
                            if (!positionSlider.userDragging) {
                                positionSlider.value = videoPlayer.position
                            }
                        }
                    }
                }
                
                Text {
                    text: formatTime(videoPlayer.position) + " / " + formatTime(videoPlayer.duration)
                    Layout.preferredWidth: 100
                    Layout.alignment: Qt.AlignVCenter
                    font.pointSize: 10
                    font.family: "monospace"
                    color: "#333333"
                }
            }
        }
        
        // 影片合成面板 - 全新設計
        Rectangle {
            id: compositePanel
            Layout.fillWidth: true
            Layout.preferredHeight: collapsed ? 45 : 350
            Layout.maximumHeight: collapsed ? 45 : 380
            color: collapsed ? "#f5f5f5" : "#e8f4fd"
            border.color: videoComposer.videoCount > 0 ? "#4CAF50" : "#ddd"
            border.width: videoComposer.videoCount > 0 ? 2 : 1
            radius: 8
            
            property bool collapsed: true
            
            Behavior on Layout.preferredHeight {
                NumberAnimation { duration: 300; easing.type: Easing.OutCubic }
            }
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 6
                
                // 可點擊的標題欄
                Rectangle {
                    Layout.fillWidth: true
                    height: 35
                    color: "transparent"
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 4
                        spacing: 8
                        
                        Text {
                            text: compositePanel.collapsed ? "▶" : "▼"
                            font.pointSize: 12
                            color: "#666"
                        }
                        
                        Text {
                            text: "影片合成器"
                            font.pointSize: 12
                            font.bold: true
                            color: "#2E7D32"
                        }
                        
                        Text {
                           text: "(" + (videoComposer ? videoComposer.videoCount : 0) + " 個影片)"
                            font.pointSize: 10
                            color: "#666"
                        }
                        
                        Item { Layout.fillWidth: true }
                        
                        // 算法狀態指示器
                        Column {
                            spacing: 2
                            visible: !compositePanel.collapsed
                            
                            Text {
                                text: videoComposer.selectedVideoCount >= 2 ? "可合成" : "需選擇2個以上影片"
                                    font.pointSize: 9
                                    color: videoComposer && videoComposer.selectedVideoCount >= 2 ? "#4CAF50" : "#FF9800"
                            }
                            
                            Text {
                                text: {
                                 var features = []
                                 if (videoComposer && videoComposer.adaptiveThreshold) features.push("自適應")
                                 if (videoComposer && videoComposer.edgeEnhancement) features.push("邊緣增強")
                                 if (videoComposer && videoComposer.centerWeighting) features.push("中心加權")
                                 return features.length > 0 ? "算法: " + features.join("、") : "算法: 基礎模式"
                                }
                                font.pointSize: 7
                                color: "#666"
                            }
                        }
                    }
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            compositePanel.collapsed = !compositePanel.collapsed
                        }
                        cursorShape: Qt.PointingHandCursor
                    }
                }
                
                // 載入的影片列表
                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 85
                    Layout.maximumHeight: 100
                    visible: !compositePanel.collapsed
                    
                    // 空狀態提示
                    Rectangle {
                        anchors.fill: parent
                        color: "#f9f9f9"
                        border.color: "#e0e0e0"
                        border.width: 1
                        radius: 6
                        visible: videoComposer && videoComposer.videoCount === 0
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 4
                            
                            Text {
                                text: "[ 空 ]"
                                font.pixelSize: 18
                                anchors.horizontalCenter: parent.horizontalCenter
                                color: "#999"
                            }
                            
                            Text {
                                text: "點擊「載入影片」開始合成"
                                font.pointSize: 10
                                color: "#999"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }
                    
                    // 影片列表
                    ScrollView {
                        anchors.fill: parent
                        visible: videoComposer.videoCount > 0
                        
                        ListView {
                            id: compositeVideoList
                            model: videoComposer.videoList
                            
                            delegate: Rectangle {
                                width: compositeVideoList.width
                                height: 28
                                color: modelData.selected ? "#C8E6C9" : "transparent"
                                border.color: modelData.selected ? "#4CAF50" : "#e0e0e0"
                                border.width: 1
                                radius: 4
                                
                                RowLayout {
                                    anchors.fill: parent
                                    anchors.margins: 6
                                    spacing: 8
                                    
                                    CheckBox {
                                        id: videoCheckBox
                                        checked: modelData.selected
                                        scale: 0.8
                                        onToggled: {
                                            videoComposer.toggleVideoSelection(index)
                                        }
                                    }
                                    
                                    Text {
                                        text: modelData.name
                                        font.pointSize: 9
                                        Layout.fillWidth: true
                                        elide: Text.ElideMiddle
                                    }
                                    
                                    Text {
                                        text: modelData.info
                                        font.pointSize: 8
                                        color: "#666"
                                        Layout.preferredWidth: 120
                                        elide: Text.ElideRight
                                    }
                                    
                                    Button {
                                        text: "X"
                                        Layout.preferredWidth: 22
                                        Layout.preferredHeight: 20
                                        font.pointSize: 8
                                        onClicked: {
                                            videoComposer.removeVideo(index)
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                
                // 合成參數控制 - 2x2網格佈局
                GroupBox {
                    title: "合成參數"
                    Layout.fillWidth: true
                    Layout.preferredHeight: 85
                    visible: !compositePanel.collapsed && videoComposer.videoCount > 0
                    
                    GridLayout {
                        anchors.fill: parent
                        columns: 3
                        rowSpacing: 6
                        columnSpacing: 10
                        
                        // 前景強度 (左上)
                        Column {
                            Layout.fillWidth: true
                            Text { 
                                text: "前景強度: " + fgWeightSlider.value.toFixed(1)
                                font.pointSize: 9 
                                color: "#333"
                            }
                            Slider {
                                id: fgWeightSlider
                                Layout.fillWidth: true
                                Layout.preferredHeight: 22
                                from: 0.1
                                to: 4.0
                                value: videoComposer.foregroundWeight
                                stepSize: 0.1
                                onValueChanged: {
                                    videoComposer.setForegroundWeight(value)
                                }
                            }
                        }
                        
                        // 背景強度 (右上)
                        Column {
                            Layout.fillWidth: true
                            Text { 
                                text: "背景強度: " + bgWeightSlider.value.toFixed(1)
                                font.pointSize: 9 
                                color: "#333"
                            }
                            Slider {
                                id: bgWeightSlider
                                Layout.fillWidth: true
                                Layout.preferredHeight: 22
                                from: 0.0
                                to: 1.0
                                value: videoComposer.backgroundWeight
                                stepSize: 0.1
                                onValueChanged: {
                                    videoComposer.setBackgroundWeight(value)
                                }
                            }
                        }
                        
                        // 模糊大小 (左下)
                        Column {
                            Layout.fillWidth: true
                            Text { 
                                text: "模糊大小: " + blurSizeSlider.value.toFixed(0)
                                font.pointSize: 9 
                                color: "#333"
                            }
                            Slider {
                                id: blurSizeSlider
                                Layout.fillWidth: true
                                Layout.preferredHeight: 22
                                from: 1
                                to: 50
                                value: videoComposer.blurSize
                                stepSize: 2
                                onValueChanged: {
                                    videoComposer.setBlurSize(Math.round(value))
                                }
                            }
                        }
                        
                        // 前景閾值 (右上)
                        Column {
                            Layout.fillWidth: true
                            Text { 
                                text: "前景閾值: " + thresholdSlider.value.toFixed(1)
                                font.pointSize: 9 
                                color: "#333"
                            }
                            Slider {
                                id: thresholdSlider
                                Layout.fillWidth: true
                                Layout.preferredHeight: 22
                                from: 10.0
                                to: 100.0
                                value: videoComposer.threshold
                                stepSize: 5.0
                                onValueChanged: {
                                    videoComposer.setThreshold(value)
                                }
                            }
                        }
                        
                        // 背景窗口大小 (左下第二行)
                        Column {
                            Layout.fillWidth: true
                            Text { 
                                text: "背景窗口: " + bgWindowSlider.value.toFixed(0)
                                font.pointSize: 9 
                                color: "#333"
                            }
                            Slider {
                                id: bgWindowSlider
                                Layout.fillWidth: true
                                Layout.preferredHeight: 22
                                from: 5
                                to: 50
                                value: videoComposer.backgroundWindowSize
                                stepSize: 5
                                onValueChanged: {
                                    videoComposer.setBackgroundWindowSize(Math.round(value))
                                }
                            }
                        }
                        
                        // 開關控制 (中下第二行)
                        Column {
                            Layout.fillWidth: true
                            spacing: 3
                            
                            Text {
                                text: "算法選項"
                                font.pointSize: 9
                                color: "#333"
                            }
                            
                            // 自適應閾值開關
                            Row {
                                spacing: 6
                                CheckBox {
                                    id: adaptiveThresholdCheckBox
                                    checked: videoComposer.adaptiveThreshold
                                    onCheckedChanged: {
                                        videoComposer.setAdaptiveThreshold(checked)
                                    }
                                }
                                Text {
                                    text: "自適應閾值"
                                    font.pointSize: 7
                                    color: "#333"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                        }
                        
                        // 增強選項 (右下第二行)
                        Column {
                            Layout.fillWidth: true
                            spacing: 3
                            
                            Text {
                                text: "增強選項"
                                font.pointSize: 9
                                color: "#333"
                            }
                            
                            // 邊緣增強開關
                            Row {
                                spacing: 6
                                CheckBox {
                                    id: edgeEnhanceCheckBox
                                    checked: videoComposer.edgeEnhancement
                                    onCheckedChanged: {
                                        videoComposer.setEdgeEnhancement(checked)
                                    }
                                }
                                Text {
                                    text: "邊緣增強"
                                    font.pointSize: 7
                                    color: "#333"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                            
                            // 中心加權開關
                            Row {
                                spacing: 6
                                CheckBox {
                                    id: centerWeightCheckBox
                                    checked: videoComposer.centerWeighting
                                    onCheckedChanged: {
                                        videoComposer.setCenterWeighting(checked)
                                    }
                                }
                                Text {
                                    text: "中心加權"
                                    font.pointSize: 7
                                    color: "#333"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                        }
                    }
                }
                
                // 操作按鈕
                RowLayout {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 38
                    spacing: 10
                    visible: !compositePanel.collapsed
                    
                    Button {
                        text: "載入影片"
                        Layout.preferredWidth: 85
                        Layout.preferredHeight: 32
                        font.pointSize: 10
                        onClicked: {
                            videoListPopup.open()
                        }
                        
                        ToolTip.visible: hovered
                        ToolTip.text: "從檔案系統載入影片"
                    }
                    
                    // 預設模式選擇
                    ComboBox {
                        id: presetComboBox
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 32
                        font.pointSize: 9
                        
                        model: ["標準模式", "高質量", "快速模式", "精細模式"]
                        
                        onActivated: function(index) {
                            switch(index) {
                                case 0: // 標準模式
                                    videoComposer.setForegroundWeight(1.5)
                                    videoComposer.setBackgroundWeight(0.8)
                                    videoComposer.setBlurSize(5)
                                    videoComposer.setThreshold(30.0)
                                    videoComposer.setBackgroundWindowSize(15)
                                    videoComposer.setAdaptiveThreshold(true)
                                    videoComposer.setEdgeEnhancement(true)
                                    videoComposer.setCenterWeighting(false)
                                    break
                                case 1: // 高質量
                                    videoComposer.setForegroundWeight(2.0)
                                    videoComposer.setBackgroundWeight(0.6)
                                    videoComposer.setBlurSize(7)
                                    videoComposer.setThreshold(25.0)
                                    videoComposer.setBackgroundWindowSize(25)
                                    videoComposer.setAdaptiveThreshold(true)
                                    videoComposer.setEdgeEnhancement(true)
                                    videoComposer.setCenterWeighting(true)
                                    break
                                case 2: // 快速模式
                                    videoComposer.setForegroundWeight(1.2)
                                    videoComposer.setBackgroundWeight(1.0)
                                    videoComposer.setBlurSize(3)
                                    videoComposer.setThreshold(40.0)
                                    videoComposer.setBackgroundWindowSize(10)
                                    videoComposer.setAdaptiveThreshold(false)
                                    videoComposer.setEdgeEnhancement(false)
                                    videoComposer.setCenterWeighting(false)
                                    break
                                case 3: // 精細模式
                                    videoComposer.setForegroundWeight(1.8)
                                    videoComposer.setBackgroundWeight(0.7)
                                    videoComposer.setBlurSize(9)
                                    videoComposer.setThreshold(20.0)
                                    videoComposer.setBackgroundWindowSize(30)
                                    videoComposer.setAdaptiveThreshold(true)
                                    videoComposer.setEdgeEnhancement(true)
                                    videoComposer.setCenterWeighting(true)
                                    break
                            }
                        }
                        
                        ToolTip.visible: hovered
                        ToolTip.text: "選擇預設的合成模式"
                    }
                    
                    Button {
                        id: composeButton
                        text: {
                            if (compositionInProgress) {
                                return "合成中..."
                            } else if (exportInProgress) {
                                return "導出中..."
                            } else {
                                return "合成並導出"
                            }
                        }
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 32
                        font.pointSize: 9
                        enabled: videoComposer.selectedVideoCount >= 2 && !compositionInProgress && !exportInProgress
                        onClicked: {
                            videoComposer.composeSelectedVideos()
                        }
                        
                        ToolTip.visible: hovered
                        ToolTip.text: {
                            if (compositionInProgress) {
                                return "合成進行中: " + compositionCurrentFrame + "/" + compositionTotalFrames + " 幀"
                            } else if (exportInProgress) {
                                return "導出進行中: " + exportCurrentFrame + "/" + exportTotalFrames + " 幀"
                            } else if (enabled) {
                                return "合成選中的影片並自動導出到播放器"
                            } else {
                                return "需要選擇至少2個影片"
                            }
                        }
                        
                        // 動畫效果
                        Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                            border.color: composeButton.enabled ? "#4CAF50" : "transparent"
                            border.width: composeButton.enabled ? 2 : 0
                            radius: 6
                            visible: composeButton.enabled
                            
                            SequentialAnimation on opacity {
                                running: composeButton.enabled
                                loops: Animation.Infinite
                                NumberAnimation { to: 0.3; duration: 1000 }
                                NumberAnimation { to: 1.0; duration: 1000 }
                            }
                        }
                    }
                    
                    // 導出狀態指示 (自動導出)
                    Rectangle {
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 32
                        color: exportInProgress ? "#FFF3E0" : "transparent"
                        border.color: exportInProgress ? "#FF9800" : "#DDD"
                        border.width: 1
                        radius: 4
                        visible: exportInProgress || videoComposer.hasComposedFrames
                        
                        Text {
                            anchors.centerIn: parent
                            text: {
                                if (exportInProgress) {
                                    return "自動導出中..."
                                } else {
                                    return "已完成導出"
                                }
                            }
                            font.pointSize: 9
                            color: exportInProgress ? "#E65100" : "#4CAF50"
                        }
                        
                        // 導出進行中的動畫
                        SequentialAnimation on opacity {
                            running: exportInProgress
                            loops: Animation.Infinite
                            NumberAnimation { to: 0.5; duration: 800 }
                            NumberAnimation { to: 1.0; duration: 800 }
                        }
                    }
                    
                    Button {
                        text: "清空列表"
                        Layout.preferredWidth: 75
                        Layout.preferredHeight: 32
                        font.pointSize: 10
                        enabled: videoComposer.videoCount > 0
                        onClicked: {
                            videoComposer.clearVideos()
                        }
                        
                        ToolTip.visible: hovered
                        ToolTip.text: "清空所有影片"
                    }
                    
                    Item { Layout.fillWidth: true }
                    
                    // 狀態指示器
                    Rectangle {
                        Layout.preferredWidth: 160
                        Layout.preferredHeight: 28
                        color: videoComposer.selectedVideoCount >= 2 ? "#E8F5E8" : "#FFF3E0"
                        border.color: videoComposer.selectedVideoCount >= 2 ? "#4CAF50" : "#FF9800"
                        border.width: 1
                        radius: 14
                        
                        RowLayout {
                            anchors.centerIn: parent
                            spacing: 6
                            
                            Text {
                                text: videoComposer.selectedVideoCount >= 2 ? "OK" : "!!"
                                font.pointSize: 12
                                font.bold: true
                            }
                            
                            Text {
                                text: "已選: " + videoComposer.selectedVideoCount + "/" + videoComposer.videoCount
                                color: videoComposer.selectedVideoCount >= 2 ? "#2E7D32" : "#F57C00"
                                font.pointSize: 9
                                font.bold: true
                            }
                        }
                    }
                }
            }
        }
        
        // 錯誤訊息
        Text {
            id: errorText
            text: ""
            color: "red"
            visible: false
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            font.pointSize: 12
        }
    }
    
    Timer {
        id: errorTimer
        interval: 5000
        onTriggered: errorText.visible = false
    }
    
    // 影片選擇彈窗
    Popup {
        id: videoListPopup
        width: 400
        height: 300
        anchors.centerIn: parent
        modal: true
        
        Rectangle {
            anchors.fill: parent
            color: "#f0f0f0"
            border.color: "#d0d0d0"
            border.width: 1
            radius: 5
            
            Column {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10
                
                Text {
                    text: "選擇影片檔案"
                    font.pixelSize: 16
                    font.bold: true
                }
                
                ScrollView {
                    width: parent.width
                    height: parent.height - 100
                    
                    ListView {
                        id: videoListView
                        model: appController.videoListModel

                        delegate: ItemDelegate {
                            width: videoListView.width
                            height: 40

                            Rectangle {
                                anchors.fill: parent
                                color: parent.hovered ? "#e0e0e0" : "transparent"
                                border.color: "#d0d0d0"
                                border.width: 1
                                radius: 3

                                Column {
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    spacing: 2

                                    Text {
                                        text: (typeof displayName !== 'undefined' && displayName !== "") ? displayName
                                              : ((typeof fileName !== 'undefined' && fileName !== "") ? fileName
                                                 : (filePath ? filePath.split('/').pop() : "(無檔名)"))
                                        font.pixelSize: 12
                                        elide: Text.ElideRight
                                    }

                                    Text {
                                        text: info
                                        font.pixelSize: 10
                                        color: "#666"
                                        elide: Text.ElideRight
                                    }
                                }
                            }

                            onClicked: {
                                console.log("Selected video:", filePath)
                                statusText.text = "正在載入影片: " + fileName
                                statusText.color = "blue"
                                videoPlayer.loadVideo(filePath)

                                // 同時加入到合成器（保留原有流程，AppController 可在後端處理更高階邏輯）
                                var selectedFiles = [filePath]
                                videoComposer.loadVideos(selectedFiles)

                                videoListPopup.close()
                            }
                        }
                    }
                }
                
                Row {
                    anchors.right: parent.right
                    spacing: 10
                    
                    Button {
                        text: "重新整理"
                        onClicked: refreshVideoList()
                    }
                    
                    Button {
                        text: "取消"
                        onClicked: videoListPopup.close()
                    }
                }
            }
        }
        
        onOpened: {
            // Delegate refresh to the C++ AppController
            appController.refreshVideoList()
        }
    }
    
    // Delegate video list refresh to C++ AppController; keep a small wrapper for compatibility
    function refreshVideoList() {
        console.log("Requesting video list refresh via appController")
        if (appController && appController.refreshVideoList) {
            appController.refreshVideoList()
        }
    }
    
    // 時間格式化函數
    function formatTime(milliseconds) {
        var seconds = Math.floor(milliseconds / 1000)
        var minutes = Math.floor(seconds / 60)
        seconds = seconds % 60
        return minutes.toString().padStart(2, '0') + ':' + seconds.toString().padStart(2, '0')
    }
}
