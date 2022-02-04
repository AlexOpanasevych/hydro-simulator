import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls 1.4 as Old
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.15
import KLib 1.0

TabButton {
    required property Item tabbar
    
    background: Rectangle{
        id: bg
        color: "transparent"
        
        Shape{
            anchors.fill: parent
            ShapePath {
                fillColor: "transparent"
                strokeColor: "#7f8c92"
                strokeWidth: 1
                startX: 0
                startY: bg.height - 1
                PathMove { x: 0; y : 0 }
                PathLine { x: bg.width; y: 0 }
                PathLine { x: bg.width; y : bg.height - 1 }
            }
        }
        
        Rectangle{
            anchors.fill: parent
            color: checked ? "white" : "transparent"
            opacity: checked ? 0.3 : 1
        }
        
    }
    
}
