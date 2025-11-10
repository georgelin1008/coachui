import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#f5f5f5"
    border.color: "#d0d0d0"
    border.width: 1
    radius: 5

    property bool userDragging: false
    property real pendingValue: 0
    property bool wasPlayingBeforeDrag: false

    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 10

        Text {
            text: "進度:"
            Layout.alignment: Qt.AlignVCenter
        }

        Slider {
            id: progressSlider
            Layout.fillWidth: true
            from: 0
            to: videoPlayer && typeof videoPlayer.duration === "number" ? Math.max(videoPlayer.duration, 0) : 0
            value: videoPlayer && typeof videoPlayer.position === "number" ? Math.max(videoPlayer.position, 0) : 0
            enabled: videoPlayer && (videoPlayer.currentFile || "") !== ""

            Timer {
                id: seekTimer
                interval: 16
                repeat: true
                running: false
                onTriggered: {
                    if (root.userDragging && videoPlayer) {
                        if (videoPlayer.setPreviewPosition) {
                            videoPlayer.setPreviewPosition(root.pendingValue)
                        } else {
                            videoPlayer.setPosition(root.pendingValue)
                        }
                    }
                }
            }

            onPressedChanged: {
                if (!videoPlayer)
                    return

                if (pressed) {
                    root.userDragging = true
                    root.pendingValue = value
                    root.wasPlayingBeforeDrag = videoPlayer.isPlaying
                    if (videoPlayer.isPlaying)
                        videoPlayer.pause()
                    seekTimer.start()
                } else {
                    root.userDragging = false
                    seekTimer.stop()
                    videoPlayer.setPosition(value)
                    if (root.wasPlayingBeforeDrag)
                        videoPlayer.play()
                }
            }

            onValueChanged: {
                if (root.userDragging) {
                    root.pendingValue = value
                }
            }
        }

        Text {
            text: {
                if (!videoPlayer)
                    return "00:00 / 00:00"
                var pos = typeof videoPlayer.position === "number" ? videoPlayer.position : 0
                var dur = typeof videoPlayer.duration === "number" ? videoPlayer.duration : 0
                return formatTime(pos) + " / " + formatTime(dur)
            }
            font.family: "monospace"
            font.pixelSize: 10
            Layout.preferredWidth: 120
            Layout.alignment: Qt.AlignVCenter
        }
    }

    Connections {
        target: videoPlayer
        enabled: !!videoPlayer

        function onPositionChanged() {
            if (!root.userDragging)
                progressSlider.value = Math.max(videoPlayer.position, 0)
        }

        function onDurationChanged() {
            progressSlider.to = Math.max(videoPlayer.duration, 0)
        }
    }

    function formatTime(ms) {
        if (!ms || ms < 0)
            ms = 0
        var totalSeconds = Math.floor(ms / 1000)
        var minutes = Math.floor(totalSeconds / 60)
        var seconds = totalSeconds % 60
        return minutes.toString().padStart(2, "0") + ":" + seconds.toString().padStart(2, "0")
    }
}
