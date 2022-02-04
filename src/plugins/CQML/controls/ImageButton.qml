import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0
import CQML 1.0 as C
import "../base/"

Button{
    id: button
    text: ""
    property color textColor: "black"
    property color buttonBackground: "white"
    readonly property real mult: 1
    property alias iconCode: label.source
    property alias description: inner.text
    property bool currentChecked: false
    property alias descriptionSize: inner.font.pixelSize
    property int buttonRadius: 0
    property int elevation: 1
    property bool internalShadow: false

    background: C.BaseBackground{
        id: background
        anchors.fill: parent

        implicitHeight: 36 * mult


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

        C.WavedMouseArea{
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
        Image {
            id: label
            source: "qrc:/resources/icons/submarineBold.svg"
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: inner.paintedHeight * 0.33
            sourceSize.width: button.width / 2
            sourceSize.height: button.height / 2
        }

        ColorOverlay {
            anchors.fill: label
            source: label
            color: button.textColor
        }

        Text{
            id: inner
            anchors.top: label.bottom
            anchors.horizontalCenter: label.horizontalCenter
            width: button.width
            font.pixelSize: button.height / 5 - 1
            wrapMode: Text.WordWrap
            color: button.enabled ? button.hasOwnProperty("textColor")
                                    ? button.textColor : Qt.rgba(1, 1, 1, 0.30)
            : Qt.rgba(0, 0, 0, 0.26)
            horizontalAlignment: Text.AlignHCenter
        }
    }
}

