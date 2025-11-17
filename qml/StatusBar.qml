import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    Layout.fillWidth: true
    height: 28
    color: "transparent"

    property var videoPlayer
    property var appState

    Text {
        id: statusText
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 6
        text: videoPlayer && videoPlayer.currentFile ? "Playing: " + videoPlayer.currentFile.split('/').pop() : "Ready"
        color: "#333"
    }
}
