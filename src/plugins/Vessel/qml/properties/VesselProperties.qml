import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.15

import CQML 1.0 as C
import Vessel 1.0
import Trajectory 1.0

Rectangle{
    id: root
    color: "transparent"
    anchors.fill: parent
    property var trajectory

    Component.onCompleted: root.trajectory = modelData.trajectory

    MouseArea{
        anchors.fill: parent
        onWheel: {}
        preventStealing: true
    }

    ColumnLayout{
        anchors.fill: parent
        spacing: 3
        Text {
            text: "Type: " + modelData.type
            color: "#6ab2f2"
        }
        Text {
            text: "Name: " + modelData.objectName
            color: "#6ab2f2"
        }
        Text {
            text: "Velocity: " + modelData.velocity.toFixed(2)
            color: "#6ab2f2"
        }
        Text {
            text: "X: " + modelData.x.toFixed(2)
            color: "#6ab2f2"
        }
        Text {
            text: "Y: " + modelData.y.toFixed(2)
            color: "#6ab2f2"
        }
        Text {
            text: "Z: " + modelData.z.toFixed(2)
            color: "#6ab2f2"
        }
        Text {
            text: "Rotation: " + modelData.rotation.toFixed(2)
            color: "#6ab2f2"
        }

        Item {
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            opacity: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            C.PlayerSlider{
                id: rotateSlider
                anchors.fill: parent
                anchors.topMargin: 10

                enabled: parent.opacity == 1
                from: -180
                to: 180
                stepSize: 0.5
                value: 0
                knobVisible: () => { return rotateSlider.pressed }
                handleVisible: () => { return true }
                knobLabelText: (value) => { return value }

                palettes: QtObject {
                    property color backgroundFill: "gray"
                    property color loadedFill: Qt.lighter("green")
                    property color valueFill: "#2f6ea5"
                    property color handle: "#6ab2f2"
                    property color text: "#2f6ea5"
                }

                onPressedChanged: {
                    if (root.trajectory){
                        if (pressed){
                            root.trajectory.opacity = 0
                        }else{
                            root.trajectory.updateProperties()
                            root.trajectory.opacity = 1
                            root.trajectory.selectTrajectory(textArea.cursorPosition)
                        }
                    }
                }

                onValueChanged: modelData.rotation = value

                Component.onCompleted: value = modelData.rotation
            }
        }

        C.Button{
            id: createTrace
            text: removeMode ? "Remove Trace" : "Create Trace"
            Layout.fillWidth: true
            Layout.leftMargin: 4
            Layout.rightMargin: 4
            Layout.bottomMargin: 4
            Layout.preferredHeight: 30
            buttonRadius: 3
            textColor: removeMode ? "#e94040" : "black"
            buttonBackground:  "#6ab2f2"
            opacity: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            enabled: opacity == 1
            property bool removeMode: !(modelData.trajectory === null)

            onReleased: {
                if (!removeMode) { modelData.createTrajectory(); trajectory = modelData.trajectory }
                else { modelData.removeTrajectory(); trajectory = modelData.trajectory }
            }
        }

        Flickable {
            id: flickable
            Layout.preferredHeight: modelData.trajectory ? 200 : 0
            Layout.fillWidth: true
            Layout.leftMargin: 4
            Layout.rightMargin: 4
            clip: true
            opacity: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            enabled: opacity == 1

            flickableDirection: Flickable.VerticalFlick

            ScrollBar.vertical:  ScrollBar { policy: ScrollBar.AlwaysOn }
            Behavior on Layout.preferredHeight { NumberAnimation { duration: 200; easing.type: Easing.OutQuad } }

            onHeightChanged: {
                if (modelData.trajectory) textArea.text = modelData.trajectory.properties
                else textArea.text = ""
            }

            TextArea.flickable: TextArea{
                id: textArea
                placeholderText: "PROPERTIES"
                selectByMouse: true
                text: ""
                background: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 20
                    color: "#2a5378"
                    border.color: "#1f2936"
                    radius: 3
                }
                color: "#6ab2f2"

                onCursorPositionChanged: { if (root.trajectory) root.trajectory.selectTrajectory(cursorPosition); }

                onTextChanged:  { if (root.trajectory) { root.trajectory.properties = text; root.trajectory.selectTrajectory(cursorPosition) }}

                Rectangle {
                    color: "lightgray"
                    opacity: 0.5
                    height: textArea.cursorRectangle.height
                    width: parent.width
                    y: textArea.cursorRectangle.y
                    z: -1
                }
            }
        }

        ListView {
            id : stagesRepeater
            Layout.fillWidth: true
            Layout.preferredHeight: 400
//            delegate: processDelegate
            model : modelData.stages
            visible: count !== 0
            spacing: 10
            clip:true

            delegate: Rectangle {
                width: parent.width - 10
                x: parent.x + 5
                height: 80
                color: "#6ab2f2"
                Text {
                    property bool active: stagesRepeater.currentIndex === index
                    anchors.fill: parent
                    wrapMode: Text.WordWrap
                    text: (index+1) + "    " + modelData
                    verticalAlignment: Text.AlignVCenter
                    color: active ? "#212326" : "#2f6ea5"
                    font.pointSize: 12

                }
            }

            onCountChanged: {
//                if (moveToEnd) {
                    stagesRepeater.currentIndex = count - 1;
                    Qt.callLater( stagesRepeater.positionViewAtEnd )
//                }
            }

        }
//        Connections {
//            target: modelData
//            function onStagesChanged(newStage) {
//                stagesRepeater.currentIndex = newStage.length-1;
//            }
//        }

        C.Button {
            id: buttonRemove
            text: "Remove"
            Layout.fillWidth: true
            Layout.leftMargin: 4
            Layout.rightMargin: 4
            Layout.bottomMargin: 4
            buttonRadius: 3
            Layout.preferredHeight: 30
            buttonBackground: "#6ab2f2"
            opacity: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            enabled: opacity == 1
            onReleased: {
                sceneArea.scene.removeItem(modelData)
                sceneArea.currentItem = sceneArea.scene
                //console.log("pressed")
            }
        }

        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}


