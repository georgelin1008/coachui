import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 8
    
    property bool compositionInProgress: false
    property int selectedCount: 0
    
    signal composeClicked()
    signal progressUpdated(real percentage)
    
    Text {
        text: "🔧 進階算法"
        font.pixelSize: 11
        font.bold: true
        color: "#ffffff"
    }
    
    ColumnLayout {
        Layout.fillWidth: true
        spacing: 5
        
        CheckBox {
            id: adaptiveThresholdCheck
            text: "自適應閾值"
            checked: false
            font.pixelSize: 9
            
            contentItem: Text {
                text: parent.text
                color: "#cccccc"
                font.pixelSize: 9
                leftPadding: parent.indicator.width + parent.spacing
                verticalAlignment: Text.AlignVCenter
            }
            
            onCheckedChanged: {
                if (videoComposer) {
                    videoComposer.adaptiveThreshold = checked
                }
            }
        }
        
        CheckBox {
            id: edgeEnhancementCheck
            text: "邊緣增強"
            checked: false
            font.pixelSize: 9
            
            contentItem: Text {
                text: parent.text
                color: "#cccccc"
                font.pixelSize: 9
                leftPadding: parent.indicator.width + parent.spacing
                verticalAlignment: Text.AlignVCenter
            }
            
            onCheckedChanged: {
                if (videoComposer) {
                    videoComposer.edgeEnhancement = checked
                }
            }
        }
        
        CheckBox {
            id: centerWeightingCheck
            text: "中心加權"
            checked: false
            font.pixelSize: 9
            
            contentItem: Text {
                text: parent.text
                color: "#cccccc"
                font.pixelSize: 9
                leftPadding: parent.indicator.width + parent.spacing
                verticalAlignment: Text.AlignVCenter
            }
            
            onCheckedChanged: {
                if (videoComposer) {
                    videoComposer.centerWeighting = checked
                }
            }
        }
    }
    
    Rectangle {
        Layout.fillWidth: true
        height: 1
        color: "#404040"
    }
    
    // 關鍵幀對齊選項
    ColumnLayout {
        Layout.fillWidth: true
        spacing: 5
        
        CheckBox {
            id: keyframeAlignCheck
            text: "🎯 關鍵幀對齊"
            checked: false
            font.pixelSize: 9
            font.bold: true
            
            contentItem: Text {
                text: parent.text
                color: "#ffd700"
                font.pixelSize: 9
                font.bold: true
                leftPadding: parent.indicator.width + parent.spacing
                verticalAlignment: Text.AlignVCenter
            }
            
            onCheckedChanged: {
                if (videoComposer) {
                    videoComposer.useKeyframeAlignment = checked
                }
                if (checked) {
                    updateCommonKeyframes()
                }
            }
        }
        
        ComboBox {
            id: keyframeSelector
            Layout.fillWidth: true
            enabled: keyframeAlignCheck.checked
            model: []
            displayText: currentIndex >= 0 ? currentText : "選擇關鍵幀..."
            
            background: Rectangle {
                color: parent.enabled ? "#3a3a3a" : "#2a2a2a"
                border.color: "#555555"
                radius: 3
            }
            
            contentItem: Text {
                text: parent.displayText
                color: parent.enabled ? "#ffffff" : "#666666"
                font.pixelSize: 9
                verticalAlignment: Text.AlignVCenter
                leftPadding: 8
            }
            
            onCurrentTextChanged: {
                if (videoComposer && currentText !== "") {
                    videoComposer.alignmentKeyframeName = currentText
                }
            }
        }
        
        Text {
            text: "⚠ 所有影片必須有此關鍵幀"
            color: "#888888"
            font.pixelSize: 8
            visible: keyframeAlignCheck.checked
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
        }
    }
    
    Item { Layout.fillHeight: true }
    
    function updateCommonKeyframes() {
        if (!keyframeManager || !root.parent) return
        
        // 從父組件獲取選中的影片路徑
        var selectedPaths = root.parent.getSelectedPaths ? root.parent.getSelectedPaths() : []
        
        if (selectedPaths.length < 2) {
            keyframeSelector.model = []
            return
        }
        
        // 查找共同的關鍵幀
        var commonKeyframes = keyframeManager.findCommonKeyframes(selectedPaths)
        keyframeSelector.model = commonKeyframes
        
        if (commonKeyframes.length > 0) {
            keyframeSelector.currentIndex = 0
        }
    }
    
    // 合成進度
    ColumnLayout {
        Layout.fillWidth: true
        spacing: 3
        visible: root.compositionInProgress
        
        ProgressBar {
            id: compositionProgressBar
            Layout.fillWidth: true
            from: 0
            to: 100
            value: 0
        }
        
        Text {
            id: progressText
            text: "合成中... 0%"
            color: "#4a9eff"
            font.pixelSize: 9
            Layout.alignment: Qt.AlignHCenter
        }
    }
    
    // 操作按鈕
    Button {
        Layout.fillWidth: true
        Layout.preferredHeight: 40
        text: "▶ 合成並導出"
        enabled: !root.compositionInProgress && root.selectedCount >= 2
        
        background: Rectangle {
            color: parent.enabled ? (parent.pressed ? "#45a045" : "#55c055") : "#555555"
            radius: 4
        }
        
        contentItem: Text {
            text: parent.text
            color: parent.enabled ? "#ffffff" : "#999999"
            font.pixelSize: 11
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        
        onClicked: root.composeClicked()
    }
    
    // 更新進度的函數
    function updateProgress(percentage) {
        compositionProgressBar.value = percentage
        progressText.text = "合成中... " + Math.floor(percentage) + "%"
    }
    
    function resetProgress() {
        compositionProgressBar.value = 0
        progressText.text = "導出完成！"
    }
}
