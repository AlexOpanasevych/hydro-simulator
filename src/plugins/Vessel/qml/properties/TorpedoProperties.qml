import QtQuick 2.15
import QtQuick.Layouts 1.15

<<<<<<< HEAD
import Trajectory 1.0
import Torpedo 1.0
=======
>>>>>>> origin/refactoring_branch
import CQML 1.0 as C

Rectangle{
    color: "transparent"
    //width: leftSideRect.width
    //height: leftSideRect.height

    ColumnLayout{
        anchors.fill: parent
        spacing: 0
        Text {
            text: "Type: " + modelData.type
            color: "#6ab2f2"
        }
        Text {
            text: "Name: " + modelData.objectName
            color: "#6ab2f2"
        }
        Text {
            property int dad: 1
            text: "Velocity: " + modelData.velocity.toPrecision(5)
            color: "#6ab2f2"
        }
        Text {
            text: "Rotation: " + modelData.rotation.toPrecision(5)
            color: "#6ab2f2"
        }
        Text {
            text: "X: " + modelData.x.toPrecision(5)
            color: "#6ab2f2"
        }
        Text {
            text: "Y: " + modelData.y.toPrecision(5)
            color: "#6ab2f2"
        }
        Text {
            text: "Z: " + modelData.z.toPrecision(5)
            color: "#6ab2f2"
        }

        ListView {
            id : stagesRepeater
//            width: .width
//            height: mainItem.height
            Layout.fillWidth: true
            Layout.preferredHeight: 400
            property int currentRepeaterIndex: 0
//            delegate: processDelegate
            model : modelData.stages


            delegate: Text {
                property bool active: stagesRepeater.currentRepeaterIndex === index
                anchors.left: parent.left
                anchors.right: parent.right
                height: 30
                wrapMode: Text.WordWrap
                text: "Stages:" + "\n" + (index+1) + "    " + modelData
                verticalAlignment: Text.AlignVCenter
                color: active ? "red" : "#6ab2f2"

            }


        }

        Connections {
            target: modelData
            function onStagesChanged(newStage) {
                stagesRepeater.currentRepeaterIndex = newStage.length-1;
            }

        }



        C.Button {
            id: buttonRemove
            text: "Remove"
            Layout.fillWidth: true
            Layout.leftMargin: 4
            Layout.rightMargin: 4
            Layout.bottomMargin: 4
            buttonRadius: 3
            buttonBackground: "#6ab2f2"
            opacity: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            enabled: visible
            onReleased: {
                sceneArea.scene.removeItem(modelData)
                sceneArea.currentItem = sceneArea.scene
            }
        }

        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
