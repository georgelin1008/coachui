import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: parent ? parent.width : 1200
    height: parent ? parent.height : 180

    signal previewToggled(bool active)
    signal recordingToggled(bool active)
    signal openVideoListRequested()

    Component.onCompleted: {
        console.log("✅ ControlsPanel loaded - accessing global C++ objects")
        console.log("   cameraController:", typeof cameraController)
        console.log("   recorder:", typeof recorder)
        console.log("   appController:", typeof appController)
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

            // ===== CAMERA TILE =====
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
                            if (typeof cameraController.setCurrentCamera === 'function') {
                                cameraController.setCurrentCamera(sel)
                            }
                        }
                    }

                    Button {
                        id: previewBtn
                        width: parent.width - 16
                        text: cameraController && cameraController.isPreviewActive ? "停止預覽" : "開始預覽"
                        onClicked: {
                            if (!cameraController) {
                                console.log("ERROR: cameraController is null")
                                return
                            }
                            if (cameraController.isPreviewActive) {
                                console.log("Calling stopPreview()")
                                if (typeof cameraController.stopPreview === 'function') {
                                    cameraController.stopPreview()
                                }
                            } else {
                                console.log("Calling startPreview()")
                                if (typeof cameraController.startPreview === 'function') {
                                    cameraController.startPreview()
                                }
                            }
                        }
                    }

                    Button {
                        width: parent.width - 16
                        text: "Snapshot"
                        onClicked: {
                            console.log("Snapshot: not yet implemented")
                        }
                    }
                }
            }

            // ===== RECORDING TILE =====
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
                            if (!recorder) {
                                console.log("ERROR: recorder is null")
                                return
                            }
                            if (!recordBtn.recording) {
                                console.log("Calling startRecording()")
                                var outPath = "/home/mxpt2/Videos/rec_" + Date.now() + ".h264"
                                if (typeof recorder.startRecording === 'function') {
                                    recorder.startRecording(outPath)
                                }
                                recordBtn.recording = true
                            } else {
                                console.log("Calling stopRecording()")
                                if (typeof recorder.stopRecording === 'function') {
                                    recorder.stopRecording()
                                }
                                recordBtn.recording = false
                            }
                        }
                    }

                    Button {
                        width: parent.width - 16
                        text: "Stop"
                        onClicked: {
                            if (recorder && typeof recorder.stop === 'function') {
                                recorder.stop()
                            }
                        }
                    }
                }
            }

            // ===== VIDEO TILE =====
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
                        text: "Load"
                        onClicked: {
                            console.log("Video: Load (not implemented)")
                        }
                    }

                    Button {
                        width: parent.width - 16
                        text: "List"
                        onClicked: {
                            console.log("Video: Opening list")
                            root.openVideoListRequested()
                        }
                    }

                    Button {
                        width: parent.width - 16
                        text: "Play"
                        onClicked: {
                            if (videoPlayer && typeof videoPlayer.play === 'function') {
                                videoPlayer.play()
                            }
                        }
                    }
                }
            }

            // ===== COMPOSER TILE =====
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
                        text: "Compose"
                        onClicked: {
                            console.log("Composer: Compose (not implemented)")
                        }
                    }

                    Button {
                        width: parent.width - 16
                        text: "Export"
                        onClicked: {
                            console.log("Composer: Export (not implemented)")
                        }
                    }
                }
            }

            // ===== TOOLS TILE =====
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
                        text: "Draw"
                        onClicked: {
                            console.log("Tools: Draw (not implemented)")
                        }
                    }

                    Button {
                        width: parent.width - 16
                        text: "Help"
                        onClicked: {
                            console.log("Help")
                        }
                    }
                }
            }

            // ===== APP TILE =====
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
                        text: "Settings"
                        onClicked: {
                            console.log("Settings (not implemented)")
                        }
                    }

                    Button {
                        width: parent.width - 16
                        text: "Refresh"
                        onClicked: {
                            if (appController && typeof appController.refreshVideoList === 'function') {
                                appController.refreshVideoList()
                            }
                        }
                    }
                }
            }
        }
    }
}
