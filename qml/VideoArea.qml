import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Item {
    id: root
    property var videoPlayer
    property var videoComposer
    property var cameraController
    property bool drawingEnabled: false
    // App window reference (passed from main.qml) so we can map coordinates to window space for diagnostics
    property var appWindow

    Rectangle {
        anchors.fill: parent
        color: "#1a1a1a"
        border.color: "#333"
        border.width: 1

        VideoOutput {
            id: videoOutput
            anchors.fill: parent
            // Ensure visible is always a boolean. Guard against undefined videoPlayer/currentFile.
            visible: !!(videoPlayer && videoPlayer.currentFile && videoPlayer.currentFile !== "")
            Component.onCompleted: {
                // protect against undefined API
                if (videoPlayer && videoPlayer.setVideoSink && videoOutput && videoOutput.videoSink) {
                    videoPlayer.setVideoSink(videoOutput.videoSink)
                    console.log("Connected videoPlayer to videoOutput.videoSink")
                }
            }
        }

        // Drawing overlay placeholder
        Text {
            anchors.centerIn: parent
            color: "white"
            text: videoPlayer && videoPlayer.currentFile ? "Playing: " + videoPlayer.currentFile.split('/').pop() : "No video loaded"
        }

        // Diagnostic MouseArea: logs local coordinates
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onPressed: function(mouse) {
                var localX = mouse.x
                var localY = mouse.y
                console.log("VideoArea clicked - local:", localX, localY)
            }
            onPositionChanged: function(mouse) {
                // useful when debugging hover/mouse tracking
                // uncomment if noisy: console.log("VideoArea mouse:", mouse.x, mouse.y)
            }
        }
    }
}
