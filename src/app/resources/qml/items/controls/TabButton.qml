import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0
import "../base"

import IconHelper 1.0

TabButton{
    id: button
    text: ""
    property color textColor: "black"
    property color buttonBackground: "white"
    readonly property real mult: 1
    property alias currentChecked: mask.visible
    implicitHeight: 35
    onHeightChanged: console.log("height", height)

    background: BaseBackground{
        id: background
        anchors.fill: button

        backgroundColor: button.enabled
                         ? buttonBackground
                         : Qt.rgba(0, 0, 0, 0.12)

        elevation: 0

        tintColor: wavedArea.currentCircle || button.focus || button.hovered
                   ? Qt.rgba(0,0,0, wavedArea.currentCircle ? 0.1
                                                            : elevation > 0 ? 0.03
                                                                            : 0.05)
                   : "transparent"

        Rectangle {
            id: mask
            anchors.fill: background
            opacity: 0.2
            visible: false
            color: "#576673"
        }

        WavedMouseArea{
            id: wavedArea
            anchors.fill: background

            Connections{
                target: wavedArea
                function onPressed(mouse) { button.onPressed(mouse)}
                function onCanceled(mouse) { button.onCanceled() }
                function onReleased(mouse) { button.onReleased(mouse) }
            }
        }
    }

    contentItem: Item{
        Text{
            id: label
            anchors.fill: parent
            font.family: button.font.family
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: button.font.hasOwnProperty("pixelSize") ? button.font.pixelSize : 17
            text: button.text
            wrapMode: Text.WordWrap
            color: button.enabled ? button.hasOwnProperty("textColor")
                                    ? button.textColor : Qt.rgba(1, 1, 1, 0.30)
            : Qt.rgba(0, 0, 0, 0.26)
            horizontalAlignment: Text.AlignHCenter
        }
    }
}

