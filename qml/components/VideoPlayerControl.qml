import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#e0e0e0"
    radius: 5
    border.color: "#c7c7c7"
    border.width: 1

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            PlaybackButtons {
            }

            Rectangle {
                width: 1
                height: 24
                color: "#b0b0b0"
                Layout.alignment: Qt.AlignVCenter
            }

            SpeedControl {
            }

            Item { Layout.fillWidth: true }
        }

        ProgressSlider {
            Layout.fillWidth: true
            Layout.preferredHeight: 48
        }
    }
}
