import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import KLib 1.0

MouseArea {
    id: toolTipRoot
    visible: App.viewMode === App.HELP && customCondition
    z: 20
    anchors.fill: target

    hoverEnabled: true

    property string messageText

    property alias radius: content.radius
    property alias backgroundColor: content.color
    property alias textColor: toolTip.color
    property alias font: toolTip.font
    property Item target: null
    property bool customCondition: true

    function onMouse(x, y)
    {
        var obj = mapToItem(toolTipRoot, x, y);
        toolTipContainer.x = obj.x;
        toolTipContainer.y = obj.y;
        toolTipContainer.visible = true
    }


    Item {
        id: toolTipContainer
        z: toolTipRoot.z + 1
        width: content.width
        height: content.height
        visible: toolTipRoot.containsMouse

        Rectangle {
            id: content
            anchors.centerIn: parent
            width: toolTip.paintedWidth + 10
            height: toolTip.paintedHeight + 10
            radius: 3

            Text {
                id: toolTip
                font.family: "Times New Roman"
                anchors {margins: 5}
                text: messageText.length > 0 ? messageText : "No description"
                wrapMode: Text.WordWrap
            }
        }
//        DropShadow {
//            id: toolTipShadow
//            z: toolTipRoot.z + 1
//            anchors.fill: parent
//            cached: true
//            horizontalOffset: 4
//            verticalOffset: 4
//            radius: 8.0
//            samples: 16
//            color: "#80000000"
//            smooth: true
//            source: toolTipContainer
//        }
        Behavior on opacity { NumberAnimation { duration: 200 }}
    }

    onEntered: {
        onMouse(mouseX, mouseY)
//        console.log("manual message should appear...")
    }
    onExited: {
        toolTipContainer.visible = false
    }
}
