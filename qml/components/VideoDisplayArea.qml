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
            visible: !!(videoPlayer && videoPlayer.currentFile && videoPlayer.currentFile !== "")

            Component.onCompleted: root.ensureVideoSink()

            Connections {
                target: videoOutput
                function onVideoSinkChanged() {
                    root.ensureVideoSink()
                }
            }
        }

        Column {
            anchors.centerIn: parent
            spacing: 8
            visible: !videoOutput.visible

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

        MouseArea {
            anchors.fill: parent
            enabled: drawingEnabled
            cursorShape: drawingEnabled ? Qt.CrossCursor : Qt.ArrowCursor
        }
    }
}
