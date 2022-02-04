import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import "../../scene"
import KLib 1.0

Slider{
    id: slider

    property string knobLabel: slider.value

    property int knobDiameter: slider.hasOwnProperty("knobDiameter")
                               ? 32 : 32

    property var knobVisible: () => { return false }

    property Component knob : Item {
        implicitHeight: slider.pressed || knobVisible()
                        ? slider.knobDiameter : 0
        implicitWidth: slider.pressed || knobVisible()
                       ? slider.knobDiameter : 0

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
            text: slider.knobLabel
            fontSizeMode: Text.Fit
            font.pixelSize: slider.knobDiameter - 19
            minimumPixelSize: 6
            wrapMode: Text.WordWrap
            color: "green"
            opacity: slider.pressed || knobVisible() ? 1 : 0
            z: 1

            property color styleColor: "green"

            Behavior on opacity {
                NumberAnimation { duration: 200}
            }
        }

        Rectangle {
            id: roundKnob
            implicitHeight: parent.height
            implicitWidth: parent.width
            radius: implicitWidth / 2
            //color: style.color
            antialiasing: true
            border.color: "black"
            border.width: 1
            clip: true

            Rectangle {
                id: lowKnob
                implicitHeight: parent.height / 2 + 3
                implicitWidth: parent.width / 2 + 3

                //color: style.color
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                antialiasing: true
                Shape
                {
                    ShapePath{
                        fillColor: "transparent"
                        strokeColor: "black"
                        strokeWidth: 3
                        startX: 0
                        startY: lowKnob.height
                        PathLine { x: lowKnob.width; y: lowKnob.height }
                        PathLine { x: lowKnob.width; y: 0 }
                    }
                }

            }

            transform: [
                Rotation {
                    origin { x: parent.width / 2; y: parent.height / 2 }
                    angle: 45;
                }
            ]
        }

        Behavior on implicitHeight {
            NumberAnimation { duration: 200}
        }

        Behavior on implicitWidth {
            NumberAnimation { duration: 200}
        }
    }

    handle: Item {
    //anchors.centerIn: parent
    implicitHeight: 8
    implicitWidth: 8
    x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
    y: slider.topPadding +  slider.availableHeight / 2 - height / 2

    Loader {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.top
        anchors.bottomMargin: 16
        sourceComponent: slider.knob
    }

    Rectangle {
        anchors.centerIn: parent
        implicitHeight: parent.implicitHeight + 3
        implicitWidth: parent.implicitWidth + 3
        color: slider.pressed ?
                   "green" :
                   "transparent"
        radius: implicitHeight / 2
        Rectangle {
            property var diameter: slider.enabled ? 16 : 12
            anchors.centerIn: parent

            border.width: 2

            implicitHeight: slider.pressed && !slider.focus ?
                                diameter + 8 :
                                diameter

            implicitWidth: slider.pressed && !slider.focus ?
                               diameter + 8  :
                               diameter

            radius: implicitWidth / 2

            Behavior on implicitHeight {
                NumberAnimation { duration: 200 }
            }

            Behavior on implicitWidth {
                NumberAnimation { duration: 200 }
            }
        }
    }
    }
}
