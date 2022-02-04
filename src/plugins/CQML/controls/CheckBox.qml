import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as Q

import CQML 1.0 as C

//@disable-check M129
Q.CheckBox{
    id: control
    property color textColor: "black"
    property color indicatorBorderColor: "black"
    property color indicatorRectColor: "green"
    property color indicatorMarkColor: "white"
    property int layout: Qt.LeftToRight
    property bool internalShadow: false
    font.pixelSize: height * 0.7

    contentItem: Item {
        implicitWidth: control.text !== "" ? text.implicitWidth + 2 : 0
        implicitHeight: 10
        baselineOffset: layout === Qt.LeftToRight ? -text.baselineOffset : text.baselineOffset

        Text {
            id: text
            anchors.fill: parent
            anchors.leftMargin: layout === Qt.LeftToRight ?  control.height : 0
            anchors.rightMargin: layout === Qt.LeftToRight ? 0 : control.height
            renderType: Text.QtRendering
            elide: Text.ElideRight
            font : control.font
            color: control.enabled ? textColor : Qt.darker(textColor, 2)
            text: control.text
            verticalAlignment: Text.AlignVCenter
        }
    }

    indicator: Item {
        id: parentRect
        height: control.height
        width: control.height
        x: layout === Qt.LeftToRight ? 0 : control.width - control.height
        implicitWidth: 48
        implicitHeight: implicitWidth

        Rectangle {
            id: indicatorRect

            anchors.centerIn: parent

            property color __internalColor: control.enabled ? control.indicatorRectColor : Qt.darker(control.indicatorRectColor, 2)

            width: parent.width * 0.9
            height: width
            radius: 2

            border.width: 2

            border.color: control.enabled
                          ? control.checked ? "transparent" : control.indicatorBorderColor : Qt.darker(control.indicatorBorderColor, 2)

            color: control.checked ? __internalColor : "transparent"

            Behavior on color {
                ColorAnimation {
                    easing.type: Easing.InOutQuad
                    duration: 200
                }
            }

            Behavior on border.color {
                ColorAnimation {
                    easing.type: Easing.InOutQuad
                    duration: 200
                }
            }

            Item {
                id: container

                anchors.centerIn: indicatorRect

                height: parent.height
                width: parent.width

                opacity: control.checked ? 1 : 0

                property int thickness: 3

                Behavior on opacity {
                    NumberAnimation {
                        easing.type: Easing.InOutQuad
                        duration: 200
                    }
                }

                Rectangle {
                    id: vert

                    anchors {
                        top: parent.top
                        right: parent.right
                        bottom: parent.bottom
                    }

                    radius: 1
                    color: control.indicatorMarkColor
                    width: container.thickness * 2

                }
                Rectangle {
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }

                    radius: 1
                    color: control.indicatorMarkColor
                    height: container.thickness
                }

                transform: [
                    Scale {
                        origin { x: container.width / 2; y: container.height / 2 }
                        xScale: 0.5
                        yScale: 1
                    },
                    Rotation {
                        origin { x: container.width / 2; y: container.height / 2 }
                        angle: 45;
                    },
                    Scale {
                        id: widthScale

                        origin { x: container.width / 2; y: container.height / 2 }
                        xScale: control.checked ? 0.6 : 0.2
                        yScale: control.checked ? 0.6 : 0.2

                        Behavior on xScale {
                            NumberAnimation {
                                easing.type: Easing.InOutQuad
                                duration: 200
                            }
                        }

                        Behavior on yScale {
                            NumberAnimation {
                                easing.type: Easing.InOutQuad
                                duration: 200
                            }
                        }
                    },
                    Translate { y: -(container.height - (container.height * 0.9)) }
                ]
            }
        }
    }
}
