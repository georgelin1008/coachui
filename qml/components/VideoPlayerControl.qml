import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#e0e0e0"
    radius: 5
    border.color: "#c7c7c7"
    border.width: 1
    
    property var videoDisplayArea: null

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

            Rectangle {
                width: 1
                height: 24
                color: "#b0b0b0"
                Layout.alignment: Qt.AlignVCenter
            }

            // 繪圖工具
            Button {
                text: videoDisplayArea && videoDisplayArea.drawingEnabled ? "✏️ 關閉繪圖" : "✏️ 繪圖"
                Layout.preferredWidth: 100
                onClicked: {
                    if (videoDisplayArea) {
                        videoDisplayArea.drawingEnabled = !videoDisplayArea.drawingEnabled
                    }
                }
            }

            Button {
                text: "↶ 復原"
                Layout.preferredWidth: 70
                enabled: videoDisplayArea && videoDisplayArea.drawingEnabled
                onClicked: {
                    if (videoDisplayArea) {
                        videoDisplayArea.undoDrawing()
                    }
                }
            }

            Button {
                text: "🗑️ 清除"
                Layout.preferredWidth: 70
                enabled: videoDisplayArea && videoDisplayArea.drawingEnabled
                onClicked: {
                    if (videoDisplayArea) {
                        videoDisplayArea.clearDrawing()
                    }
                }
            }

            Button {
                text: "📷 快照"
                Layout.preferredWidth: 80
                onClicked: {
                    if (videoDisplayArea) {
                        videoDisplayArea.captureSnapshot()
                    }
                }
            }

            Button {
                text: "🖼️ 瀏覽快照"
                Layout.preferredWidth: 100
                onClicked: {
                    snapshotBrowser.open()
                }
            }

            Item { Layout.fillWidth: true }
        }

        ProgressSlider {
            Layout.fillWidth: true
            Layout.preferredHeight: 48
        }
    }
}
