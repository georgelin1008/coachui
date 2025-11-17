import QtQuick
import QtQuick.Controls

Item {
    id: root
    property bool enabled: false
    property color strokeColor: "#FF0000"
    property real strokeWidth: 3
    
    // 儲存所有筆畫
    property var strokes: []
    property var currentStroke: null

    function clear() {
        strokes = []
        canvas.requestPaint()
    }
    
    function undo() {
        if (strokes.length > 0) {
            strokes.pop()
            canvas.requestPaint()
        }
    }

    Canvas {
        id: canvas
        anchors.fill: parent
        renderStrategy: Canvas.Threaded
        renderTarget: Canvas.FramebufferObject
        
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)
            
            // 繪製所有已完成的筆畫
            for (var i = 0; i < strokes.length; i++) {
                var stroke = strokes[i]
                if (stroke.points.length < 2) continue
                
                ctx.strokeStyle = stroke.color
                ctx.lineWidth = stroke.width
                ctx.lineCap = "round"
                ctx.lineJoin = "round"
                
                ctx.beginPath()
                ctx.moveTo(stroke.points[0].x, stroke.points[0].y)
                for (var j = 1; j < stroke.points.length; j++) {
                    ctx.lineTo(stroke.points[j].x, stroke.points[j].y)
                }
                ctx.stroke()
            }
            
            // 繪製當前正在畫的筆畫
            if (currentStroke && currentStroke.points.length >= 2) {
                ctx.strokeStyle = currentStroke.color
                ctx.lineWidth = currentStroke.width
                ctx.lineCap = "round"
                ctx.lineJoin = "round"
                
                ctx.beginPath()
                ctx.moveTo(currentStroke.points[0].x, currentStroke.points[0].y)
                for (var k = 1; k < currentStroke.points.length; k++) {
                    ctx.lineTo(currentStroke.points[k].x, currentStroke.points[k].y)
                }
                ctx.stroke()
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        enabled: root.enabled
        hoverEnabled: true
        cursorShape: enabled ? Qt.CrossCursor : Qt.ArrowCursor
        
        onPressed: {
            // 開始新筆畫
            currentStroke = {
                color: root.strokeColor,
                width: root.strokeWidth,
                points: [{x: mouse.x, y: mouse.y}]
            }
        }
        
        onPositionChanged: {
            if (currentStroke) {
                // 添加點到當前筆畫
                currentStroke.points.push({x: mouse.x, y: mouse.y})
                
                // 即時重繪整個畫布（包含當前筆畫）
                canvas.requestPaint()
            }
        }
        
        onReleased: {
            if (currentStroke) {
                // 儲存筆畫
                strokes.push(currentStroke)
                currentStroke = null
                canvas.requestPaint()
            }
        }
    }
}
