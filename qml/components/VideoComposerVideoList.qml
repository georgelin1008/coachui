import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 5
    
    property alias videoModel: loadedVideosModel
    signal addVideoClicked()
    signal clearVideosClicked()
    
    Text {
        text: "📹 已載入影片"
        font.pixelSize: 11
        font.bold: true
        color: "#ffffff"
    }
    
    ScrollView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true
        
        ListView {
            id: videoListView
            model: ListModel { id: loadedVideosModel }
            
            delegate: Rectangle {
                width: parent.width
                height: 30
                color: model.selected ? "#4a7a4a" : "#3a3a3a"
                border.color: "#555"
                border.width: 1
                radius: 3
                
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 5
                    
                    CheckBox {
                        id: checkbox
                        checked: model.selected
                        onCheckedChanged: {
                            loadedVideosModel.setProperty(index, "selected", checked)
                        }
                    }
                    
                    Text {
                        Layout.fillWidth: true
                        text: model.name
                        color: "#ffffff"
                        font.pixelSize: 10
                        elide: Text.ElideMiddle
                    }
                }
            }
        }
    }
    
    RowLayout {
        Layout.fillWidth: true
        spacing: 5
        
        Button {
            text: "+"
            Layout.preferredWidth: 40
            font.pixelSize: 14
            font.bold: true
            onClicked: root.addVideoClicked()
        }
        
        Button {
            text: "✕"
            Layout.preferredWidth: 40
            font.pixelSize: 12
            onClicked: root.clearVideosClicked()
        }
    }
}
