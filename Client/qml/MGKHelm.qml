import QtQuick 2.0
import QtQuick.Controls 2.0

import KLib 1.0

Item {
    id: root
    property var modelData;
    property string caption : qsTr(modelData.caption)
    x : modelData.x
    y : modelData.y
    height : modelData.height
    width : modelData.width
//    property string imageSource : "file:/" + App.resPath + "/" + modelData.additionalQml.imageSource
    // change !!!
    //property var value: helmArea.fullAngle
    property int revolutionRatio: 64
    property int maxRotationCount: 1
//    property alias infinite: helmArea.infinite
    property color backgroundHelm: "#b6a092"
//    property color handleColor: "#b6a092"
    property bool vertical: modelData !== undefined && modelData.additionalQml !== undefined && modelData.additionalQml.vertical !== undefined ? modelData.additionalQml.vertical : false
    property string description : modelData !== undefined && modelData.additionalQml !== undefined && modelData.additionalQml.description !== undefined ? modelData.additionalQml.description : "No description"


    ManualMessage {
        messageText: root.description
        target: parent
        radius: 5
        font.family: "Times New Roman"
        backgroundColor: "#565656"
        textColor: "white"
        z : 10
    }

    CustomDial {
        id: dial

        backgroundColor: backgroundHelm

        property real prevValue;
        property real fullAngle;
        property int verifyFullTurn;
        property bool live : modelData.additionalQml.live !== undefined ? modelData.additionalQml.live : false
        property real lastReceivedValue
        property int lastReceivedFullTurn
        property bool usingSlider: false
        onLiveChanged: {
            console.log("live changed", live)
        }

        helm: Rectangle {
            width: 30
            height: 30
            radius: 15
            y: 25
            border.width: 2
            border.color: "#514440"

            color: backgroundHelm
        }

        onPressed: {
            prevValue = value
            verifyFullTurn = fullTurn
        }

        onValueChanged: {
            if(live && !usingSlider) {

                modelData.requestValue = {"value" : {"value" : value, "fullTurn" : fullTurn}, "extra" : ["any", "same"]}/*value + fullTurn * 360  > prevFullAngle ? value + fullTurn * 360 : fullTurn * 360 + value - prevValue*/
//                console.log(JSON.stringify(modelData.requestValue))
                //dial.setValue(prevValue);
                //dial.fullTurn = verifyFullTurn
                App.client.sCommunicator.requestValueChanged(modelData.requestValue, modelData)
//                console.log("full turn, value changed", dial.fullTurn)

            }
        }

        onReleased: {
            if(!live) {
                modelData.requestValue = {"value" : {"value" : value, "fullTurn" : fullTurn}, "extra" : ["any", "same"]}/*value + fullTurn * 360  > prevFullAngle ? value + fullTurn * 360 : fullTurn * 360 + value - prevValue*/
                dial.setValue(prevValue);
                dial.fullTurn = verifyFullTurn
                App.client.sCommunicator.requestValueChanged(modelData.requestValue, modelData)
//                console.log("full turn, released", dial.fullTurn)

            }
            else {
                dial.setValue(lastReceivedValue)
                dial.fullTurn = lastReceivedFullTurn
            }
        }
    }

    Connections {
        target: modelData
        function onValueChanged(value) {
            if(!dial.live) {
                dial.fullTurn = value.value.fullTurn
                dial.setValue(value.value.value)
                console.log(dial.value, dial.fullTurn)
            }
            else {
                dial.lastReceivedValue = value.value.value
                dial.lastReceivedFullTurn = value.value.fullTurn
            }
        }
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        y: 220
        text: caption
        id: captionText
    }

    property double delta: 2.5

    Slider {
        orientation: root.vertical ? Qt.Vertical : Qt.Horizontal
        background: Rectangle {
            color: "#efefef";
            border.color: "#514440"
            anchors.fill: parent
            border.width: 1
            radius: 18

            Rectangle {
                color: "#efefef";
                border.color: "#514440"
                radius: 12
                border.width: 1
                width: root.vertical ? parent.width / 4 : parent.width - 24
                height: root.vertical ? parent.height - 24 : parent.height / 4
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        id: velocitySlider
        anchors.horizontalCenter: { if(!root.vertical) return parent.horizontalCenter }
        anchors.verticalCenter: { if(root.vertical) return parent.verticalCenter }
        anchors.bottom: { if(!root.vertical) return parent.top }
        anchors.right: { if(root.vertical) return parent.left }
        width: {if(!root.vertical) return parent.width / 1.5}
        height: {if(root.vertical) return parent.height / 1.5}

        property real periodValue: 0
        property real periodFullTurn: 0

        Timer {
            id: incrementTimer
//            running: true
            repeat: true
            interval: 50
            onTriggered: {
                if(parent.value !== 0) {
                    velocitySlider.periodValue += parent.value * delta
//                    console.log("periodValue....", velocitySlider.periodValue)
                    velocitySlider.periodFullTurn = Math.floor(velocitySlider.periodValue / 360)
//                    console.log("fullTurn....", velocitySlider.periodFullTurn)
                    dial.setValue(Math.fmod(dial.value + parent.value * delta, 360));
                    if(parent.live) {
                        modelData.requestValue = {"value" : {"value" : dial.value, "fullTurn" : dial.fullTurn + velocitySlider.periodFullTurn}, "extra" : ["any", "same"]}
//                        console.log("......", JSON.stringify(modelData.requestValue))
                        App.client.sCommunicator.requestValueChanged(modelData.requestValue, modelData)
                    }
//                    console.log(velocitySlider.periodFullTurn, velocitySlider.periodValue)
                }
            }
        }

        onPressedChanged: {
            if(!pressed) {
                if(!live) {
                    modelData.requestValue = {"value" : {"value" : dial.value, "fullTurn" : dial.fullTurn + velocitySlider.periodFullTurn}, "extra" : ["any", "same"]}
                    dial.setValue(dial.prevValue)
                    dial.fullTurn = dial.verifyFullTurn
                    App.client.sCommunicator.requestValueChanged(modelData.requestValue, modelData)
                }
                else {
                    dial.setValue(dial.lastReceivedValue)
                    dial.fullTurn = dial.lastReceivedFullTurn
                }
                dial.usingSlider = false
                value = 0
                periodValue = 0
                incrementTimer.stop()
            }
            else {
                dial.usingSlider = true;
                dial.prevValue = dial.value;
                velocitySlider.periodValue = dial.value
                incrementTimer.restart()
            }
        }

        stepSize: 1
        from: -4
        to: 4
        anchors.margins: 8
    }

    function updateHScale(scale) {
        y *= scale
        height *= scale
    }

    function updateWScale(scale) {
        x *= scale
        width *= scale
    }

    Component.onCompleted: {
        Math.fmod = function (a,b) { return Number((a - (Math.floor(a / b) * b)).toPrecision(8)); };
    }
}
