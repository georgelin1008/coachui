import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root
    spacing: 10

    Text {
        text: "曝光:"
        Layout.alignment: Qt.AlignVCenter
    }

    Slider {
        id: exposureSlider
        Layout.preferredWidth: 200
        from: 50
        to: {
            if (cameraController && cameraController.frameRate > 0) {
                var maxVal = Math.floor(1000000 / cameraController.frameRate)
                return Math.floor(maxVal / 50) * 50
            }
            return 20000
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
        text: {
            if (cameraController) {
                return (cameraController.exposureValue / 1000).toFixed(2) + "ms"
            }
            return "0.00ms"
        }
        Layout.preferredWidth: 55
        Layout.alignment: Qt.AlignVCenter
        font.family: "monospace"
    }

    Button {
        text: "自動"
        Layout.preferredWidth: 45
        font.pixelSize: 9
        onClicked: {
            if (cameraController) {
                cameraController.setExposureValue(4000)
            }
        }
    }

    Text {
        text: "預設:"
        Layout.alignment: Qt.AlignVCenter
        font.pixelSize: 9
    }

    Button {
        text: "低"
        Layout.preferredWidth: 25
        Layout.preferredHeight: 25
        font.pixelSize: 8
        onClicked: {
            if (cameraController) {
                cameraController.setExposureValue(1000)
            }
        }
    }

    Button {
        text: "中"
        Layout.preferredWidth: 25
        Layout.preferredHeight: 25
        font.pixelSize: 8
        onClicked: {
            if (cameraController) {
                cameraController.setExposureValue(4000)
            }
        }
    }

    Button {
        text: "高"
        Layout.preferredWidth: 25
        Layout.preferredHeight: 25
        font.pixelSize: 8
        onClicked: {
            if (cameraController) {
                cameraController.setExposureValue(8000)
            }
        }
    }

    Item { Layout.fillWidth: true }
}
