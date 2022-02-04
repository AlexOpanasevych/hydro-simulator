import QtQuick 2.0
import QtQuick.Controls 2.0
import KLib 1.0

Item {
    id: root
    property var modelData;

    x : modelData.x
    y : modelData.y
    height : modelData.height
    width : modelData.width
    property string description: ""
    property color enabledColor: modelData !== undefined && modelData.additionalQml !== undefined && modelData.additionalQml.enabledColor !== undefined ? modelData.additionalQml.enabledColor : "#FFC273"
    property color disabledColor: modelData !== undefined && modelData.additionalQml !== undefined && modelData.additionalQml.disabledColor !== undefined ? modelData.additionalQml.disabledColor : "#eeeeee"
    property variant caseCaptions: modelData !== undefined && modelData.additionalQml !== undefined ? modelData.additionalQml.names : []
    property int spacing: modelData !== undefined && modelData.additionalQml !== undefined ? modelData.additionalQml.spacing : 16
    property int currentIndex: /*modelData.value !== undefined ? modelData.value : */-1
    property double buttonWidth: modelData !== undefined ? modelData.width : 100
    property double buttonHeight: modelData !== undefined ? modelData.width : 100

    onCurrentIndexChanged: {
        console.log("dadadadaadada", JSON.stringify(modelData.value))
    }

    Component.onCompleted: {
        if (root.description === "")
        {
            for (var i = 0; i < caseCaptions.length; i++)
            {
                description += caseCaptions[i].replace('\n',' ')
                if (i !== root.caseCaptions.length - 1)
                {
                    description += ", "
                }
            }
        }
    }

    ManualMessage {
        messageText: root.description
        target: parent
        radius: 5
        font.family: "Times New Roman"
        backgroundColor: "#565656"
        textColor: "white"
    }

    Row {
        spacing: root.spacing
        Repeater {
            id: buttonRepeater
            model: caseCaptions.length
            MGKButton {
                enabledColor: root.enabledColor
                disabledColor: root.disabledColor
                id: button
                width: buttonWidth
                height: buttonHeight
                caption: caseCaptions[index]
                inButtonGroup: true
                ledOn: currentIndex == index
                onPressedChanged: {
                    if(pressed) {
                        if(index !== currentIndex)
                            root.modelData.requestValue = index
                        else {
                            if(ledMode.currentMode === ledMode.tickling) {
                                ledMode.currentMode = ""
                                ledTimer.stop()
                            }
                            root.modelData.requestValue = -1
                        }
                        App.client.sCommunicator.requestValueChanged(root.modelData.requestValue, root.modelData)
                    }
                    else {console.log("released in button group", root.enabledColor, root.disabledColor)}
                }
            }
        }
    }

    function updateHScale(scale) {
        y *= scale
        height *= scale
        for(let i = 0; i < caseCaptions.length; i++) {
            buttonRepeater.itemAt(i).y *= scale
            buttonRepeater.itemAt(i).height *= scale

        }
    }

    function updateWScale(scale) {
        x *= scale
        width *= scale
        spacing *= scale
        for(let i = 0; i < caseCaptions.length; i++) {
            buttonRepeater.itemAt(i).x *= scale
            buttonRepeater.itemAt(i).width *= scale
        }
    }

    Connections {
        target: modelData !== undefined ? modelData : null
        function onValueChanged(value) {
            currentIndex = value.index !== undefined ? value.index : value
            if(value.ledMode !== undefined)
                buttonRepeater.itemAt(currentIndex).ledMode.currentMode = value.ledMode
        }
    }

}

