import QtQuick
import QtQuick.Controls

Item {
    id: root
    property bool enabled: false
    property color strokeColor: "#FF0000"
    property real strokeWidth: 3

    // Minimal API: clear() and toggle
    function clear() { /* TODO: implement strokes storage and redraw */ }

    MouseArea {
        anchors.fill: parent
        enabled: root.enabled
        onPressed: {
            // placeholder for starting a stroke
        }
        onPositionChanged: {
            // placeholder for drawing
        }
        onReleased: {
            // placeholder
        }
    }
}
