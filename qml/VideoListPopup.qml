import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: popup
    width: 480
    height: 320
    modal: true
    anchors.centerIn: Overlay.overlay

    // Signals
    signal videoSelected(string filePath)
    signal videosSelected(var filePaths)  // 多選模式
    
    // Properties
    property bool multiSelectMode: false
    property var selectedVideos: []

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
            
            // 標題和多選開關
            RowLayout {
                Layout.fillWidth: true
                spacing: 10
                
                Text { 
                    text: multiSelectMode ? "選擇影片檔案（多選）" : "選擇影片檔案"
                    font.pixelSize: 16
                    font.bold: true 
                }
                
                Item { Layout.fillWidth: true }
                
                Text {
                    text: "並排模式"
                    font.pixelSize: 12
                    visible: true
                }
                
                Switch {
                    id: multiSelectSwitch
                    checked: multiSelectMode
                    onCheckedChanged: {
                        multiSelectMode = checked
                        selectedVideos = []  // 清空已選
                    }
                }
                
                Text {
                    text: multiSelectMode ? "已選: " + selectedVideos.length : ""
                    font.pixelSize: 12
                    color: "#2196f3"
                    font.bold: true
                    visible: multiSelectMode
                }
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
                        
                        property bool isSelected: selectedVideos.indexOf(filePath) >= 0

                        Rectangle {
                            anchors.fill: parent
                            color: isSelected ? "#bbdefb" : (parent.hovered ? "#e0e0e0" : "transparent")
                            border.color: isSelected ? "#2196f3" : "#d0d0d0"
                            border.width: isSelected ? 2 : 1
                            radius: 3

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 8
                                spacing: 8
                                
                                // 多選模式的勾選框
                                CheckBox {
                                    visible: multiSelectMode
                                    checked: isSelected
                                    onClicked: {
                                        // 由 ItemDelegate.onClicked 處理
                                    }
                                }

                                Column {
                                    Layout.fillWidth: true
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
                        }

                        onClicked: {
                            if (multiSelectMode) {
                                // 多選模式：切換選擇狀態
                                var index = selectedVideos.indexOf(filePath)
                                var newSelection = selectedVideos.slice()  // 複製陣列
                                
                                if (index >= 0) {
                                    // 已選中，取消選擇
                                    newSelection.splice(index, 1)
                                } else {
                                    // 未選中，添加選擇（最多4個）
                                    if (newSelection.length < 4) {
                                        newSelection.push(filePath)
                                    } else {
                                        console.log("最多只能選擇 4 個影片")
                                    }
                                }
                                
                                selectedVideos = newSelection
                                console.log("已選影片:", selectedVideos)
                            } else {
                                // 單選模式：直接載入
                                console.log("Selected video:", filePath)
                                
                                if (typeof videoPlayer !== 'undefined' && videoPlayer && 
                                    typeof videoPlayer.loadVideo === 'function') {
                                    videoPlayer.loadVideo(filePath)
                                }

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
                
                // 多選模式的確認按鈕
                Button {
                    text: "確認並排播放 (" + selectedVideos.length + ")"
                    visible: multiSelectMode
                    enabled: selectedVideos.length >= 2
                    
                    background: Rectangle {
                        color: parent.enabled ? (parent.down ? "#1976d2" : (parent.hovered ? "#2196f3" : "#42a5f5")) : "#bdbdbd"
                        radius: 4
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    onClicked: {
                        if (selectedVideos.length >= 2) {
                            console.log("🎬 準備合成並排影片:", selectedVideos)
                            popup.videosSelected(selectedVideos)
                            popup.close()
                        }
                    }
                }
                
                Button {
                    text: "取消"
                    onClicked: {
                        selectedVideos = []
                        popup.close()
                    }
                }
            }
        }
    }

    // When popup opens, refresh the video list
    onOpened: {
        console.log("📂 VideoListPopup opened, refreshing video list...")
        selectedVideos = []  // 清空已選
        
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
