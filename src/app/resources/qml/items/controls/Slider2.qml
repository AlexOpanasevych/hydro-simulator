import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

Slider{
    id: control

    hoverEnabled: true
    property string knobLabel: control.value.toFixed(2)
    property color lineColor: "green"
    property color loadedlineColor: "white"

    property QtObject palettes: QtObject {
        property color backgroundFill: "gray"
        property color loadedFill: Qt.lighter("green")
        property color valueFill: "#2f6ea5"
        property color handle: "#6ab2f2"
        property color text: "black"
    }

    property int knobDiameter: 32
    property real loadedValue

    property var knobVisible: () => { return (control.pressed || mouseArea.containsMouse) }
    property var handleVisible:() => { return (control.pressed || mouseArea.containsMouse) }
    property bool areaHovered: false

    onValueChanged: control.knobLabel = value.toFixed(2)
    property Component knob : Item {
        implicitHeight: control.pressed || knobVisible()
                        ? control.knobDiameter : 0
        implicitWidth: control.pressed || knobVisible()
                       ? control.knobDiameter : 0
        visible: implicitHeight !== 0
        Label {
            anchors {
                fill: parent
                topMargin: 4
                bottomMargin: 2
                leftMargin: 4
                rightMargin: 4
            }

            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            text: control.knobLabel
            fontSizeMode: Text.Fit
            font.pixelSize: control.knobDiameter - 19
            minimumPixelSize: 14
            color: palettes.text
            opacity: control.pressed || knobVisible() ? 1 : 0
            z: 1
        }
    }

    handle: Item {
        implicitHeight: 8
        implicitWidth: 2
        x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
        y: control.topPadding +  control.availableHeight / 2 - height / 2
        opacity: handleVisible() ? 1 : 0

        Behavior on opacity { NumberAnimation { duration : 200 } }

        Rectangle {
            anchors.centerIn: parent
            implicitHeight: parent.implicitHeight + 3
            implicitWidth: parent.implicitWidth + 3
            color: control.pressed || handleVisible() ?
                       palettes.handle :
                       "transparent"
            radius: implicitHeight / 2
        }
    }

    background: Rectangle {
        x: control.leftPadding
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: 200
        implicitHeight: 5
        width: control.availableWidth
        height: implicitHeight + ( control.knobVisible() ? 2 : 0 )
        radius: 2
        color: palettes.backgroundFill

        Rectangle
        {
            width: (loadedValue * control.availableWidth / control.to)
            height: parent.height
            color: palettes.loadedFill
            radius: 2
        }

        Rectangle {
            width: control.visualPosition * parent.width
            height: parent.height
            color: palettes.valueFill
            radius: 2
        }


        Loader {
            id: loader
            anchors.bottom: parent.top
            anchors.bottomMargin: 1
            sourceComponent: control.knob

            Binding{
                target: loader.item
                property: "x"
                value: control.pressed ?
                           handle.x - control.knobDiameter / 2 - 5:
                           mouseArea.textX - control.knobDiameter / 2 + 1
            }
        }

        MouseArea{
            id: mouseArea
            property real textX: 0

            hoverEnabled: true
            anchors.fill: parent
            anchors.topMargin: -3
            anchors.bottomMargin: -3
            preventStealing: true
            acceptedButtons: Qt.NoButton
            onMouseXChanged: { if (hovered) { textX = mouseX } }
            onMouseYChanged: { if (hovered) { textX = mouseX } }

            Binding{
                target: mouseArea
                property: "textX"
                value: handle.x
            }

            onTextXChanged: {
                control.knobLabel = value.toFixed(2)
            }
        }
    }
}
