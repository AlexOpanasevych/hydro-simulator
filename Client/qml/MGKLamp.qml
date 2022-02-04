import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: root

    property var modelData;

    property string description: modelData.additionalQml.description !== undefined ? modelData.additionalQml.description : "No description"
    property string caption : modelData.caption;
    property bool light: modelData.value

    x : modelData.x;
    y : modelData.y
    width : modelData.width
    height : modelData.height
    opacity : light ? 1 : 0

    Rectangle {
        anchors.fill: parent
        border.color: "#958881"
        border.width: 2
        color: light ? "#FFC273" : "#eeeeee"
        radius: 12
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            text: root.caption
        }
    }

    ManualMessage {
        messageText: root.description
        target: parent
        radius: 5
        font.family: "Times New Roman"
        backgroundColor: "#565656"
        textColor: "white"
        z : 99999999
    }

    Timer {
        id: timer
        repeat: false
        interval: 500
        onTriggered: light = false
    }

    function enableTimeout(timeout) {
        timer.interval = timeout
        timer.start()
        light = true
    }

    function updateHScale(scale) {
        y *= scale
        height *= scale
    }

    function updateWScale(scale) {
        x *= scale
        width *= scale
    }
}
