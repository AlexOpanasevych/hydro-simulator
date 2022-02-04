import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0
import "../base/"

Button{
    id: button
    text: ""
    property color textColor: "black"
    property color buttonBackground: "white"
    readonly property real mult: 1
    property alias iconCode: label.text
    property alias description: inner.text
    property bool currentChecked: false
    property alias descriptionSize: inner.font.pixelSize
    property int buttonRadius: 0
    property int elevation: 1
    property bool internalShadow: false

    background: BaseBackground{
        id: background
        anchors.fill: parent
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
                function onClicked(mouse) { button.onClicked(mouse) }
            }
        }
    }

    contentItem: Item{
        anchors.centerIn: parent
        anchors.fill: button
        Text{
            id: label
            //anchors.centerIn: parent
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: button.height / 2 - font.pixelSize / 2 - (inner.text === "" ? 1 : inner.height / 2)
            font.family: button.font.family
            font.pixelSize: button.height / 2
            wrapMode: Text.WordWrap
            color: button.enabled ? button.hasOwnProperty("textColor")
                                    ? button.textColor : Qt.rgba(1, 1, 1, 0.30)
            : Qt.rgba(0, 0, 0, 0.26)
            horizontalAlignment: Text.AlignHCenter
        }

        Text{
            id: inner
            visible: text !== ""
            anchors.top: label.bottom
            anchors.horizontalCenter: label.horizontalCenter
            font.pixelSize: button.height / 5 - 1
            wrapMode: Text.WordWrap
            color: button.enabled ? button.hasOwnProperty("textColor")
                                    ? button.textColor : Qt.rgba(1, 1, 1, 0.30)
            : Qt.rgba(0, 0, 0, 0.26)
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
