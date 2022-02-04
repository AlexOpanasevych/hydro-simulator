import QtQuick 2.15
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import KLib 1.0
import CQML 1.0 as C

Item{
    id: playerItem
    required property KScenePlayer player
    property real offset: 0
    property bool noHide: false
    height: 40
    signal start()

    //onOffsetChanged: console.log(offset)
    //color: Qt.rgba(14, 93, 153, 0.6)

    Connections{
        target: player
        function onCurrentTimeChanged(time)
        {
            timeSlider.value = time
        }
    }

    Rectangle{
        anchors.fill: rightPart
        anchors.leftMargin: 220
        color: Qt.rgba(14, 93, 153, 0.6)
    }

    Rectangle {
        id: playground
        property real offset: 0
        property real subpanelHeight: 225
        anchors.top: parent.top
        anchors.topMargin: -offset
        width: 220
        height: parent.height + subpanelHeight
        color: Qt.rgba(14, 93, 153, 0.6)
        //onStateChanged: console.log(state)
        //onOffsetChanged: console.log(offset)
        ColumnLayout{
            anchors.fill: parent
            RowLayout{
                id: leftPart
                Layout.preferredHeight: playerItem.height
                Layout.maximumHeight: playerItem.height
                Layout.fillWidth: true
                spacing: 0
                C.IconButton{
                    Layout.fillHeight: true
                    Layout.preferredWidth: height
                    iconCode: C.IconHelper.icon["play"]
                    textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
                    buttonBackground: "#30384f"
                    font.family: currentChecked ? C.IconHelper.iconsPressed.name : C.IconHelper.icons.name
                    currentChecked: player.mode === KScenePlayer.PLAY
                    onPressed: {
                        //start()
                        show()
                    }
                }

                C.IconButton{
                    Layout.fillHeight: true
                    Layout.preferredWidth: height
                    iconCode: C.IconHelper.icon["pause"]
                    textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
                    buttonBackground: "#30384f"
                    font.family: currentChecked ? C.IconHelper.iconsPressed.name : C.IconHelper.icons.name
                    currentChecked: player.mode === KScenePlayer.PAUSE
                    onPressed: {
                        player.pause()
                    }
                    Connections {
                        target: sceneArea.scene.scenePlayer
                        function onModeChanged(mode) {
                            if(mode == sceneArea.scene.scenePlayer.PAUSE) {
                                player.pause()
                            }
                        }
                    }
                }

                C.IconButton{
                    Layout.fillHeight: true
                    Layout.preferredWidth: height
                    iconCode: C.IconHelper.icon["stop"]
                    textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
                    buttonBackground: "#30384f"
                    font.family: currentChecked ? C.IconHelper.iconsPressed.name : C.IconHelper.icons.name
                    currentChecked: player.mode === KScenePlayer.STOP

                    onPressed: {
                        sceneArea.currentItem = sceneArea.scene
                        player.stop()
                    }
                }

                Item{
                    id: time
                    Layout.fillHeight: true
                    Layout.preferredWidth: 100
                    Label{
                        anchors.centerIn: parent
                        text: {
                            let min = Math.floor(player.currentTime/ 60)
                            let sec = (player.currentTime % 60).toFixed(0)
                            let min2 = Math.floor(player.maxTime / 60)
                            let sec2 = (player.maxTime % 60).toFixed(0)
                            let result = ""


                            result += (min < 10 ? "0" : "") + min + ":"
                            result += (sec < 10 ? "0" : "") + sec + "/"
                            result += (min2 < 10 ? "0" : "") + min2 + ":"
                            result += (sec2 < 10 ? "0" : "") + sec2
                            return result
                        }
                    }
                }
            }

            Rectangle{
                id: background
                Layout.fillHeight: true
                Layout.fillWidth: true
                width: parent.width
                color: "#1f2936"

                ColumnLayout{
                    anchors.fill: parent
                    spacing: 0

                    Text {
                        id: typeText
                        Layout.fillWidth: true
                        Layout.minimumHeight: 40

                        color: "#6ab3f3"
                        text: "SIMULATION PROPERTIES"
                        font.pixelSize: 17
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap

                        Shape{
                            Layout.fillWidth: true
                            Layout.preferredHeight: 5
                            ShapePath {
                                fillColor: "transparent"
                                strokeColor: "#7f8c92"
                                strokeWidth: 1
                                startX: 0
                                startY: typeText.height - 1
                                PathLine { x: typeText.width; y: typeText.height - 1 }
                            }

                        }

                        Rectangle{
                            color: "white"
                            anchors.fill: typeText
                            opacity: 0.1
                        }

                    }

                    Text{
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40
                        Layout.leftMargin: 3
                        color: "#d4e0ea"
                        text: "Time: " + maxTime.knobLabelText(maxTime.value)
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 17
                    }

                    C.PlayerSlider{
                        id: maxTime
                        Layout.preferredHeight: 30
                        Layout.fillWidth: true
                        from: 0
                        to: 1000
                        stepSize: 1
                        value: 0
                        knobVisible: () => { return maxTime.pressed }
                        palettes: QtObject {
                            property color backgroundFill: "gray"
                            property color loadedFill: Qt.lighter("green")
                            property color valueFill: "#2f6ea5"
                            property color handle: "#6ab2f2"
                            property color text: "#2f6ea5"
                        }
                        handleVisible: () => { return true }

                        //onValueChanged: scene.scenePlayer.maxTime = value
                    }

                    Text{
                        id: textSpeed
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40
                        Layout.leftMargin: 3
                        color: "#d4e0ea"
                        text: "Speed: " + speed.value
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 17
                    }

                    C.PlayerSlider{
                        id: speed
                        Layout.preferredHeight: 30
                        Layout.fillWidth: true
                        from: 0.25
                        to: 4
                        stepSize: 0.25
                        value: 1
                        knobVisible: () => { return speed.pressed }
                        handleVisible: () => { return true }
                        knobLabelText: (value) => { return value }

                        palettes: QtObject{
                            property color backgroundFill: "gray"
                            property color loadedFill: Qt.lighter("green")
                            property color valueFill: "green"
                            property color handle: Qt.darker("green")
                            property color text: "green"
                        }
                    }

                    RowLayout{
                        Layout.fillWidth: true
                        Layout.preferredHeight: 50

                        C.Button {
                            id: buttonCancel
                            text: "CANCEL"
                            textColor: "#1f2936"
                            buttonBackground: "#6ab2f2"
                            Layout.fillWidth: true
                            Layout.leftMargin: 4
                            Layout.rightMargin: 4
                            Layout.bottomMargin: 4
                            buttonRadius: 3
                            Layout.fillHeight: true
                            Layout.maximumHeight: 30
                            onPressed: {
                                hide()
                                //sceneArea.scene.removeAllItems(KScene.SHIP)
                                //sceneArea.currentItem = scene
                            }
                        }
                        C.Button {
                            id: buttonAccept
                            text: "ACCEPT"
                            textColor: "#1f2936"
                            buttonBackground: "#6ab2f2"
                            Layout.fillWidth: true
                            Layout.leftMargin: 4
                            Layout.rightMargin: 4
                            Layout.bottomMargin: 4
                            Layout.fillHeight: true
                            buttonRadius: 3
                            Layout.maximumHeight: 30
                            onPressed: {
                                scene.scenePlayer.speed = speed.value
                                scene.scenePlayer.maxTime = maxTime.value
                                scene.scenePlayer.start()

                                hide()
                            }
                        }
                    }

                    Item{
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }
        }

        state: "hide"

        states: [
            State {
                name: "show"
                PropertyChanges {
                    target: playground
                }
            },
            State {
                name: "hide"
                PropertyChanges {
                    target: playground
                }
            }]

        transitions: [
            Transition {
                from: "*"
                to: "show"

                NumberAnimation {
                    target: playground
                    property: "offset"
                    duration: 300
                    to: playground.subpanelHeight
                    easing.type: Easing.OutQuint
                }
            },
            Transition {
                from: "*"
                to: "hide"
                NumberAnimation {
                    target: playground
                    property: "offset"
                    duration: 300
                    to: 0
                    easing.type: Easing.OutQuint
                }

            }
        ]

    }

    RowLayout{
        id: rightPart
        anchors.fill: parent

        spacing: 0

        Item {
            Layout.preferredWidth: 220
        }



        Item{
            Layout.fillWidth: true
            Layout.fillHeight: true

            C.PlayerSlider{
                id: timeSlider
                anchors.fill: parent
                from: 0
                to: 1000
                stepSize: 1
                enabled: player.mode !== KScenePlayer.PLAY

                //value: player.currentTime
                handleVisible: () => { return true }
            }

        }

        C.IconButton{
            id: glueButton
            Layout.fillHeight: true
            Layout.preferredWidth: height
            iconCode: C.IconHelper.icon["thumbtag"]
            textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
            buttonBackground: "#30384f"
            font.family: currentChecked ? C.IconHelper.iconsPressed.name : C.IconHelper.icons.name
            currentChecked: noHide

            onPressed: {
                noHide = !noHide
            }
        }
    }

    Timer{
        id: timer
        running: false
        interval: 1500
        repeat: false
        onTriggered: if (!noHide) close()
    }

    MouseArea{
        anchors.fill: parent
        anchors.bottomMargin: parent.height * 0.75
        anchors.topMargin: -10
        hoverEnabled: true
        preventStealing: true
        acceptedButtons: Qt.NoButton
        property real prevY: 0

        onContainsMouseChanged: {
            if (containsMouse)
            {
                //console.log("contains")
                if (mouseY > prevY)
                    timer.stop()
            }
            else
            {
                //console.log("no contains")
                if (mouseY < prevY && !noHide || prevY === 0)
                    timer.restart()
            }
            prevY = mouseY
        }
    }

    state: "show"

    //onStateChanged: console.log(state)

    states: [
        State {
            name: "show"
            PropertyChanges {
                target: playerItem
            }
        },
        State {
            name: "hide"
            PropertyChanges {
                target: playerItem
            }
        }]

    transitions: [
        Transition {
            from: "hide"
            to: "show"

            NumberAnimation {
                target: playerItem
                property: "offset"
                duration: 300
                from: playerItem.height
                to: 0
                easing.type: Easing.OutQuint
            }
        },
        Transition {
            from: "show"
            to: "hide"
            NumberAnimation {
                target: playerItem
                property: "offset"
                duration: 300
                from: 0
                to: playerItem.height
                easing.type: Easing.OutQuint
            }

        }
    ]

    function open()
    {
        playerItem.state = "show"
    }

    function close()
    {
        playerItem.state = "hide"
    }

    function show()
    {
        if (player.currentTime === 0)
        {
            playground.state = "show"
            playerItem.noHide = true
            glueButton.enabled = false
        }
        else
        {
            player.start()
        }
    }

    function hide()
    {
        playground.state = "hide"
        playerItem.noHide = false
        glueButton.enabled = true
    }

    function isOpened()
    {
        return playerItem.state === "show"
    }
}

