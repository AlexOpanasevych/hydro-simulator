import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import KLib 1.0

Item {
    id: root

    property var modelData;

    x : modelData ? modelData.x : 0
    y : modelData ? modelData.y : 0
    height : modelData.height
    width : modelData.width

    property color enabledColor: modelData !== undefined && modelData.additionalQml !== undefined && modelData.additionalQml.enabledColor !== undefined ? modelData.additionalQml.enabledColor : "#FFC273"
    property color disabledColor: modelData !== undefined && modelData.additionalQml !== undefined && modelData.additionalQml.disabledColor !== undefined ? modelData.additionalQml.disabledColor : "#eeeeee"
    property int ledOn: /*modelData !== undefined && modelData.value !== undefined && modelData.value.led !== undefined ? modelData.value : false*/0
    property double backgroundRadius: 14
    property string caption: modelData.caption
    property bool inButtonGroup: false
    property string description : modelData !== undefined && modelData.additionalQml !== undefined && modelData.additionalQml.description !== undefined ? modelData.additionalQml.description : "No description"
    property alias pressed: button.pressed
    property alias ledMode: ledMode
    property alias ledTimer: ledTimer
    property bool canHold: modelData !== undefined && modelData.additionalQml !== undefined && modelData.additionalQml.canHold !== undefined ? modelData.additionalQml.canHold : false

    onLedOnChanged: {
        console.log("I'm changed")
    }

    QtObject {
        id: ledMode
        property string constantly: "constantly"
        property string blinkOneTime: "blink"
        property string tickling: "tickling"
        property string currentMode

        onCurrentModeChanged: {
            if(currentMode !== ledMode.constantly && currentMode !== "")
                ledTimer.start()
        }
    }



    antialiasing: true

    Button {
        id: button
        anchors.fill: parent
        background: Rectangle {
            id: backgroundRect
            color: ledOn ? enabledColor : disabledColor
            border.width: button.pressed ? 2 : 1
            border.color: "#333333"
            radius: backgroundRadius
        }

//        text: ledOn

        onPressed: {
            if(!inButtonGroup) {
                if(ledMode.currentMode === ledMode.tickling){
                    ledTimer.stop()
                    modelData.requestValue = "released"
                }
                else
                    modelData.requestValue = !ledOn ? "pressed" : "released"

                App.client.sCommunicator.requestValueChanged(modelData.requestValue, modelData)
            }
        }

        onPressAndHold: {
            if(canHold) {
                holdTimer.start()
            }
        }

        onReleased: {
            if(canHold) {
                holdTimer.stop()
            }
        }
    }

    Timer {
        id: holdTimer

        interval: 100

        repeat: true

        onTriggered: {
            modelData.requestValue = "pressed"
            App.client.sCommunicator.requestValueChanged(modelData.requestValue, modelData)
        }
    }

    Timer {
        id: ledTimer

        interval: 200

        repeat: /*if(!inButtonGroup) */ledMode.currentMode === ledMode.tickling ? true : false

        onTriggered: ledOn = !ledOn;

        onRunningChanged: {
            if(!running) {

                if(ledMode.currentMode === ledMode.blinkOneTime) {
                    modelData.requestValue = "released"
                    App.client.sCommunicator.requestValueChanged(modelData.requestValue, modelData)
                }

            }
        }
//        triggeredOnStart: ledMode.currentMode === ledMode.blinkOneTime

    }




    Connections {
        target: modelData !== undefined ? modelData : null
        function onValueChanged(value) {
            if(!inButtonGroup) {
                ledOn = value.led !== undefined ? parseInt(value.led) : ledOn
                ledMode.currentMode = value.ledMode !== undefined ? value.ledMode : ledMode
                console.log("value changed", ledOn)
            }
        }
    }






    Rectangle {
        id: blurRectangle
        anchors.fill: parent
        radius: backgroundRect.radius
        color: "#a0000000"
        visible: button.pressed
    }
    Rectangle {
        id: buttonPress
        anchors.centerIn: parent
        visible: button.pressed
        height: parent.height / 1.2
        width: height
        radius: backgroundRect.radius
        color: backgroundRect.color
    }
//    InnerShadow {
//        anchors.fill: parent
//        visible: button.pressed
//        cached: true
//        radius: 15.0
//        samples: 31
//        source: backgroundRect
//        color: "#a0000000"
//    }

//    InnerShadow {
//        anchors.fill: parent
//        visible: button.pressed
//        cached: true
//        radius: 15.0
//        samples: 31
//        source: button
//        color: "#a0000000"
//    }


    DropShadow {
       anchors.fill: buttonPress
       cached: true
       visible: button.pressed
       horizontalOffset: 0
       verticalOffset: 0
       radius: 15.0
       samples: 31
       color: "#a0000000"
       source: buttonPress
    }

    Text {
        id: buttonText
        anchors.fill: parent
        text: caption
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: (disabledColor.r < 0.5 || disabledColor.g < 0.5 || disabledColor.a < 0.5) ? "#ffffff" : "#000000"
        font.pointSize: 12
        wrapMode: Text.WordWrap
        textFormat: Text.RichText
    }

    function updateHScale(scale) {
        y *= scale
        height *= scale
    }

    function updateWScale(scale) {
        x *= scale
        width *= scale
    }

    ManualMessage {
        id: manualMessage
        messageText: root.description
        target: parent
        radius: 5
        font.family: "Times New Roman"
        backgroundColor: "#565656"
        textColor: "white"
        customCondition: !inButtonGroup
        z : 10
    }
}
