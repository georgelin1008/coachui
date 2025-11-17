import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#2d2d2d"
    border.color: "#1a1a1a"
    border.width: 1
    
    property bool compositionInProgress: false
    
    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 10
        
        // 左側：影片清單
        VideoComposerVideoList {
            id: videoList
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            
            onAddVideoClicked: composerListPopup.open()
            onClearVideosClicked: {
                videoList.videoModel.clear()
                if (videoComposer) {
                    videoComposer.clearVideos()
                }
            }
        }
        
        Rectangle {
            Layout.fillHeight: true
            width: 1
            color: "#404040"
        }
        
        // 中間：合成參數
        VideoComposerParameters {
            id: parameters
            Layout.preferredWidth: 280
            Layout.fillHeight: true
        }
        
        Rectangle {
            Layout.fillHeight: true
            width: 1
            color: "#404040"
        }
        
        // 右側：進階選項和操作
        VideoComposerActions {
            id: actions
            Layout.preferredWidth: 180
            Layout.fillHeight: true
            
            compositionInProgress: root.compositionInProgress
            selectedCount: getSelectedCount()
            
            onComposeClicked: startComposition()
        }
    }
    
    // 連接 videoComposer 信號
    Connections {
        target: videoComposer
        
        function onCompositionStarted() {
            root.compositionInProgress = true
            console.log("合成開始")
        }
        
        function onCompositionProgress(percentage) {
            actions.updateProgress(percentage)
        }
        
        function onCompositionCompleted(frameCount) {
            console.log("合成完成，共", frameCount, "幀，開始導出")
            if (videoComposer) {
                var timestamp = new Date().getTime()
                var outputPath = "/home/mxpt2/Videos/composed_" + timestamp + ".mp4"
                console.log("導出路徑:", outputPath)
                videoComposer.exportComposedVideo(outputPath)
            }
        }
        
        function onExportCompleted(filePath) {
            root.compositionInProgress = false
            actions.resetProgress()
            console.log("影片已導出:", filePath)
            
            if (typeof statusHandler !== 'undefined' && statusHandler) {
                statusHandler.showSuccess("合成完成: " + filePath.split('/').pop())
            }
            
            if (videoPlayer && typeof videoPlayer.loadVideo === 'function') {
                videoPlayer.loadVideo(filePath)
            }
        }
        
        function onCompositionError(errorMsg) {
            root.compositionInProgress = false
            console.error("合成錯誤:", errorMsg)
            
            if (typeof statusHandler !== 'undefined' && statusHandler) {
                statusHandler.showError("合成失敗: " + errorMsg)
            }
        }
        
        function onExportStarted() {
            console.log("導出開始")
        }
        
        function onExportProgress(current, total) {
            var percentage = (current / total) * 100
            actions.updateProgress(percentage)
            console.log("導出進度:", current, "/", total)
        }
        
        function onExportError(errorMsg) {
            root.compositionInProgress = false
            console.error("導出錯誤:", errorMsg)
            
            if (typeof statusHandler !== 'undefined' && statusHandler) {
                statusHandler.showError("導出失敗: " + errorMsg)
            }
        }
    }
    
    // 專用的影片選擇彈窗
    VideoComposerListPopup {
        id: composerListPopup
        
        onVideoSelected: function(filePath) {
            addVideoToList(filePath)
        }
    }
    
    function addVideoToList(filePath) {
        var fileName = filePath.split('/').pop()
        
        for (var i = 0; i < videoList.videoModel.count; i++) {
            if (videoList.videoModel.get(i).path === filePath) {
                console.log("影片已存在於列表中")
                return
            }
        }
        
        videoList.videoModel.append({
            name: fileName,
            path: filePath,
            selected: true
        })
        
        if (videoComposer && typeof videoComposer.loadVideos === 'function') {
            videoComposer.loadVideos([filePath])
        }
    }
    
    function getSelectedCount() {
        var count = 0
        for (var i = 0; i < videoList.videoModel.count; i++) {
            if (videoList.videoModel.get(i).selected) {
                count++
            }
        }
        return count
    }
    
    function getSelectedPaths() {
        var paths = []
        for (var i = 0; i < videoList.videoModel.count; i++) {
            if (videoList.videoModel.get(i).selected) {
                paths.push(videoList.videoModel.get(i).path)
            }
        }
        return paths
    }
    
    function startComposition() {
        var selectedPaths = []
        for (var i = 0; i < videoList.videoModel.count; i++) {
            if (videoList.videoModel.get(i).selected) {
                selectedPaths.push(videoList.videoModel.get(i).path)
            }
        }
        
        if (selectedPaths.length < 2) {
            if (typeof statusHandler !== 'undefined' && statusHandler) {
                statusHandler.showError("請至少選擇兩個影片進行合成")
            }
            return
        }
        
        console.log("開始合成", selectedPaths.length, "個影片")
        
        if (videoComposer && typeof videoComposer.composeVideos === 'function') {
            videoComposer.composeVideos(selectedPaths)
        } else {
            console.error("videoComposer.composeVideos 不可用")
        }
    }
}
