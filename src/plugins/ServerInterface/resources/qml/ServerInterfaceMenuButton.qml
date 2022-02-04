import QtQuick 2.0
import QtQuick.Controls 2.0

Button{
    objectName: "ServerInterface"
    background: Rectangle {
        anchors.fill: parent
        color: "#30384f"
    }
    Text {
        id: text
        text: qsTr("ServerInterface")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: "#ADBDCB"
    }
}

