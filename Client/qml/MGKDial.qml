import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.1

import KLib 1.0

Item {
    id: root
    property int index;
    x : modelData.x
    y : modelData.y
    height : modelData.height
    width : modelData.width

    onXChanged: {
        console.log(modelData.x)
    }
    onYChanged: {
        console.log(modelData.y)
    }

    property real prevVal;

    property string caption : modelData.caption

    property double maxValue: 1
    property double minValue: 0

    property double minAngle: -140
    property double maxAngle: 140

    property double imageRotation: 0
    property bool valueConstrain: false

    property string imageSource;

    property alias fontSize: cap.font.pointSize
    antialiasing: true

    property var modelData;

    imageSource : "file:/" + App.resPath + "/" + modelData.additionalQml.imageSource

    Image {
        id: image
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        clip: true
        source: imageSource
        onSourceChanged: {
            console.log(source)
        }
//        rotation: dial.value
        onRotationChanged: {
            console.log(rotation)
        }

        Component.onCompleted: {
            rotation = 2 * dial.value * 133 - 133
        }
    }

//    CustomDial {
//        id: dial

//        property real prevValue;

//        anchors {
//            top: parent.top
//            left: parent.left
//        }
//        diameter: root.height
//        backgroundColor: "transparent"

//        helm: Rectangle {
//            width: 24
//            height: 24
//            radius: 12
////            border.width: 1
////            border.color: "black"
//            color: "transparent"
//        }

//        onValueChanged: {
//            image.rotation = value;
//        }

//        onPressed: {
//            prevValue = value
//        }

//        onReleased: {
//            modelData.requestValue = {"value" : value, "extra" : ["any", "same"]}
//            App.client.sCommunicator.requestValueChanged(modelData.requestValue, modelData)
//            dial.setValue(prevVal);
//        }

//    }

    Dial {
        id: dial
        property real prevValue;
        anchors.fill: parent

        background: Item {
            width: root.height
            height: width
//            color: "transparent"
//            radius: width / 2
        }

        handle: Item {}

        anchors {
            top: parent.top
            left: parent.left
            fill: parent
        }

        onValueChanged: {
            image.rotation = 2 * value * 133 - 133;
        }

//        handle: image

        Timer {
            id: sendTimer
            interval: 100
            repeat: true
            property real lastValue : 0.0
            onTriggered: {
                if(lastValue !== dial.value) {

                    modelData.requestValue = {"value" : dial.value, "extra" : ["any", "same"]}
                    App.client.sCommunicator.requestValueChanged(modelData.requestValue, modelData)

                    lastValue = dial.value
                }
            }
        }

        onPressedChanged: {
            if(pressed) {
                prevValue = value
                sendTimer.start()
            }
            else {
                sendTimer.stop()
                if(value !== sendTimer.lastValue) {
                    modelData.requestValue = {"value" : value, "extra" : ["any", "same"]}
                    App.client.sCommunicator.requestValueChanged(modelData.requestValue, modelData)
                }
            }
        }
    }

    Connections {
        target: !modelData ? null : modelData
        function onValueChanged(value) {
            dial.value = value.value
        }
    }



    Text {
        id: cap
        font.pointSize: 12
        anchors.top: dial.bottom
        anchors.topMargin: 8
        anchors.horizontalCenter: root.horizontalCenter
        text: root.caption
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
        image.rotation = imageRotation;
    }
}
