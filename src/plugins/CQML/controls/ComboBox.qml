import QtQuick 2.15
import QtQuick.Controls 2.15 as Q
import QtQuick.Shapes 1.15
import CQML 1.0 as C

//@disable-check M129
Q.ComboBox {
    id: control
    property color textColor: "#6ab3f3"
    property color backgroundColor: "#30384f"
    property color hoveredBackgroundColor: Qt.darker(control.backgroundColor, 1.3)
    property real delegateHeight: control.height
    property color highlightColor: textColor
    property var source
    property int radius: 5
    property bool internalShadow: false
    property int elevation: 0
    font.pixelSize: height * 0.5
    implicitWidth: 50
    enabled: count > 0


    delegate: Item{
        height: control.delegateHeight
        width: control.width
        C.BaseBackground {
            id: bg
            height: parent.height
            width: parent.width - listView.spacing
            anchors.centerIn: parent
            radius: control.radius
            backgroundColor: mouseArea.containsMouse ? control.hoveredBackgroundColor : control.backgroundColor
            Item {
                id: ind
                anchors.left: parent.left
                anchors.leftMargin: parent.height * 0.15
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
                width: childrenRect.width * 2.5
                Item {
                    width: parent.height * 0.2
                    height: width * 0.4 + width
                    anchors.verticalCenter: parent.verticalCenter

                    opacity: control.currentIndex === index

                    property int thickness: 2

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
                        color: control.highlightColor
                        width: container.thickness

                    }
                    Rectangle {
                        anchors {
                            left: parent.left
                            right: parent.right
                            bottom: parent.bottom
                        }

                        radius: 1
                        color: control.highlightColor
                        height: container.thickness
                    }

                    transform: [
                        Rotation {
                            origin { x: container.width / 2; y: container.height / 2 }
                            angle: 45;
                        }
                    ]
                }
            }

            Text {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: ind.width
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: 5
                text: modelData
                color: control.textColor
                font: control.font
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle{
                anchors.fill: parent
                color: "transparent"
                radius: control.radius
                border.color: highlightColor
                border.width: parent.height * 0.04
                antialiasing: true
                visible: control.currentIndex === index
            }

            C.WavedMouseArea{
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                circleRadius: control.radius
                onReleased: {
                    control.currentIndex = index
                    control.popup.close()
                }
            }

        }
    }

    indicator: Item {
        id: parentRect
        height: control.height
        width: control.width

        Item {
            id: container
            anchors.right: parent.right
            anchors.rightMargin: parent.height * 0.2
            anchors.verticalCenter: parent.verticalCenter
            height: width
            width: parent.height * 0.3
            property int thickness: 3
            antialiasing: true

            Behavior on opacity {
                NumberAnimation {
                    easing.type: Easing.InOutQuad
                    duration: 200
                }
            }

            Rectangle {
                anchors {
                    top: parent.top
                    right: parent.right
                    bottom: parent.bottom
                }

                radius: 1
                color: control.highlightColor
                width: container.thickness

            }
            Rectangle {
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }

                radius: 1
                color: control.highlightColor
                height: container.thickness
            }

            transform: [
                Rotation {
                    origin { x: container.width / 2; y: container.height / 2 }
                    angle: 45;
                }
            ]
        }

    }

    contentItem: Item{
        width: control.width
        height: control.height
        Text {
            x: parent.height * 0.07
            width: control.width - control.width * 0.2
            height: parent.height
            text: control.displayText
            font: control.font
            color: control.textColor
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
    }

    background: C.BaseBackground {
        radius: 5
        internalShadow: control.internalShadow
        elevation: control.elevation
        backgroundColor: control.backgroundColor
    }

    popup: Q.Popup {
        width: control.width
        x: control.width / 2 - width / 2
        y: control.source ?
               mapToItem(null, 0 , 0) +  (control.count > 5 ? 5 * control.height : control.count * control.height) + listView.spacing * (control.count > 5 ? 6 : (control.count + 2)) < control.source.height ?
                   control.height + 2 :
                   control.height + 1 - (control.source.height - implicitHeight) :
        control.height + 2
        implicitHeight: contentItem.implicitHeight
        padding: 0

        Behavior on y { NumberAnimation { duration: 200 } }
        enter: Transition {
            ParallelAnimation{
                NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 100 }
                SequentialAnimation{
                    NumberAnimation { property: "width"; from: control.width * 0.8; to: control.width * 1.12;  duration: 70 }
                    NumberAnimation { property: "width"; to: control.width;  duration: 30 }
                }

                NumberAnimation { property: "height"; from: 0; to: control.count > 5 ? control.delegateHeight * 5 : control.delegateHeight * control.count + listView.spacing * (control.count > 5 ? 6 : (control.count + 2));  duration: 100 }
            }
        }

        contentItem: Item{
            height: control.count === 0 ? 0 :(control.count > 5 ? 5 * control.height : control.count * control.height) + listView.spacing * (control.count > 5 ? 6 : (control.count + 2))
            width: control.popup.width
            ListView {
                id: listView
                anchors.fill: parent
                anchors.topMargin: spacing
                spacing: 5
                clip: true
                model: control.popup.visible ? control.delegateModel : null
                currentIndex: control.highlightedIndex

                Q.ScrollIndicator.vertical: Q.ScrollIndicator { }
            }
        }

        background: C.BaseBackground {
            visible: control.count !== 0
            backgroundColor: control.backgroundColor
            radius: 5
            elevation: 2
        }
    }
}
