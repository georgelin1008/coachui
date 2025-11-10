import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: parent ? parent.width : 1200
    height: parent ? parent.height : 180
    
    // Color background (optional)
    Rectangle {
        anchors.fill: parent
        color: "transparent"
    }

    signal previewToggled(bool active)
    signal recordingToggled(bool active)
    signal openVideoListRequested()
    
    Component.onCompleted: {
        console.log("ControlsPanel: Component loaded")
        console.log("  - Global cameraController available:", typeof cameraController)
        console.log("  - Global recorder available:", typeof recorder)
    }

    Flickable {
        id: tilesFlick
        anchors.fill: parent
        contentWidth: row.width + 24
        contentHeight: height
        clip: true

        Row {
            id: row
            x: 6
            y: 6
            spacing: 12

            // Camera Tile
            Rectangle {
                width: 150
                height: tilesFlick.height - 12
                color: "#f5f6f7"
                radius: 6
                border { color: "#ddd"; width: 1 }

                Column {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    Text {
                        text: "Camera"
                        font.bold: true
                        font.pixelSize: 13
                    }

                    ComboBox {
                        id: camCombo
                        width: parent.width - 16
                        model: cameraController && cameraController.availableCameras ? cameraController.availableCameras : []
                        onActivated: {
                            if (!cameraController) return
                            var sel = currentText
                            if (typeof cameraController.setCurrentCamera === 'function') cameraController.setCurrentCamera(sel)
                        }
                    }

                    Button {
                        id: previewBtn
                        width: parent.width - 16
                        text: {
                            if (cameraController && cameraController.isPreviewActive) {
                                return "停止預覽"
                            } else {
                                return "開始預覽"
                            }
                        }
                        onClicked: {
                            // Access global cameraController directly (from main.cpp context)
                            if (!cameraController) {
                                console.log("ERROR: cameraController is null")
                                return
                            }
                            console.log("Preview button clicked, isPreviewActive:", cameraController.isPreviewActive)
                            if (cameraController.isPreviewActive) {
                                console.log("Calling stopPreview()")
                                if (typeof cameraController.stopPreview === 'function') {
                                    cameraController.stopPreview()
                                } else {
                                    console.log("ERROR: stopPreview is not a function")
                                }
                                root.previewToggled(false)
                            } else {
                                console.log("Calling startPreview()")
                                if (typeof cameraController.startPreview === 'function') {
                                    cameraController.startPreview()
                                } else {
                                    console.log("ERROR: startPreview is not a function")
                                }
                                root.previewToggled(true)
                            }
                        }
                    }

                    Button {
                        width: parent.width - 16
                        text: "Snapshot"
                        onClicked: {
                            console.log("Snapshot button clicked")
                            if (cameraController && typeof cameraController.snapshot === 'function') {
                                cameraController.snapshot()
                            } else {
                                console.log("Snapshot not yet implemented")
                            }
                        }
                    }
                }
            }

            // Recording Tile
            Rectangle {
                width: 140
                height: tilesFlick.height - 12
                color: "#f5f6f7"
                radius: 6
                border { color: "#ddd"; width: 1 }

                Column {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    Text {
                        text: "Recording"
                        font.bold: true
                        font.pixelSize: 13
                    }

                    Button {
                        id: recordBtn
                        width: parent.width - 16
                        text: recordBtn.recording ? "停止錄製" : "開始錄製"
                        property bool recording: false
                        onClicked: {
                            // Access global recorder directly (from main.cpp context)
                            if (!recorder) {
                                console.log("ERROR: recorder is null")
                                return
                            }
                            console.log("Record button clicked, recording:", recordBtn.recording)
                            if (recordBtn.recording) {
                                console.log("Calling stopRecording()")
                                if (typeof recorder.stopRecording === 'function') {
                                    recorder.stopRecording()
                                }
                                recordBtn.recording = false
                                root.recordingToggled(false)
                            } else {
                                console.log("Calling startRecording()")
                                var outPath = "/home/mxpt2/Videos/rec_" + Date.now() + ".h264"
                                console.log("Recording to:", outPath)
                                if (typeof recorder.startRecording === 'function') {
                                    recorder.startRecording(outPath)
                                }
                                recordBtn.recording = true
                                root.recordingToggled(true)
                            }
                        }
                    }

                    Button {
                        width: parent.width - 16
                        text: "Stop"
                        onClicked: {
                            if (recorder && typeof recorder.stop === 'function') recorder.stop()
                        }
                    }
                }
            }

            // Video Tile
            Rectangle {
                width: 150
                height: tilesFlick.height - 12
                color: "#f5f6f7"
                radius: 6
                border { color: "#ddd"; width: 1 }

                Column {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    Text {
                        text: "Video"
                        font.bold: true
                        font.pixelSize: 13
                    }

                    Button {
                        width: parent.width - 16
                        text: "Load Video"
                        onClicked: {
                            if (appState && typeof appState.openFileDialog === 'function') appState.openFileDialog()
                        }
                    }

                    Button {
                        width: parent.width - 16
                        text: "List"
                        onClicked: {
                            root.openVideoListRequested()
                        }
                    }

                    Button {
                        width: parent.width - 16
                        text: "Play"
                        onClicked: {
                            if (typeof videoPlayer !== 'undefined' && videoPlayer.play) videoPlayer.play()
                        }
                    }
                }
            }

            // Composer Tile
            Rectangle {
                width: 160
                height: tilesFlick.height - 12
                color: "#f5f6f7"
                radius: 6
                border { color: "#ddd"; width: 1 }

                Column {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    Text {
                        text: "Composer"
                        font.bold: true
                        font.pixelSize: 13
                    }

                    Button {
                        width: parent.width - 16
                        text: "Load"
                        onClicked: {
                            console.log("Composer: Load (placeholder)")
                        }
                    }

                    Button {
                        width: parent.width - 16
                        text: "Start"
                        onClicked: {
                            if (typeof videoComposer !== 'undefined' && videoComposer.start) videoComposer.start()
                        }
                    }
                }
            }

            // Tools Tile
            Rectangle {
                width: 130
                height: tilesFlick.height - 12
                color: "#f5f6f7"
                radius: 6
                border { color: "#ddd"; width: 1 }

                Column {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    Text {
                        text: "Tools"
                        font.bold: true
                        font.pixelSize: 13
                    }

                    Button {
                        width: parent.width - 16
                        text: appState && appState.drawingEnabled ? "繪圖: 開" : "繪圖: 關"
                        font.pixelSize: 11
                        onClicked: {
                            if (appState) appState.drawingEnabled = !appState.drawingEnabled
                        }
                    }

                    Button {
                        width: parent.width - 16
                        text: "Export"
                        font.pixelSize: 11
                        onClicked: {
                            if (typeof videoPlayer !== 'undefined' && videoPlayer.exportFrame) videoPlayer.exportFrame()
                        }
                    }
                }
            }

            // App Tile
            Rectangle {
                width: 130
                height: tilesFlick.height - 12
                color: "#f5f6f7"
                radius: 6
                border { color: "#ddd"; width: 1 }

                Column {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    Text {
                        text: "App"
                        font.bold: true
                        font.pixelSize: 13
                    }

                    Button {
                        width: parent.width - 16
                        text: "Refresh"
                        font.pixelSize: 11
                        onClicked: {
                            if (appState && typeof appState.refresh === 'function') appState.refresh()
                        }
                    }

                    Button {
                        width: parent.width - 16
                        text: "Help"
                        font.pixelSize: 11
                        onClicked: {
                            console.log('Help')
                        }
                    }
                }
            }
        }
    }

    // Sync state with external properties
    Timer {
        interval: 500
        repeat: true
        running: true
        onTriggered: {
            if (cameraController && typeof cameraController.isPreviewActive !== 'undefined') {
                previewBtn.toggled = !!cameraController.isPreviewActive
            }
            if (recorder && typeof recorder.isRecording !== 'undefined') {
                recordBtn.recording = !!recorder.isRecording
            }
        }
    }

    }
    }
}

```
}
