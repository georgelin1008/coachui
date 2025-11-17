import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: popup
    width: 500
    height: 400
    modal: true
    anchors.centerIn: Overlay.overlay

    signal videoSelected(string filePath)
    
    Rectangle {
        anchors.fill: parent
        color: "#2d2d2d"
        border.color: "#404040"
        border.width: 1
        radius: 5
        
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10
            
            // 標題列
            RowLayout {
                Layout.fillWidth: true
                spacing: 10
                
                Text { 
                    text: "選擇 H264 影片"
                    font.pixelSize: 16
                    font.bold: true
                    color: "#ffffff"
                }
                
                Item { Layout.fillWidth: true }
                
                Button {
                    text: "✕"
                    font.pixelSize: 16
                    onClicked: popup.close()
                    background: Rectangle {
                        color: parent.pressed ? "#555555" : "#3a3a3a"
                        radius: 3
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
            
            // 影片列表
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                
                ListView {
                    id: videoListView
                    model: ListModel { id: videoListModel }
                    spacing: 2
                    
                    delegate: Rectangle {
                        width: parent.width
                        height: 50
                        color: mouseArea.containsMouse ? "#3a5a7a" : "#3a3a3a"
                        border.color: "#555555"
                        border.width: 1
                        radius: 3
                        
                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 8
                            spacing: 10
                            
                            Text {
                                text: model.name
                                color: "#ffffff"
                                font.pixelSize: 13
                                elide: Text.ElideMiddle
                                Layout.fillWidth: true
                            }
                            
                            Text {
                                text: "▶"
                                color: "#4a9eff"
                                font.pixelSize: 20
                            }
                        }
                        
                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            
                            onClicked: {
                                console.log("選擇影片:", model.path)
                                popup.videoSelected(model.path)
                            }
                        }
                    }
                }
            }
            
            // 底部資訊列
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 25
                color: "#252525"
                radius: 3
                
                Text {
                    id: countText
                    anchors.centerIn: parent
                    text: "共 0 個 H264 影片"
                    color: "#cccccc"
                    font.pixelSize: 11
                }
            }
        }
    }
    
    function refreshList() {
        videoListModel.clear()
        
        if (typeof videoPlayer === 'undefined' || !videoPlayer) {
            console.warn("VideoComposerListPopup: videoPlayer not available")
            return
        }
        
        var allFiles = videoPlayer.getVideoFiles("/home/mxpt2/Videos")
        var h264Count = 0
        
        for (var i = 0; i < allFiles.length; i++) {
            var filePath = allFiles[i]
            var fileName = filePath.split('/').pop()
            
            // 只顯示 .h264 檔案
            if (fileName.toLowerCase().endsWith('.h264')) {
                videoListModel.append({
                    name: fileName,
                    path: filePath
                })
                h264Count++
            }
        }
        
        countText.text = "共 " + h264Count + " 個 H264 影片"
    }
    
    Component.onCompleted: {
        refreshList()
    }
    
    onOpened: {
        refreshList()
    }
}
