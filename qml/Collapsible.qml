import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    // allow consumer children to be placed into the internal content wrapper
    default property alias content: contentWrapper.data
    property alias title: headerText.text
    property bool collapsed: false
    signal toggled(bool collapsed)

    Layout.fillWidth: true

    Rectangle {
        id: frame
        anchors.fill: parent
        color: "transparent"
    }

    ColumnLayout {
        id: col
        Layout.fillWidth: true
        spacing: 4

        RowLayout {
            id: headerRow
            Layout.fillWidth: true
            spacing: 8
            Rectangle { width: 0; height: 0; visible: false }
            Text { id: headerText; text: "Section"; font.bold: true }
            Item { Layout.fillWidth: true }
            ToolButton {
                id: toggleBtn
                text: root.collapsed ? "+" : "–"
                onClicked: {
                    root.collapsed = !root.collapsed
                    toggled(root.collapsed)
                }
            }
        }

        // content wrapper is a Layout-aware container; consumer children are inserted here via the default property alias
        ColumnLayout {
            id: contentWrapper
            Layout.fillWidth: true
            visible: !root.collapsed
            spacing: 4
        }
    }

    // compute implicitHeight so layouts behave
    implicitHeight: headerRow.implicitHeight + (root.collapsed ? 0 : contentWrapper.implicitHeight) + col.spacing
}
