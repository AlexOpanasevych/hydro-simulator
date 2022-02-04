import QtQuick 2.15

Rectangle {
    id: root
    property int size: 12
    property bool isOpen: false
    width: root.size
    height: root.size
    color: "yellow"
    Text {
        anchors.fill: parent
        text: root.isOpen ? "-" :"+"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 17
    }
    MouseArea {
        anchors.fill: parent
        onClicked: isOpen = !isOpen
    }

}
