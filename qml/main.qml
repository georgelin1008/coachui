import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

/*
  main.qml - Based on main_1029 (fully working version)
  Key principle: Sub-components directly access global C++ context properties
  (cameraController, videoPlayer, videoComposer, recorder, appController)
  WITHOUT property passing - this avoids null binding issues.
  
  Each sub-component directly uses: target: cameraController, etc.
*/

ApplicationWindow {
    id: appWindow
    visible: true
    width: 1280
    height: 800
    title: "CoachUI - 攝影機預覽與錄製"
    
    // AppState - fully QML-based
    AppState { id: appState }
    
    // No header
    header: null

    // ===== MAIN 3-TIER LAYOUT =====
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // ===== TIER 1: CONTROLS (Fixed 180px) =====
        ControlsPanel {
            id: controlsPanel
            Layout.fillWidth: true
            Layout.preferredHeight: 180
            
            Component.onCompleted: {
                console.log("✅ ControlsPanel fully loaded")
            }
        }

        // ===== TIER 2: VIDEO AREA (Fills) =====
        VideoArea {
            id: videoArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            appWindow: appWindow
            
            Component.onCompleted: {
                console.log("✅ VideoArea fully loaded")
            }
        }

        // ===== TIER 3: STATUS BAR (Fixed 28px) =====
        StatusBar {
            id: statusBar
            Layout.fillWidth: true
            Layout.preferredHeight: 28
            appState: appState
            
            Component.onCompleted: {
                console.log("✅ StatusBar fully loaded")
            }
        }
    }

    // ===== VIDEO LIST POPUP =====
    VideoListPopup {
        id: videoListPopup
        
        Component.onCompleted: {
            console.log("✅ VideoListPopup loaded")
            console.log("   Global appController:", typeof appController)
        }
    }

    // ===== LISTEN TO CONTROLS PANEL SIGNALS =====
    Connections {
        target: controlsPanel
        function onOpenVideoListRequested() {
            console.log("� Opening video list popup...")
            videoListPopup.open()
        }
    }

    // ===== LISTEN TO VIDEO LIST POPUP SIGNALS =====
    Connections {
        target: videoListPopup
        function onVideoSelected(filePath) {
            console.log("� Video selected:", filePath)
            appState.selectVideo(filePath)
        }
    }

    // ===== ROUTE VIDEO SELECTIONS TO VIDEOPLAYER =====
    Connections {
        target: appState
        function onVideoSelected(filePath) {
            if (videoPlayer && typeof videoPlayer.loadVideo === 'function') {
                console.log("🎬 Loading video:", filePath)
                videoPlayer.loadVideo(filePath)
            }
        }
    }

    // ===== APP INITIALIZATION =====
    Component.onCompleted: {
        console.log("")
        console.log("========== CoachUI INITIALIZED ==========")
        console.log("Window: " + width + " x " + height)
        console.log("Global C++ Context Properties:")
        console.log("  ✓ cameraController:", typeof cameraController !== 'undefined' ? "OK" : "MISSING")
        console.log("  ✓ videoPlayer:", typeof videoPlayer !== 'undefined' ? "OK" : "MISSING")
        console.log("  ✓ videoComposer:", typeof videoComposer !== 'undefined' ? "OK" : "MISSING")
        console.log("  ✓ recorder:", typeof recorder !== 'undefined' ? "OK" : "MISSING")
        console.log("  ✓ appController:", typeof appController !== 'undefined' ? "OK" : "MISSING")
        console.log("==========================================")
        console.log("")
    }
}
