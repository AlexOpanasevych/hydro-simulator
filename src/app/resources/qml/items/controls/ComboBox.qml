import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.15

import KLib 1.0
import "../base"

Item{
    id: root
    property string text: ""
    width: 60
    height: 40
    property var model
    property int currentIndex: 0
    property string currentText: !model ? "" : model[0]
    property color backgroundColor: Qt.rgba(156,191,209,0.3)
    property color textColor: "silver"

    signal modeChanged(bool mode)
    signal pressed()

    function reset()
    {
        state = "off"
    }

    state: "off"
    states:[
        State {
            name: "off"
            PropertyChanges {
                target: root
            }
            onCompleted: modeChanged(false)
        },
        State {
            name: "on"
            PropertyChanges {
                target: root
            }
            onCompleted: modeChanged(true)
        }
    ]

    WavedMouseArea{
        anchors.fill: parent
        onClicked: {
            if (root.state === "on")
            {
                root.state = "off"
            }
            else if (root.state === "off")
            {
                listViewPopup.open()
            }
            root.pressed()
        }
    }

    Rectangle{
        id: currentTextItem
        anchors.fill: parent
        color: backgroundColor
        border.width: 1
        border.color: "black"
        Text {
            anchors.fill: parent
            text:  root.state === "off" ? root.text : currentText
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: root.textColor
        }
    }

    Popup{
        id: listViewPopup
        y: currentTextItem.height
        width: parent.width
        property real realHeight: listView.count > 5 ? 300 : listView.count * root.height
        modal: false
        focus: false
        clip: true

        exit: Transition {
            NumberAnimation { target: listViewPopup; duration: 600; property: "height"; from: listViewPopup.realHeight; to: 0; easing.type: Easing.OutQuint }
        }

        enter: Transition {
            NumberAnimation { target: listViewPopup; duration: 600; property: "height"; from: 0; to: listViewPopup.realHeight; easing.type: Easing.OutQuint }
        }

        background: Rectangle{
            id: popupRect
            color: "blue"
            anchors.fill: parent

            LinearGradient {
                anchors.fill: parent
                start: Qt.point(0,0)
                end: Qt.point(popupRect.width, 0)
                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: "#30384f"
                    }
                    GradientStop {
                        position: 1.0
                        color: "#7f8c92"
                    }
                }
            }

            ListView {
                id: listView
                anchors.fill: parent
                model: root.model
                clip: true
                currentIndex: 0
                highlightMoveDuration: 100

                delegate: ItemDelegate {
                    id: delegate
                    width: ListView.view.width
                    height: root.height
                    highlighted: listView.highlightedIndex === index
                    hoverEnabled: listView.hoverEnabled
                    property string delegateText: modelData

                    Text {
                        anchors.fill: parent
                        anchors.leftMargin: 4
                        anchors.rightMargin: 4
                        text: delegate.delegateText
                        verticalAlignment: Text.AlignVCenter
                        color: "#fffdfe"
                        font.weight: listView.currentIndex === index ? Font.DemiBold : Font.Normal
                    }

                    background: Item {
                        LinearGradient {
                            anchors.fill: parent
                            visible: delegate.ListView.isCurrentItem
                            start: Qt.point(0, 0)
                            end: Qt.point(0, delegate.height)
                            opacity: 0.3
                            gradient: Gradient {
                                GradientStop {
                                    position: 0.0
                                    color: "transparent"
                                }
                                GradientStop {
                                    position: 1.0
                                    color: "#fffdfe"
                                }
                            }
                        }

                        Shape {
                            anchors.fill: parent
                            ShapePath {
                                fillColor: "transparent"
                                strokeColor: "#7f8c92"
                                strokeWidth: 1
                                startX: 0
                                startY: delegate.height - 1
                                PathLine { x: delegate.width; y: delegate.height - 1 }
                            }
                        }
                    }

                    MouseArea{
                        anchors.fill: parent
                        onClicked: { root.currentIndex = index; root.currentText = root.model[index]; root.state = "on"; listViewPopup.close() }
                    }
                }
            }
        }
    }
}
