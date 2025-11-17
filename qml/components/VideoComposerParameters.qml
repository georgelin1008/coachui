import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 8
    
    Text {
        text: "⚙️ 合成參數"
        font.pixelSize: 11
        font.bold: true
        color: "#ffffff"
    }
    
    ColumnLayout {
        Layout.fillWidth: true
        spacing: 6
        
        // 前景權重
        RowLayout {
            Layout.fillWidth: true
            
            Text {
                text: "前景:"
                color: "#cccccc"
                font.pixelSize: 9
                Layout.preferredWidth: 40
            }
            
            Slider {
                id: foregroundSlider
                Layout.fillWidth: true
                from: 0.1
                to: 4.0
                value: 1.0
                stepSize: 0.1
                
                onValueChanged: {
                    if (videoComposer) {
                        videoComposer.foregroundWeight = value
                    }
                }
            }
            
            Text {
                text: foregroundSlider.value.toFixed(1)
                color: "#4a9eff"
                font.pixelSize: 10
                Layout.preferredWidth: 30
            }
        }
        
        // 背景權重
        RowLayout {
            Layout.fillWidth: true
            
            Text {
                text: "背景:"
                color: "#cccccc"
                font.pixelSize: 9
                Layout.preferredWidth: 40
            }
            
            Slider {
                id: backgroundSlider
                Layout.fillWidth: true
                from: 0.0
                to: 1.0
                value: 0.3
                stepSize: 0.05
                
                onValueChanged: {
                    if (videoComposer) {
                        videoComposer.backgroundWeight = value
                    }
                }
            }
            
            Text {
                text: backgroundSlider.value.toFixed(2)
                color: "#4a9eff"
                font.pixelSize: 10
                Layout.preferredWidth: 30
            }
        }
        
        // 模糊大小
        RowLayout {
            Layout.fillWidth: true
            
            Text {
                text: "模糊:"
                color: "#cccccc"
                font.pixelSize: 9
                Layout.preferredWidth: 40
            }
            
            Slider {
                id: blurSlider
                Layout.fillWidth: true
                from: 1
                to: 50
                value: 5
                stepSize: 2
                
                onValueChanged: {
                    if (videoComposer) {
                        videoComposer.blurSize = Math.floor(value)
                    }
                }
            }
            
            Text {
                text: Math.floor(blurSlider.value).toString()
                color: "#4a9eff"
                font.pixelSize: 10
                Layout.preferredWidth: 30
            }
        }
        
        // 前景閾值
        RowLayout {
            Layout.fillWidth: true
            
            Text {
                text: "閾值:"
                color: "#cccccc"
                font.pixelSize: 9
                Layout.preferredWidth: 40
            }
            
            Slider {
                id: thresholdSlider
                Layout.fillWidth: true
                from: 10
                to: 100
                value: 25
                stepSize: 5
                
                onValueChanged: {
                    if (videoComposer) {
                        videoComposer.threshold = Math.floor(value)
                    }
                }
            }
            
            Text {
                text: Math.floor(thresholdSlider.value).toString()
                color: "#4a9eff"
                font.pixelSize: 10
                Layout.preferredWidth: 30
            }
        }
    }
    
    Item { Layout.fillHeight: true }
}
