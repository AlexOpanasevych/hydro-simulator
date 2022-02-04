import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.15

import CQML 1.0 as C

Rectangle{
    color: "#1f2936"
    property string type: ""

    Text {
        anchors.fill: parent
        visible: listView.count === 0
        text: scene.proxy.count === 0 ? "" : "empty"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        color: "#6ab3f3"
    }

    ColumnLayout{
        anchors.fill: parent
        spacing: 0

        Text {
            id: typeText
            Layout.fillWidth: true
            Layout.minimumHeight: 60
            color: "#6ab3f3"
            text: (type + " ITEMS").toUpperCase()
            font.pixelSize: 17
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap

            Shape{
                Layout.fillWidth: true
                Layout.preferredHeight: 5
                ShapePath {
                    fillColor: "transparent"
                    strokeColor: "#7f8c92"
                    strokeWidth: 1
                    startX: 0
                    startY: typeText.height - 1
                    PathLine { x: typeText.width; y: typeText.height - 1 }
                }

            }

            Rectangle{
                color: "white"
                anchors.fill: typeText
                opacity: 0.1
            }

        }

        ListView {
            id: listView
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: sceneArea.scene.proxy
            clip: true
            currentIndex: 0
            highlightMoveDuration: 100

            delegate: ItemDelegate {
                id: delegate
                width: ListView.view.width
                height: 30
                highlighted: listView.highlightedIndex === index
                hoverEnabled: listView.hoverEnabled
                property bool selected: sceneArea.currentItem === modelData
                onSelectedChanged: if (selected) listView.currentIndex = index

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 4
                    anchors.rightMargin: 4
                    text: modelData.id + " " + modelData.objectName + " " + modelData.x.toFixed(1) + " " + modelData.y.toFixed(1)
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
                    onClicked: { sceneArea.currentItem = modelData; sceneRect.moveToItem(modelData) }
                    onDoubleClicked: {}
                }

                Component.onCompleted: if (index === listView.count - 1) type = modelData.type
            }
        }

        C.Button {
            id: buttonRemove
            text: "Remove All"
            textColor: "#1f2936"
            buttonBackground: "#6ab2f2"
            Layout.fillWidth: true
            Layout.leftMargin: 4
            Layout.rightMargin: 4
            Layout.bottomMargin: 4
            buttonRadius: 3
            Layout.maximumHeight: listView.count > 0 ? 30 : 0
            visible: height !== 0
            onPressed: {
                sceneArea.scene.removeAllItems(type)
                sceneArea.currentItem = scene
            }
        }
    }
}

