import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls 1.4 as Old
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.15
import KLib 1.0

Item
{
    id: button
    property alias text: textArea.text
    property color textColor: "black"
    signal clicked()
    Layout.fillWidth: true
    Layout.preferredHeight: 50
    clip: true
    
    LinearGradient {
        anchors.fill: parent
        start: Qt.point(0, 0)
        end: Qt.point(0, button.height)
        opacity: 0.3
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: "transparent"
            }
            GradientStop {
                position: 1.0
                color: "#fffdfe"
            }
        }
    }
    
    Shape {
        anchors.fill: parent
        ShapePath {
            fillColor: "transparent"
            strokeColor: "#7f8c92"
            strokeWidth: 1
            startX: 0
            startY: button.height - 1
            PathLine { x: button.width; y: button.height - 1 }
            PathMove { x: 0; y : 0 }
            PathLine { x: button.width; y : 0}
        }
    }
    
    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onReleased: {
            button.clicked()
        }
    }

    Text {
        id: textArea
        anchors.fill: parent
        text: ""
        color: textColor
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
