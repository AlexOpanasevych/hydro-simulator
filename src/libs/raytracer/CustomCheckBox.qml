import QtQuick 2.12
import QtQuick.Controls 2.12

CheckBox {
    id: root
    property int mode: -1
    property int idx: -1
    indicator: Rectangle {
        implicitWidth: 26
        implicitHeight: 26
        x: root.leftPadding
        y: parent.height / 2 - height / 2
        radius: 3
        border.color: "#333333"
        color: "#eeeeee"
        Rectangle {
            width: 14
            height: 14
            x: 6
            y: 6
            radius: 2
            color: "#FFC273"
            border.color: "#333333"
            visible: (root.idx == root.mode)
        }
    }
    //custom text
    contentItem: Text {
        text: root.text
        font: root.font
        color: "#333333"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        leftPadding: root.indicator.width + root.spacing
    }
}
