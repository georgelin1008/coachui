import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Rectangle {
    id: root
    implicitWidth: 120
    color: "#f5f5f5"
    border.color: "#d0d0d0"
    border.width: 1
    radius: 5

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        Button {
            text: "打關鍵幀"
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            enabled: videoPlayer && videoPlayer.currentFile !== ""
            font.bold: true
            font.pixelSize: 13

            background: Rectangle {
                color: parent.enabled ? (parent.down ? "#1976d2" : (parent.hovered ? "#2196f3" : "#42a5f5")) : "#bdbdbd"
                radius: 6
            }

            contentItem: Text {
                text: parent.text
                font: parent.font
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: {
                if (!videoPlayer || !keyframeManager)
                    return

                var timestamp = videoPlayer.position
                var defaultName = "關鍵幀 " + (keyframeManager.keyframeCount + 1)
                keyframeManager.addKeyframe(timestamp, defaultName, "")
                keyframeManager.saveKeyframes()
            }
        }

        Button {
            text: "管理 (" + (keyframeManager ? keyframeManager.keyframeCount : 0) + ")"
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            enabled: keyframeManager && keyframeManager.keyframeCount > 0
            font.pixelSize: 11

            background: Rectangle {
                color: parent.enabled ? (parent.down ? "#455a64" : (parent.hovered ? "#546e7a" : "#607d8b")) : "#e0e0e0"
                radius: 6
            }

            contentItem: Text {
                text: parent.text
                font: parent.font
                color: parent.enabled ? "white" : "#999"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: {
                keyframeListWindow.show()
                keyframeListWindow.raise()
                keyframeListWindow.requestActivate()
            }
        }

        Item { Layout.fillHeight: true }
    }

    // 懸浮視窗 - 關鍵幀列表
    Window {
        id: keyframeListWindow
        width: 400
        height: 500
        title: "關鍵幀管理"
        flags: Qt.Dialog
        modality: Qt.NonModal

        Rectangle {
            anchors.fill: parent
            color: "#f5f5f5"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 8

                // 標題
                Text {
                    text: "關鍵幀列表 (" + (keyframeManager ? keyframeManager.keyframeCount : 0) + ")"
                    font.pixelSize: 16
                    font.bold: true
                    color: "#333"
                }

                // 關鍵幀列表
                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    ListView {
                        id: keyframeList
                        model: keyframeManager ? keyframeManager.getKeyframes() : []
                        spacing: 6

                        delegate: Rectangle {
                            required property var modelData
                            width: keyframeList.width
                            height: 60
                            color: mouseArea.containsMouse ? "#e8e8e8" : "#ffffff"
                            border.color: "#d0d0d0"
                            border.width: 1
                            radius: 4

                            MouseArea {
                                id: mouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                acceptedButtons: Qt.LeftButton
                                
                                onClicked: function(mouse) {
                                    // 檢查是否點擊在按鈕區域
                                    var buttonArea = parent.width - 120 // 兩個按鈕加間距
                                    if (mouse.x < buttonArea) {
                                        // 點擊在文字區域，跳轉
                                        if (videoPlayer) {
                                            videoPlayer.setPosition(modelData.timestamp)
                                            console.log("跳轉到:", modelData.timestamp)
                                        }
                                    }
                                }
                                
                                onDoubleClicked: function(mouse) {
                                    var buttonArea = parent.width - 120
                                    if (mouse.x < buttonArea) {
                                        keyframeEditDialog.keyframeIndex = modelData.index
                                        keyframeEditDialog.keyframeName = modelData.name
                                        keyframeEditDialog.keyframeDescription = modelData.description || ""
                                        keyframeEditDialog.open()
                                    }
                                }
                            }

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 8
                                spacing: 10

                                Column {
                                    Layout.fillWidth: true
                                    spacing: 3

                                    Text {
                                        text: modelData.name || "未命名"
                                        font.pixelSize: 13
                                        font.bold: true
                                        color: "#333"
                                    }

                                    Text {
                                        text: formatTime(modelData.timestamp)
                                        font.pixelSize: 11
                                        color: "#666"
                                    }

                                    Text {
                                        text: modelData.description || ""
                                        font.pixelSize: 10
                                        color: "#999"
                                        wrapMode: Text.WordWrap
                                        visible: text !== ""
                                    }
                                }

                                Button {
                                    text: "編輯"
                                    Layout.preferredWidth: 55
                                    Layout.preferredHeight: 32
                                    font.pixelSize: 10
                                    z: 1

                                    onClicked: {
                                        keyframeEditDialog.keyframeIndex = modelData.index
                                        keyframeEditDialog.keyframeName = modelData.name
                                        keyframeEditDialog.keyframeDescription = modelData.description || ""
                                        keyframeEditDialog.open()
                                    }
                                }

                                Button {
                                    text: "刪除"
                                    Layout.preferredWidth: 55
                                    Layout.preferredHeight: 32
                                    font.pixelSize: 10
                                    z: 1

                                    background: Rectangle {
                                        color: parent.down ? "#c62828" : (parent.hovered ? "#d32f2f" : "#e53935")
                                        radius: 3
                                    }

                                    contentItem: Text {
                                        text: parent.text
                                        font: parent.font
                                        color: "white"
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }

                                    onClicked: {
                                        if (keyframeManager)
                                            keyframeManager.removeKeyframe(modelData.index)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // 編輯對話框
    Window {
        id: keyframeEditDialog
        title: "編輯關鍵幀"
        width: 400
        height: 250
        flags: Qt.Dialog | Qt.WindowStaysOnTopHint
        modality: Qt.ApplicationModal

        property int keyframeIndex: -1
        property string keyframeName: ""
        property string keyframeDescription: ""

        function open() {
            show()
            raise()
            requestActivate()
        }

        Rectangle {
            anchors.fill: parent
            color: "#f5f5f5"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                Text {
                    text: "名稱:"
                    font.pixelSize: 12
                }

                TextField {
                    id: editNameField
                    Layout.fillWidth: true
                    text: keyframeEditDialog.keyframeName
                    placeholderText: "輸入關鍵幀名稱"
                    
                    Component.onCompleted: forceActiveFocus()
                }

                Text {
                    text: "說明:"
                    font.pixelSize: 12
                }

                TextArea {
                    id: editDescriptionField
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: keyframeEditDialog.keyframeDescription
                    placeholderText: "輸入說明（可選）"
                    wrapMode: TextArea.Wrap
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    Item { Layout.fillWidth: true }

                    Button {
                        text: "取消"
                        Layout.preferredWidth: 80
                        Layout.preferredHeight: 35

                        onClicked: {
                            keyframeEditDialog.close()
                        }
                    }

                    Button {
                        text: "確定"
                        Layout.preferredWidth: 80
                        Layout.preferredHeight: 35
                        
                        background: Rectangle {
                            color: parent.down ? "#1976d2" : (parent.hovered ? "#2196f3" : "#42a5f5")
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
                            if (keyframeManager && keyframeEditDialog.keyframeIndex >= 0) {
                                keyframeManager.updateKeyframe(
                                    keyframeEditDialog.keyframeIndex,
                                    editNameField.text || "未命名",
                                    editDescriptionField.text
                                )
                                keyframeManager.saveKeyframes()
                            }
                            keyframeEditDialog.close()
                        }
                    }
                }
            }
        }
    }

    // 格式化時間
    function formatTime(ms) {
        var totalSeconds = Math.floor(ms / 1000)
        var hours = Math.floor(totalSeconds / 3600)
        var minutes = Math.floor((totalSeconds % 3600) / 60)
        var seconds = totalSeconds % 60

        if (hours > 0) {
            return hours + ":" + 
                   (minutes < 10 ? "0" : "") + minutes + ":" + 
                   (seconds < 10 ? "0" : "") + seconds
        } else {
            return minutes + ":" + (seconds < 10 ? "0" : "") + seconds
        }
    }

    // 監聽影片變化
    Connections {
        target: videoPlayer
        function onCurrentFileChanged() {
            if (keyframeManager && videoPlayer.currentFile !== "") {
                keyframeManager.setCurrentVideo(videoPlayer.currentFile)
                console.log("✅ 載入關鍵幀:", videoPlayer.currentFile)
            }
        }
    }

    // 監聽關鍵幀變化，更新列表
    Connections {
        target: keyframeManager
        function onKeyframesChanged() {
            if (keyframeList)
                keyframeList.model = keyframeManager.getKeyframes()
        }
    }
}
