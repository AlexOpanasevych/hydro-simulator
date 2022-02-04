import QtQuick 2.0
import KLib 1.0


Item {
    id: root
    property var modelData
    width: modelData.width
    height: modelData.height
    x: modelData.x
    y: modelData.y
    property double inputStepper: 0
    property var processor
    property double manualInputAngle: 0
    property string description : modelData.additionalQml.description !== undefined ? modelData.additionalQml.description : "No description"
    property string caption : modelData !== undefined ? modelData.caption : ""

    onModelDataChanged: {
        if (modelData.additionalQml.divBackground !== undefined) {
            backLoader.sourceComponent = modelData.additionalQml.divBackground !== "0" ? divBackComp : emptyBackComp
        }
    }

    Loader {
        id: backLoader
        anchors.fill: parent
        sourceComponent: divBackComp
    }
    Component {
        id: divBackComp
        Item {
            anchors.fill: parent
            Rectangle {
                id: backR
                anchors.fill: parent
                color: "#ac9b91"
                radius: 150
                border.width: 2
                border.color: "#514440"
            }
            DivisionBackground {
                id: divBack
                angles: stepper.angles
            }

        }
    }

    Component {
        id: emptyBackComp
        Item {}
    }


    property Item handle: Image {
        source: "file:/" + App.resPath + "/" + modelData.additionalQml.imageSource
        fillMode: Image.PreserveAspectFit
        height: modelData !== undefined && modelData.additionalQml !== undefined && modelData.additionalQml.divBackground !== undefined ? parent.height : parent.height / 2
        width: modelData !== undefined && modelData.additionalQml !== undefined && modelData.additionalQml.divBackground !== undefined ? parent.width : parent.width / 2
        anchors.centerIn: parent
    }

    Text {
        id: cap
        font.pointSize: 10
        anchors.bottom: root.bottom
        anchors.bottomMargin: 16
        //        anchors.topMargin: 8
        anchors.horizontalCenter: root.horizontalCenter
        text: root.caption
    }

    onManualInputAngleChanged: {
        stepper.inputAngle = manualInputAngle;
    }

    property variant angles: []
    property variant imageSource

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

    onHandleChanged: {
        initHandle();
    }

    Component.onCompleted: {
        initHandle();
    }

    function initHandle() {
        handle.parent = root
    }

    QtObject {
        id: internal
        property vector2d startPos: Qt.vector2d(0, 0)
    }


    SwitchStepper {
        angles: modelData.additionalQml.angles
        id: stepper

        property int prevValue
        property int prevAngle

        onOutputAngleChanged: {
            handle.rotation = stepper.outputAngle;
        }
    }

    Connections {
        target: modelData !== undefined ? modelData : null
        function onValueChanged(value) {
            stepper.outputValue = value[0]
            stepper.outputAngle = value[1]

            modelData.requestValue = stepper.outputValue
            App.client.sCommunicator.requestValueChanged(modelData.requestValue, modelData)
            stepper.outputAngle = stepper.prevAngle
            stepper.outputValue = stepper.prevValue
        }
    }
    MouseArea {
        anchors.fill: parent
        onPressed: {
            internal.startPos = Qt.vector2d(mouseX, mouseY);
            stepper.prevAngle = stepper.outputAngle
            stepper.prevValue = stepper.outputValue
        }

        onMouseYChanged: {
            mousePosChanged();
        }

        onMouseXChanged: {
            mousePosChanged();
        }

        //        function bound(val) { return Math.max(range.minimumValue, Math.min(range.maximumValue, val)); }

        function mousePosChanged() {
            var currentPos = Qt.vector2d(mouseX, mouseY);
            var vector1 = currentPos.minus(internal.startPos);
            if(vector1.length() > 10) {
                stepper.inputAngle = KMath.constrainDegrees(KMath.getDegrees(vector1, Qt.vector2d(0, -1)));
            }
        }
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
