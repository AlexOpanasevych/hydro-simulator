import QtQuick 2.0
import QtQuick.Controls 2.12
import KLib 1.0

Item {
    id: root
    property var modelData;

    property bool switchMode: false
    property bool incrementMode: false
    property bool decrementMode: false
    property string description: modelData.caption
    property bool clamp: true
    property int min: 0
    property int max: 360
    property double delta: 0.5

    property int sticking: 0

    x: modelData.x
    y: modelData.y
    width: modelData.width
    height: modelData.height

    QtObject {
        id: internal
        property bool switchState: false
    }

    ManualMessage {
        messageText: root.description
    }

//    Timer {
//        id: stickingTimer
//        interval: sticking; repeat: true
//        onTriggered: {
//            modelData.currentState = modelData.actions[0] // clicked
//            modelData.requestValue = true
//        }
//        onRunningChanged: {
//            if(!running) {
//                modelData.currentState = modelData.actions[0] // clicked
//                modelData.requestValue = false
//            }
//        }
//    }


    Button {
        id: button
        anchors.fill: parent
        property bool ledOn: modelData.value
        property string caption : modelData.caption

        background: Rectangle {
            border.width: 1
            color: "#ac9b91"
            border.color: "#333333"
            radius: root.width

            Rectangle {
                color: button.ledOn ? "#FFC273" : "#eeeeee"
                border.width: button.pressed ? 2 : 1
                border.color: "#333333"
                radius: root.width
                anchors.fill: parent
                anchors.margins: 8
            }
        }

        Text {
            anchors.horizontalCenter: button.horizontalCenter
            anchors.top: button.bottom
            anchors.topMargin: 16
            text: modelData.caption
            font.pointSize: 12
        }

        onPressAndHold: {
//            stickingTimer.start();
        }
        onReleased: {
//            stickingTimer.stop();
            // clicked
            modelData.requestValue = !modelData.value
            App.client.sCommunicator.requestValueChanged(modelData.requestValue, modelData)
        }

        onPressed: {
            // change
//            if(sticking == 0) {
//                modelData.currentState = modelData.actions[0] // clicked
//                modelData.requestValue = !modelData.value
//            }
        }
    }

}
