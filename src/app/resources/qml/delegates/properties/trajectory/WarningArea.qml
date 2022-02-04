import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import CQML 1.0
import KLib 1.0

WavedMouseArea{
    id: warningArea
    clip: true
    property var trueFunc : () => { return true; }
    property var falseFunc : () => { return false;}
    property string warningText
    property Item sourceItem
    
    states: [
        State {
            name: "warning"
            when: trueFunc()
            AnchorChanges {
                target: warningArea
                anchors{
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                }
            }
        },
        State {
            name: "nowarning"
            when: falseFunc()
            AnchorChanges {
                target: warningArea
                anchors{
                    left:   parent.left
                    right:  parent.left
                    top:    parent.top
                    bottom: parent.top
                }
            }
        }
    ]
    
    ShaderEffectSource {
        id: effectSource
        
        sourceItem: warningArea.sourceItem? warningArea.sourceItem : layout
        anchors.fill: parent
        
        sourceRect: Qt.rect(x,y, width, height)

        Component.onCompleted: { if (!sourceItem)  console.log("Warning area: Missed source element")}
    }
    
    FastBlur{
        id: blur
        anchors.fill: effectSource
        
        source: effectSource
        radius: 64
        transparentBorder: true
    }
    
    Label {
        anchors.centerIn: parent
        color: "#e94040"
        text: warningText ? warningText : "Input warning"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 15
        font.bold: true
        background: Rectangle{
            height: parent.height * 1.5
            radius: height / 2
            anchors.centerIn: parent
            width: parent.width * 1.5
            color: "#1f2936"
        }
        
        z: 3
    }
    
    Rectangle{
        id: rect
        anchors.fill: parent
        color: "gray"
        opacity: 0.3
    }
}
