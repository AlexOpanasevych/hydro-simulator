import QtQuick 2.12
import QtQuick.Window 2.12
import "resources/qml"
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import KLib 1.0


Window {
    id: rootWindow
    visible: true
    width: 1920
    height: 1080
    color: "#9B97B2"

    SplitView {
        anchors.fill: parent

        Item {
            SplitView.preferredWidth: rootWindow.width / 3 * 2
            SplitView.preferredHeight: rootWindow.height
            SplitView.minimumWidth: rootWindow.width / 3

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
            SplitView.preferredWidth: rootWindow.width / 3
            SplitView.preferredHeight: rootWindow.height


            ClientsList {
                anchors.fill: parent
                listHeader: "Список доступних клієнтів"
                list: App.server.clientsModel/*App.server.clientsModel*/
                itemColor: "lightgray"
                color: "#00b8a8"
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
