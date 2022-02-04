import QtQuick 2.0
import QtQuick.Controls 2.2
import KLib 1.0

Rectangle {
    id: root

    property var modelData;

    property string description: modelData.additionalQml.description !== undefined ? modelData.additionalQml.description : "No description"
    property double captionMargin: modelData.additionalQml.captionMargin !== undefined ? modelData.additionalQml.captionMargin : 60
    property int numbersAfterDot: modelData.additionalQml.numbersAfterDot !== undefined ? modelData.additionalQml.numbersAfterDot : 2
    property string caption : modelData.caption

    x: modelData.x
    y: modelData.y

    ManualMessage {
        id: manualMessage
        messageText: root.description
        target: parent
        radius: 5
        font.family: "Times New Roman"
        backgroundColor: "#565656"
        textColor: "white"
        z : 10
    }

    property var processor: (arg) => { return arg }

    width: valueText.width
    height: valueText.height

    Text {
        text: root.caption
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: valueText.top
        anchors.bottomMargin: captionMargin
        font.pointSize: 12
    }

    Rectangle {
        anchors.fill: valueText
        color: "#b6a092"
        border.color: "#958881"
        border.width: 2

        anchors.margins: -20
    }

    Text {
        id: valueText
        text: root.processor(modelData.value).toFixed(numbersAfterDot)
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 12
    }

    function updateHScale(scale) {
        y *= scale
        height *= scale
    }

    function updateWScale(scale) {
        x *= scale
        width *= scale
    }

//    Component.onCompleted: {
//        StreamConnector.bindToStream("position", (data) => {
//                                         modelData.value = data.distance
//                                     })
//    }

    MGKNumberViewPrivate {
        onValueChanged: {
//            console.log("value changed")
            modelData.value = value.distance
        }
    }

}
