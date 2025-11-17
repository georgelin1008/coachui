import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    Layout.fillWidth: true
    color: "#e8f4fd"
    border.width: 1
    radius: 6

    property var videoComposer
    property var appState

    ColumnLayout {
        id: mainCol
        Layout.fillWidth: true
        Layout.fillHeight: true
        anchors.margins: 0
        spacing: 4

        RowLayout {
            spacing: 8
            Text { text: "Composer"; font.bold: true }
            Button { text: "載入影片"; onClicked: videoComposer ? videoComposer.openFileDialog() : undefined }
        }

        Text { text: videoComposer ? ("已載入影片: " + videoComposer.videoCount) : "未連接 videoComposer" }
    }

    // ensure the Rectangle reports a useful implicitHeight so parent layouts stack correctly
    implicitHeight: mainCol.implicitHeight + 4
}
