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
    property alias iconCode: label.source
    property alias description: inner.text
    property alias currentChecked: mask.visible
    property alias descriptionSize: inner.font.pixelSize

    background: BaseBackground{
        id: background
        anchors.fill: parent

        implicitHeight: 36 * mult

        //radius: 2

        backgroundColor: button.enabled
                         ? buttonBackground
                         : Qt.rgba(0, 0, 0, 0.12)

        elevation: {
            var elevation = 1

            if (elevation > 0 && (button.focus || wavedArea.currentCircle))
                elevation++;

            if(!button.enabled)
                elevation = 0;

            return elevation;
        }

        tintColor: wavedArea.currentCircle || button.focus || button.hovered
                   ? Qt.rgba(0,0,0, wavedArea.currentCircle ? 0.1
                                                            : elevation > 0 ? 0.03
                                                                            : 0.05)
                   : "transparent"

        Rectangle {
            id: mask
            anchors.fill: parent
            //maskSource: background
            opacity: 0.2
            visible: false
            color: "#576673"
        }


        WavedMouseArea{
            id: wavedArea
            anchors.fill: parent

            Connections{
                target: wavedArea
                function onPressed(mouse) { button.onPressed(mouse)}
                function onCanceled(mouse) { button.onCanceled() }
                function onReleased(mouse) { button.onReleased(mouse) }
                function onClicked(mouse) { button.onClicked(mouse) }
            }
        }
    }

    contentItem: Item{
        anchors.centerIn: parent
        //implicitHeight: Math.max(36 * mult, label.height + inner.height + 16 * mult)
        //implicitWidth:  button.width
        //clip: true

        Image {
            id: label
            source: "qrc:/resources/icons/submarineBold.svg"
            anchors.centerIn: parent
            anchors.topMargin: parent.height / 3
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

