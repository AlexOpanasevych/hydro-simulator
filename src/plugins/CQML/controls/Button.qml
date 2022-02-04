import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0
import "../base"

//@disable-check M129
Button{
    id: button
    text: ""
    property color textColor: "black"
    property color buttonBackground: "white"
    readonly property real mult: 1
    property bool currentChecked: false
    property int buttonRadius: 0
    property int elevation: 1
    property bool internalShadow: false

    Behavior on height { NumberAnimation {duration: 200 } }

    background: BaseBackground{
        id: background
        radius: buttonRadius
        backgroundColor: button.enabled
                         ? buttonBackground
                         : Qt.rgba(0, 0, 0, 0.12)

        elevation: button.elevation

        tintColor: wavedArea.currentCircle || button.focus || button.hovered
                   ? Qt.rgba(0,0,0, wavedArea.currentCircle ? 0.1
                                                            : elevation > 0 ? 0.03
                                                                            : 0.05)
                   : "transparent"

        internalShadow: button.internalShadow

        Rectangle {
            id: mask
            anchors.fill: parent
            radius: buttonRadius
            opacity: 0.2
            visible: button.currentChecked
            color: "#576673"
        }

        WavedMouseArea{
            id: wavedArea
            anchors.fill: parent
            circleRadius: buttonRadius

            Connections{
                target: wavedArea
                function onPressed(mouse) { button.onPressed(mouse)}
                function onCanceled(mouse) { button.onCanceled() }
                function onDoubleClicked() { button.doubleClicked() }
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
            font.pixelSize:  button.height / 2
            text: button.text
            wrapMode: Text.WordWrap
            color: button.enabled ? button.textColor : Qt.rgba(0, 0, 0, 0.26)
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
