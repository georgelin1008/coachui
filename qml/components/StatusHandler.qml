import QtQuick

Item {
    id: root

    property string currentStatus: "就緒"
    property string currentColor: "#333333"

    function showError(message) {
        root.currentStatus = message
        root.currentColor = "#e53935"
        errorTimer.restart()
    }

    function showSuccess(message) {
        root.currentStatus = message
        root.currentColor = "#43a047"
        errorTimer.stop()
    }

    function showInfo(message) {
        root.currentStatus = message
        root.currentColor = "#1e88e5"
        errorTimer.stop()
    }

    function showWarning(message) {
        root.currentStatus = message
        root.currentColor = "#fb8c00"
        errorTimer.restart()
    }

    Timer {
        id: errorTimer
        interval: 5000
        repeat: false
        onTriggered: {
            root.currentStatus = "就緒"
            root.currentColor = "#333333"
        }
    }
}
