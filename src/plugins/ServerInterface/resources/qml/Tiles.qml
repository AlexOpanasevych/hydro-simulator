import QtQuick 2.0
import QtQml.Models 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import KLib 1.0

Rectangle {
    id: root

    property int tilePropMargin;
    property color tileColor;
    property color propertyColor;
    property var tileModel
    property string tileLabel;
    property int columns : 3
    property bool moveMode


    //required property int devType

    Repeater {
        id: tileRepeater
        anchors.fill: parent
        model: App.server.modesModel
        delegate: Rectangle{
            id: rootDel
            x: modelData.x
            y: modelData.y

            color: "#C6C7C4"

            border {
                width: 2
                color: "#353B3C"
            }

            radius: 3

            width: modelData.width
            height: modelData.height


            Rectangle {
                id: header
                color: "#003249"
                radius: 3
                width: parent.width
                height: textHeader.paintedHeight
                Text {
                    id: textHeader
                    color: "white"
                    text: modelData.name
                    font.pixelSize: 15
                }
            }

            ListView {
                id: connectedClients
                height: parent.height - tile.height - header.height

                width: parent.width - 4

                anchors.horizontalCenter: parent.horizontalCenter

                model: modelData.clients

                spacing: 3

                anchors.top: header.bottom

                ScrollBar.vertical: ScrollBar {/*policy: "AlwaysOn"*/}

                delegate: Rectangle {
                    id: clientDelegate
                    property bool alive: modelData.alive
                    color: alive ? "#007EA7" : "lightgray"
                    radius: 3

                    height: 20
                    width: parent !== null ? parent.width : 0

                    RowLayout {
                        spacing: 1
                        anchors.fill: parent
                        Text {
                            id: clientIP
                            text: "ip: " + modelData.addr
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.minimumWidth: parent.width / 2
                            height: 10

                            color: clientDelegate.alive ? "white" : "darkgray"
                        }
                        Text {
                            id: clientPort
                            text: modelData.port
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            height: 10
                            color: clientDelegate.alive ? "white" : "darkgray"
                        }

                        Button {
                            Layout.fillHeight: true
                            width: 20
                            background: Rectangle {
                                anchors.fill: parent
                                color: clientDelegate.alive ? "#9AD1D4" : "darkgray"
                                radius: 3
                            }
                            visible: connectedClients.count
                            text: "\uF068"
                            font.family: "Font Awesome 5 Pro Regular"

                            onClicked: {
                                if(clientDelegate.alive)
                                    App.sConfigurator.unbindClientFromMode(modelData)
                            }
                        }
                    }

                    onAliveChanged: {
                        if(alive == true) {
                            message.backgroundColor = "#58A84D"
                            message.messageText = "Нове підключення: Клієнт " + modelData.addr + "щойно під'єднався!"
                            message.outerPopup.open()
                        }
                    }
                }
            }


            Rectangle {
                id: tile

                property alias dropProxy: dragTarget
                radius: 3
                anchors.margins: 3

                //                    Layout.fillWidth: true
                //                    Layout.fillHeight: true
                //                    anchors.fill: parent
                width: parent.width - anchors.margins
                height: parent.height - anchors.margins - header.height

                anchors {
                    bottom: parent.bottom
                }

                Text {
                    anchors.fill: parent
                    text: "\uF067"
                    font.family: "Font Awesome 5 Pro Regular"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }


                DropArea {
                    id: dragTarget

                    anchors.fill: parent
                    z: 1

                    onEntered: {
                        drag.source.caught = true

                        App.sConfigurator.enteredMode = modelData
                        console.log("Entered ssssssssssssssssssssssssss", modelData.x)
                    }
                    onExited: drag.source.caught = false;

                    onDropped: {
                        console.log("dropped")
                    }

                    states: [State {
                            name: "drag"
                            when: dragTarget.containsDrag
                            PropertyChanges {
                                target: tile
                                explicit: true
                                color: Qt.darker(color)
                            }
                        }]
                }

                states: [
                    State {
                        name: "hasClients"
                        when: connectedClients.count > 0
                        PropertyChanges {
                            target: tile
                            explicit: true

                            width: tile.parent.width - anchors.margins
                            height: 50 - anchors.margins
                        }
                    }
                ]

                Behavior on width { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
                Behavior on height { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }

            }

            MouseArea{
                z:1
                property int __x: 0
                property int __y: 0
                anchors.fill: parent

                visible: moveMode
                onPressed: {
                    if(moveMode) {
                        var c = mapToItem(root, mouseX, mouseY)
                        __x = c.x
                        __y = c.y

                        console.log("pressed", mouseX, mouseY)
                    }
                }

                onPositionChanged: {
                    if (moveMode) {
                        var c = mapToItem(root, mouseX, mouseY)
                        modelData.x = modelData.x + (c.x - __x)

                        __x = c.x
                        modelData.y = modelData.y + (c.y - __y)
                        __y = c.y
                    }
                }
            }

        }
    }


    PopupMessage {
        id: message
    }
}


