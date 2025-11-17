import QtQml 2.0

QtObject {
    id: appState

    // Centralized application state used by main window and components.
    // Contract (public properties):
    //  - selectedVideo: string (full path)
    //  - drawingEnabled: bool
    //  - compositionInProgress: bool
    //  - exportInProgress: bool
    property string selectedVideo: ""
    property bool drawingEnabled: false
    property bool compositionInProgress: false
    property bool exportInProgress: false
    property int refreshCounter: 0

    // Signals for cross-component events
    signal videoSelected(string filePath)
    signal requestRefresh()

    function selectVideo(path) {
        if (!path) return
        selectedVideo = path
        videoSelected(path)
        console.log("AppState: selectedVideo ->", path)
    }

    function refresh() {
        refreshCounter++
        requestRefresh()
        console.log("AppState: refresh requested (", refreshCounter, ")")
    }
}
