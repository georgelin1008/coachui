import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root
    spacing: 4

    Text {
        text: "速度:"
        font.pixelSize: 11
        Layout.alignment: Qt.AlignVCenter
    }

    function setRate(rate) {
        if (!videoPlayer)
            return
        videoPlayer.setPlaybackRate(rate)
    }

    Repeater {
        model: [0.25, 0.5, 1.0, 1.5, 2.0]
        delegate: Button {
            required property double modelData
            text: modelData.toFixed(modelData === 2 ? 0 : 2) + "x"
            Layout.preferredWidth: 36
            Layout.preferredHeight: 26
            font.pixelSize: 9
            font.bold: videoPlayer && typeof videoPlayer.playbackRate === "number" && Math.abs((videoPlayer.playbackRate || 1.0) - modelData) < 0.01
            enabled: videoPlayer && (videoPlayer.currentFile || "") !== ""
            onClicked: root.setRate(modelData)
        }
    }

    Text {
        text: {
            if (!videoPlayer)
                return "1.00x"
            var rate = videoPlayer.playbackRate
            if (typeof rate !== "number" || isNaN(rate))
                rate = 1.0
            return rate.toFixed(2) + "x"
        }
        font.pixelSize: 11
        Layout.alignment: Qt.AlignVCenter
        Layout.preferredWidth: 50
    }
}
