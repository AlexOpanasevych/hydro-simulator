import QtQuick 2.15
import QtQuick.Layouts 1.15
import Trajectory 1.0
import Torpedo 1.0
import CQML 1.0 as C
import QtQuick.Controls 2.15

Rectangle{
    color: "transparent"
    //width: leftSideRect.width
    //height: leftSideRect.height

    MouseArea{
        anchors.fill: parent
        onWheel: {}
        preventStealing: true
    }

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

        Item {
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            visible: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            enabled: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            ColumnLayout {
                anchors.fill: parent
                anchors.topMargin: 10
//                enabled: parent.enabled

                Label {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    text: "Rotation"
                    color: "white"
                    font.pointSize: 11
                }

                C.PlayerSlider{
                    id: rotateSlider
//                    enabled: parent.enabled
                    Layout.fillHeight: true
                    Layout.fillWidth: true
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
    //                    if (root.trajectory){
    //                        if (pressed){
    //                            root.trajectory.opacity = 0
    //                        }else{
    //                            root.trajectory.updateProperties()
    //                            root.trajectory.opacity = 1
    //                            root.trajectory.selectTrajectory(textArea.cursorPosition)
    //                        }
    //                    }
                    }

                    onValueChanged: modelData.rotation = value

                    Component.onCompleted: value = modelData.rotation
                }
            }
        }

        ButtonGroup {
            buttons: torpDistChoose.children
        }
        RowLayout {
            id: torpDistChoose
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            visible: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            enabled: scene.scenePlayer.mode === 2 // KScenePlayer.STOP


            C.CheckBox {
                id: torpDistCheckYes
                Layout.maximumHeight: 20

                Layout.fillWidth: true

                font.pointSize: 12
                text: "Ввімкнути дистанцію розходження"
                textColor: "white"
                checked: true
                onCheckedChanged: {
                    if(checked)
                        modelData.firstRotateDistance = 250
                }
                Component.onCompleted: {
                    if(modelData.firstRotateDistance === 250) {
                        checked = true
                    }
                }
            }

            C.CheckBox {
                id: torpDistCheckNo
                text: "Вимкнути"
                Layout.maximumHeight: 20
                Layout.fillWidth: true
                font.pointSize: 12
                textColor: "white"
                onCheckedChanged: {
                    if(checked)
                        modelData.firstRotateDistance = 0
                }
                Component.onCompleted: {
                    if(modelData.firstRotateDistance === 0) {
                        checked = true
                    }
                }
            }
        }

        Item {
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            visible: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            enabled: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            ColumnLayout {
                anchors.fill: parent
//                anchors.topMargin: 10
                Label {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    text: "First Rotation Angle"
                    color: "white"
                    font.pointSize: 11
                }

                C.PlayerSlider{
                    id: firstRaSlider

                    Layout.fillHeight: true
                    Layout.fillWidth: true
    //                enabled: parent.enabled
                    from: -170
                    to: 170
                    stepSize: 5
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
    //                    if (root.trajectory){
    //                        if (pressed){
    //                            root.trajectory.opacity = 0
    //                        }else{
    //                            root.trajectory.updateProperties()
    //                            root.trajectory.opacity = 1
    //                            root.trajectory.selectTrajectory(textArea.cursorPosition)
    //                        }
    //                    }
                    }

                    onValueChanged: modelData.firstRa = value

                    Component.onCompleted: value = modelData.firstRa
                }

            }

        }

        Item {
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            visible: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            enabled: scene.scenePlayer.mode === 2 // KScenePlayer.STOP

            ColumnLayout {
                anchors.fill: parent
//                anchors.topMargin: 10

                Label {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    text: "Second Rotation Angle"
                    color: "white"
                    font.pointSize: 11
                }

                C.PlayerSlider{
                    id: secondRaSlider
                    Layout.fillHeight: true
                    Layout.fillWidth: true
//                    enabled: parent.enabled
                    from: -170
                    to: 170
                    stepSize: 10
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
    //                    if (root.trajectory){
    //                        if (pressed){
    //                            root.trajectory.opacity = 0
    //                        }else{
    //                            root.trajectory.updateProperties()
    //                            root.trajectory.opacity = 1
    //                            root.trajectory.selectTrajectory(textArea.cursorPosition)
    //                        }
    //                    }
                    }

                    onValueChanged: modelData.secondRa = value

                    Component.onCompleted: value = modelData.secondRa
                }
            }

        }

        Item {
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            visible: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            enabled: scene.scenePlayer.mode === 2 // KScenePlayer.STOP

            ColumnLayout {
                anchors.fill: parent
//                anchors.topMargin: 10

                Label {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    text: "Torpedo Divergency"
                    color: "white"
                    font.pointSize: 11
                }

                C.PlayerSlider{
                    id: torpDivSlider

                    property int currentBoard: 0
                    Layout.fillHeight: true
                    Layout.fillWidth: true
//                    enabled: parent.enabled
                    from: 0
                    to: 28000
                    stepSize: 1000
                    value: 15000
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
    //                    if (root.trajectory){
    //                        if (pressed){
    //                            root.trajectory.opacity = 0
    //                        }else{
    //                            root.trajectory.updateProperties()
    //                            root.trajectory.opacity = 1
    //                            root.trajectory.selectTrajectory(textArea.cursorPosition)
    //                        }
    //                    }
                    }

                    onValueChanged: modelData.torpedoDivergency = value

                    Component.onCompleted: value = modelData.torpedoDivergency

                    Connections {
                        target: launchCountGroup

                        function onClicked(button) {
                            torpDivSlider.currentBoard = Array.prototype.indexOf.call(launchCountGroup.buttons, button)
                            switch(torpDivSlider.currentBoard) {
                            case 0:
                                torpDivSlider.from = 0
                                break;
                            default:
                                torpDivSlider.from = 500
                                break;
                            }

                        }
                    }
                }

            }

        }

        ButtonGroup {
            buttons: boardChoose.children
        }
        RowLayout {
            id: boardChoose
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            visible: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            enabled: scene.scenePlayer.mode === 2 // KScenePlayer.STOP


            C.CheckBox {
                id: lbCheck
                Layout.maximumHeight: 20

                Layout.fillWidth: true

                font.pointSize: 12
                text: "Лівий борт"
                textColor: "white"
                checked: true
//                onCheckedChanged: {
//                    rbCheck.checked = false
//                }
            }

            C.CheckBox {
                id: rbCheck
                text: "Правий борт"
                Layout.maximumHeight: 20
                Layout.fillWidth: true
                font.pointSize: 12
                textColor: "white"
//                onCheckedChanged: {
//                    lbCheck.checked = false
//                }
            }
        }

        Item {
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            visible: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            enabled: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            C.PlayerSlider{
                id: boardSlider
                anchors.fill: parent
                anchors.topMargin: 10

                enabled: parent.enabled
                from: 0
                to: 170
                stepSize: 10
                value: 15000
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
//                    if (root.trajectory){
//                        if (pressed){
//                            root.trajectory.opacity = 0
//                        }else{
//                            root.trajectory.updateProperties()
//                            root.trajectory.opacity = 1
//                            root.trajectory.selectTrajectory(textArea.cursorPosition)
//                        }
//                    }
                }

//                onValueChanged: modelData.rotation = value

//                Component.onCompleted: value = modelData.rotation
            }
        }

        ButtonGroup {
            id: launchCountGroup
            buttons: launchParams.children
        }

        ColumnLayout {
            id: launchParams
            Layout.fillWidth: true
            Layout.preferredHeight: 100

            visible: scene.scenePlayer.mode === 2 // KScenePlayer.STOP
            enabled: scene.scenePlayer.mode === 2 // KScenePlayer.STOP

            RadioButton {
                checked: true
                Text {
                    anchors.left: parent.right
                                        anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("одиночний залп")
                    color: "white"
                }
            }

            RadioButton {
                Text {
                                        anchors.left: parent.right
                                        anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("двоторпедний залп")
                    color: "white"

                }
            }

            RadioButton {
                Text {
                                        anchors.left: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("триторпедний залп")
                    color: "white"

                }
            }

            RadioButton {
                Text {
                                        anchors.left: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("чотириторпедний залп")
                    color: "white"

                }
            }
        }

        Label {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            text: "Stages:"
            color: "white"
            font.family: "Arial"
            font.pointSize: 14
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
                    //@disable-check M325
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
//    Connections {
//        target: scene.scenePlayer
//        function onModeChanged(mode) {
//            if(mode === 0) {
//                modelData.sendCreateMsg({"FRA":firstRaSlider.value, "SRA": secondRaSlider.value, "board":torpDivSlider.currentBoard,
//                                        "boardAngle":boardSlider.value})
//            }
//        }
//    }
}
