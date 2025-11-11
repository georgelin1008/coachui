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
    property bool wasPlayingBeforeDrag: false
    property real lastSeekTime: 0

    Timer {
        id: seekTimer
        interval: 50  // 20fps - 平衡流暢度和性能
        repeat: false
        onTriggered: {
            if (videoPlayer && root.userDragging) {
                videoPlayer.setPosition(progressSlider.value)
            }
        }
    }

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
            stepSize: 100  // 100ms 步進
            
            onPressedChanged: {
                if (!videoPlayer) return

                if (pressed) {
                    root.userDragging = true
                    root.wasPlayingBeforeDrag = videoPlayer.isPlaying
                    if (videoPlayer.isPlaying) {
                        videoPlayer.pause()
                    }
                    // 按下時立即 seek 一次
                    videoPlayer.setPosition(value)
                    root.lastSeekTime = Date.now()
                } else {
                    root.userDragging = false
                    seekTimer.stop()
                    // 放開時最終 seek
                    videoPlayer.setPosition(value)
                    if (root.wasPlayingBeforeDrag) {
                        videoPlayer.play()
                    }
                }
            }

            onMoved: {
                // 使用節流避免過於頻繁的 seek
                if (videoPlayer && root.userDragging) {
                    seekTimer.restart()
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
