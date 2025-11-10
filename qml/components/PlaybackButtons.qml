import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root
    spacing: 6

    Button {
        text: videoPlayer && videoPlayer.isPlaying ? "暫停" : "播放"
        Layout.preferredWidth: 64
    enabled: videoPlayer && (videoPlayer.currentFile || "") !== ""

        background: Rectangle {
            color: videoPlayer && videoPlayer.isPlaying ? "#d32f2f" : "#4caf50"
            radius: 4
        }

        onClicked: {
            if (!videoPlayer)
                return

            if (videoPlayer.isPlaying) {
                videoPlayer.pause()
            } else {
                videoPlayer.play()
            }
        }
    }

    Button {
        text: "停止"
        Layout.preferredWidth: 54
    enabled: videoPlayer && (videoPlayer.currentFile || "") !== ""
        onClicked: videoPlayer ? videoPlayer.stop() : null
    }

    Button {
        text: "<<"
        Layout.preferredWidth: 40
    enabled: videoPlayer && (videoPlayer.currentFile || "") !== ""
        onClicked: videoPlayer && videoPlayer.seekBackward ? videoPlayer.seekBackward() : null
    }

    Button {
        text: ">>"
        Layout.preferredWidth: 40
    enabled: videoPlayer && (videoPlayer.currentFile || "") !== ""
        onClicked: videoPlayer && videoPlayer.seekForward ? videoPlayer.seekForward() : null
    }
}
