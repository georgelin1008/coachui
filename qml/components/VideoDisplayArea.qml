import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Item {
    id: root
    property bool drawingEnabled: false
    property string placeholderText: "尚未載入影片"

    function ensureVideoSink() {
        console.log("VideoDisplayArea: ensureVideoSink called")
        console.log("  videoPlayer:", videoPlayer)
        console.log("  videoOutput.videoSink:", videoOutput.videoSink)
        
        if (!videoPlayer || !videoOutput.videoSink) {
            console.log("  -> Skipping: missing videoPlayer or videoSink")
            return
        }

        if (typeof videoPlayer.setVideoSink === "function") {
            videoPlayer.setVideoSink(videoOutput.videoSink)
            console.log("VideoDisplayArea: 已連結 videoSink")
            console.log("  currentFile:", videoPlayer.currentFile)

            if (videoPlayer.currentFile && videoPlayer.currentFile !== "" && typeof videoPlayer.play === "function") {
                console.log("  -> Calling videoPlayer.play()")
                videoPlayer.play()
            } else {
                console.log("  -> NOT calling play - currentFile missing or play not a function")
            }
        } else {
            console.log("  -> setVideoSink is not a function")
        }
    }

    Component.onCompleted: Qt.callLater(ensureVideoSink)

    Rectangle {
        anchors.fill: parent
        color: "#1a1a1a"
        border.color: "#333"
        border.width: 1

        VideoOutput {
            id: videoOutput
            anchors.fill: parent
            visible: !!(videoPlayer && videoPlayer.currentFile && videoPlayer.currentFile !== "") && !snapshotImage.visible

            Component.onCompleted: root.ensureVideoSink()

            Connections {
                target: videoOutput
                function onVideoSinkChanged() {
                    root.ensureVideoSink()
                }
            }
        }

        // 快照圖片顯示
        Image {
            id: snapshotImage
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            visible: source != ""
            
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    // 點擊關閉快照顯示
                    snapshotImage.source = ""
                }
            }
            
            // 關閉按鈕
            Rectangle {
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 10
                width: 40
                height: 40
                color: "#80000000"
                radius: 20
                visible: parent.visible
                
                Text {
                    anchors.centerIn: parent
                    text: "✕"
                    color: "white"
                    font.pixelSize: 24
                }
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        snapshotImage.source = ""
                    }
                }
            }
        }

        Column {
            anchors.centerIn: parent
            spacing: 8
            visible: !videoOutput.visible && !snapshotImage.visible

            Text {
                text: cameraController && cameraController.isPreviewActive ? "攝影機預覽已啟動" : placeholderText
                color: "#fafafa"
                font.pixelSize: 20
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                text: videoPlayer && videoPlayer.currentFile ? videoPlayer.currentFile.split('/').pop() : ""
                color: "#bdbdbd"
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                visible: !!(videoPlayer && videoPlayer.currentFile)
            }
        }

        // 繪圖圖層
        DrawingCanvas {
            id: drawingCanvas
            anchors.fill: parent
            enabled: root.drawingEnabled
            strokeColor: "#FF0000"
            strokeWidth: 3
            visible: !snapshotImage.visible
        }
    }
    
    // 公開方法
    function clearDrawing() {
        drawingCanvas.clear()
    }
    
    function undoDrawing() {
        drawingCanvas.undo()
    }
    
    function showSnapshot(imagePath) {
        snapshotImage.source = imagePath
    }
    
    function captureSnapshot() {
        var timestamp = new Date().getTime()
        var filename = "snapshot_" + timestamp + ".png"
        var savePath = "/home/mxpt2/coachui/photo/" + filename
        
        root.grabToImage(function(result) {
            if (result.saveToFile(savePath)) {
                console.log("快照已儲存:", savePath)
                if (typeof statusHandler !== 'undefined' && statusHandler) {
                    statusHandler.showInfo("快照已儲存: " + filename)
                }
            } else {
                console.error("快照儲存失敗")
                if (typeof statusHandler !== 'undefined' && statusHandler) {
                    statusHandler.showError("快照儲存失敗")
                }
            }
        })
    }
}
