import QtQuick 2.0
//import QtQuick.Window 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
//import ServerInterface 1.0
import KLib 1.0

Item {
    id: serverWindow
    visible: true
//    width: 1920
//    height: 1080
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent

        color: "#9B97B2"

    }



//    title: "Server Manager v1.0"
    Button {
        z: 10
        anchors.topMargin: 20
        anchors.leftMargin: 20
        text: "\uF01E"
        font.family: "Font Awesome 5 Pro Regular"
        onPressed: {
            App.server.updateStates()
        }
        width: 50
        height: 50
    }

    SplitView {
        anchors.fill: parent

        Item {
            SplitView.preferredWidth:  serverWindow.width / 3 * 2
            SplitView.preferredHeight: serverWindow.height
            SplitView.minimumWidth:    serverWindow.width / 3

            clip: true

            Item {
                anchors.fill: parent
                Layout.fillHeight: true

                Layout.fillWidth: true
                Label {
                    text: "Platina"
                    z:1
                    anchors.margins: 5
                }

                Button {
                    height: 50
                    width: 50
                    anchors.right: parent.right
                    z: 1

                    text: "upload plugins"
                    font.pixelSize: 30

                    onPressed: {
                        pluginUplBar.visible = true
                        App.updateService.updatePlugins();
                    }

                    Connections {
                        target: App.updateService
                        function onPluginsUpdated() {
                            pluginUplBar.visible = false
                        }
                    }
                }

                Tiles {
                    id: tiles

                    moveMode: moveModeState.checked
                    anchors.fill: parent

                    tilePropMargin: 3

                    tileColor: "#DED6D6"

                    color: "#73A6AD"
                    columns: 2

                    propertyColor: "#ADA0A6"

                    Component.onCompleted: {
                        console.log(App.server.modesModel.count(), propertyColor)
                    }

                    //onDevTypeChanged: console.log(devType)
                }


                CheckBox {
                    id: moveModeState
                    anchors.margins: 10
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    text: "Enable move mode"
                }
            }

        }

        Item {
            SplitView.preferredWidth: serverWindow.width / 3
            SplitView.preferredHeight: serverWindow.height
            SwipeView {
                id: swipeView
                anchors.fill: parent
                clip: true



                ClientsList {
                    id: firstPage
    //                anchors.fill: parent
                    listHeader: "Список доступних клієнтів"
                    list: App.server.clientsModel/*App.server.clientsModel*/
                    itemColor: "lightgray"
                    color: "#00b8a8"
                    clearCallback: () => {
                                    App.sConfigurator.deleteAllClients()
                                  }

                }

                ColumnLayout {
    //                anchors.fill: parent
                    id: secondPage
                    ColumnLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        spacing: 5
                        Text {
                            Layout.alignment: Qt.AlignTop
                            text: "Bronze"
                            font.pointSize: 10
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            Layout.fillWidth: true
                            height: 20
                        }

                        ListView {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            model: App.server.bronzeStation.stateModel

                            spacing: 3

                            delegate: Rectangle {
//                                radius: 5
                                height: 20
                                width: parent.width
                                color: "transparent"
                                Text {
                                    id: state1
                                    anchors.fill: parent
                                    text: "stateID: " + modelData.stateId + ",   value: " + JSON.stringify(modelData.value)
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }

                            Component.onCompleted: {
                                log(App.server.bronzeStation.stateModel.count())
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        spacing: 5
                        Text {
                            Layout.alignment: Qt.AlignTop
                            text: "Platina"
                            font.pointSize: 10
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            Layout.fillWidth: true
                            height: 20
                        }

                        ListView {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            model: App.server.platinaStation.stateModel
                            spacing: 3
                            delegate: Rectangle {
                                height: 20
                                width: parent.width
//                                radius: 5
                                color: "transparent"
                                Text {
                                    id: state2
                                    anchors.fill: parent
                                    text: "stateID: " + modelData.stateId + ",   value: " + JSON.stringify(modelData.value)
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                            Component.onCompleted: {
                                log(App.server.bronzeStation.stateModel.count())
                            }
                        }
                    }
                }
            }
        }
    }

    ProgressBar {
        id: pluginUplBar
        indeterminate: true
        visible: false
        width: 300
        anchors.centerIn: parent
    }
}
