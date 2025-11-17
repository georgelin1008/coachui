import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#f0f0f0"
    border.color: "#d0d0d0"
    border.width: 1
    radius: 5

    property var statusHandler

    signal recordingStarted()
    signal recordingStopped()
    signal testVideoRequested()
    signal videoListRequested()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 8

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Text {
                text: "攝影機:"
                Layout.alignment: Qt.AlignVCenter
            }

            ComboBox {
                id: cameraCombo
                model: cameraController ? cameraController.availableCameras : []
                Layout.preferredWidth: 150

                onCurrentTextChanged: {
                    if (!cameraController || !currentText)
                        return

                    cameraController.setCurrentCamera(currentText)
                }
            }

            Button {
                text: cameraController && cameraController.isPreviewActive ? "停止預覽" : "開始預覽"
                Layout.preferredWidth: 100

                onClicked: {
                    if (!cameraController)
                        return

                    if (cameraController.isPreviewActive) {
                        cameraController.stopPreview()
                        statusHandler && statusHandler.showInfo("預覽已停止")
                    } else {
                        cameraController.startPreview()
                        statusHandler && statusHandler.showSuccess("預覽已啟動")
                    }
                }
            }

            Button {
                text: "重新整理"
                Layout.preferredWidth: 90
                onClicked: cameraController ? cameraController.refreshCameras() : null
            }

            Item { Layout.fillWidth: true }
        }

        ExposureControl {
            id: exposureControl
            Layout.fillWidth: true
        }

        RecordingControl {
            Layout.fillWidth: true

            onRecordingStarted: {
                statusHandler && statusHandler.showInfo("開始錄製…")
                root.recordingStarted()
            }

            onRecordingStopped: {
                statusHandler && statusHandler.showInfo("錄製已停止")
                root.recordingStopped()
            }

            onTestVideoRequested: root.testVideoRequested()
            onVideoListRequested: root.videoListRequested()
        }
    }
}
