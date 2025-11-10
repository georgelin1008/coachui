import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root
    spacing: 10

    signal recordingStarted()
    signal recordingStopped()
    signal testVideoRequested()
    signal videoListRequested()

    Text {
        text: "錄製:"
        Layout.alignment: Qt.AlignVCenter
    }

    ComboBox {
        id: frameRateCombo
        Layout.preferredWidth: 80
        model: ["30fps", "60fps", "90fps", "120fps"]
        currentIndex: 3

        onCurrentTextChanged: {
            if (!cameraController || !currentText)
                return

            var fps = parseInt(currentText.replace("fps", ""))
            if (!isNaN(fps)) {
                cameraController.setFrameRate(fps)
            }
        }
    }

    Button {
        text: "開始錄製"
        Layout.preferredWidth: 90
        onClicked: {
            if (!recorder)
                return

            if (cameraController && cameraController.isPreviewActive) {
                cameraController.stopPreview()
            }

            Qt.callLater(function() {
                var fps = cameraController ? cameraController.frameRate : 120
                var outputPath = "/home/mxpt2/Videos/rec_" + Date.now() + ".h264"
                recorder.startRecording(outputPath, fps)
                root.recordingStarted()
            })
        }
    }

    Button {
        text: "停止錄製"
        Layout.preferredWidth: 90
        onClicked: {
            if (!recorder)
                return

            recorder.stopRecording()
            root.recordingStopped()
        }
    }

    Button {
        text: "載入影片"
        Layout.preferredWidth: 90
        onClicked: root.videoListRequested()
    }

    Button {
        text: "測試播放"
        Layout.preferredWidth: 90
        onClicked: root.testVideoRequested()
    }

    Item { Layout.fillWidth: true }
}
