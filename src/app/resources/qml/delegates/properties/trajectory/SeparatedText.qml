import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15

import "../../../items/controls" as C
import "../../../items/base"

Text {
    id: typeText
    color: "#6ab3f3"
    font.pixelSize: 15
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    wrapMode: Text.WordWrap
    
    Shape{
        Layout.fillWidth: true
        Layout.preferredHeight: 5
        ShapePath {
            fillColor: "transparent"
            strokeColor: "#7f8c92"
            strokeWidth: 1
            startX: 0
            startY: typeText.height - 1
            PathLine { x: typeText.width; y: typeText.height - 1 }
        }
        
    }
    
    Rectangle{
        color: "white"
        anchors.fill: typeText
        opacity: 0.1
    }
    
}
