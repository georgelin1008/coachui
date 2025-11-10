import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: popup
    width: 480
    height: 320
    modal: true
    anchors.centerIn: Overlay.overlay

    // Signal emitted when a video is selected
    signal videoSelected(string filePath)

    Rectangle {
        anchors.fill: parent
        color: "#f0f0f0"
        border.color: "#d0d0d0"
        border.width: 1
        radius: 5
        
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10
            
            Text { 
                text: "選擇影片檔案"
                font.pixelSize: 16
                font.bold: true 
            }
            
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                
                ListView {
                    id: videoListView
                    // Access global appController directly
                    model: (typeof appController !== 'undefined' && appController) ? appController.videoListModel : null
                    
                    delegate: ItemDelegate {
                        width: videoListView.width
                        height: 40

                        Rectangle {
                            anchors.fill: parent
                            color: parent.hovered ? "#e0e0e0" : "transparent"
                            border.color: "#d0d0d0"
                            border.width: 1
                            radius: 3

                            Column {
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 2

                                Text {
                                    text: (typeof displayName !== 'undefined' && displayName !== "") ? displayName
                                          : ((typeof fileName !== 'undefined' && fileName !== "") ? fileName
                                             : (filePath ? filePath.split('/').pop() : "(無檔名)"))
                                    font.pixelSize: 12
                                    elide: Text.ElideRight
                                }

                                Text {
                                    text: info || ""
                                    font.pixelSize: 10
                                    color: "#666"
                                    elide: Text.ElideRight
                                }
                            }
                        }

                        onClicked: {
                            console.log("Selected video:", filePath)
                            
                            // Load video into player
                            if (typeof videoPlayer !== 'undefined' && videoPlayer && 
                                typeof videoPlayer.loadVideo === 'function') {
                                videoPlayer.loadVideo(filePath)
                            }

                            // Also load into composer (preserve original workflow)
                            if (typeof videoComposer !== 'undefined' && videoComposer && 
                                typeof videoComposer.loadVideos === 'function') {
                                var selectedFiles = [filePath]
                                videoComposer.loadVideos(selectedFiles)
                            }

                            popup.close()
                            popup.videoSelected(filePath)
                        }
                    }
                }
            }
            
            RowLayout {
                Layout.fillWidth: true
                spacing: 10
                
                Button {
                    text: "重新整理"
                    onClicked: {
                        console.log("Refreshing video list...")
                        if (typeof appController !== 'undefined' && appController && 
                            typeof appController.refreshVideoList === 'function') {
                            appController.refreshVideoList()
                        }
                    }
                }
                
                Item { Layout.fillWidth: true }
                
                Button {
                    text: "取消"
                    onClicked: popup.close()
                }
            }
        }
    }

    // When popup opens, refresh the video list
    onOpened: {
        console.log("📂 VideoListPopup opened, refreshing video list...")
        if (typeof appController !== 'undefined' && appController && 
            typeof appController.refreshVideoList === 'function') {
            console.log("   Calling appController.refreshVideoList()")
            appController.refreshVideoList()
        } else {
            console.log("   ERROR: appController not available or refreshVideoList not found")
            console.log("   typeof appController:", typeof appController)
            console.log("   appController:", appController)
        }
    }
}
