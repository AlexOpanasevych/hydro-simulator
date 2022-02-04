import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import KLib 1.0
import QtQuick.Layouts 1.12
import "qml"

Window {
    id: rootWindow
    visible: true
    width: 1920
    height: 1080
//    minimumHeight: 1080
    minimumWidth: 1920

    color: "#c7b0a3"

    property bool mOpened: false

    Loader {
        sourceComponent: App.client.clientName !== "all" && App.client.clientName !== "" ? qmlLoaderComp : App.client.clientName === "all" ? globalQmlLoaderComp : null
    }


    Component {
        id: qmlLoaderComp
        QMLLoader {
            id: qmlLoader
        }
    }

    Component {
        id: globalQmlLoaderComp
        GlobalQMLLoader {
            id: globalQmlLoader
            focus: true

            Component.onCompleted: {
                forceActiveFocus()
            }

            Keys.onDigit1Pressed: {
                onDigitPressed(0)
                rootWindow.height = 1080
                forceActiveFocus()
            }
            Keys.onDigit2Pressed: {

                onDigitPressed(1)
                rootWindow.height = 1080
                forceActiveFocus()
            }
            Keys.onDigit3Pressed: {

                onDigitPressed(2)
                rootWindow.height = 1080
                forceActiveFocus()
            }
            Keys.onDigit4Pressed: {

                onDigitPressed(3)
                rootWindow.height = 1080
                forceActiveFocus()
            }
            Keys.onDigit5Pressed: {

                onDigitPressed(4)
                rootWindow.height = 1080
                forceActiveFocus()
            }
            Keys.onDigit6Pressed: {

                onDigitPressed(5)
                rootWindow.height = 1080
                forceActiveFocus()
            }
            Keys.onDigit7Pressed: {
                onDigitPressed(6)
                rootWindow.height = 1080
                forceActiveFocus()
            }
            Keys.onDigit8Pressed: {
                for(var i = 0; i < globalQmlLoader.globalRep.count; i++) {
                    globalQmlLoader.globalRep.itemAt(i).scale = 1
                    globalQmlLoader.globalRep.itemAt(i).enabled = true
                    globalQmlLoader.globalRep.itemAt(i).visible = true
                    onItemAdded(i, globalQmlLoader.globalRep.itemAt(i))
                }
                rootWindow.height /= 2
                forceActiveFocus()
            }
        }
    }

    ServerStatusPopup {
        id: disconnectPopup
        Connections {
            target: App.client

            function onServerDisconnected() {
                disconnectPopup.open()
                App.client.reconnect()
                errorMessagePopup.goodOrBad = false
                errorMessagePopup.message = "Не вдалося під'єднатися до сервера!"
                errorMessagePopup.open()
            }

            function onServerConnected() {
                disconnectPopup.close()
                errorMessagePopup.goodOrBad = true
                errorMessagePopup.message = "Під'єднано!"
                errorMessagePopup.open()
            }
        }

    }


    Menu {
        id: menu

        title: "Виберіть, що потрібно зробити"

        height: parent.height / 3 * 2

        width: parent.width / 3 * 2

        anchors.centerIn: parent

        background: Rectangle {
            id: menuRect
            anchors.fill: parent

            color: "#686963"

            radius: 5
        }

        enter: Transition {
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
        }

        exit: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
             }
        }

        ListView {
            anchors.fill: parent
            model: App.menuModel
            delegate: MenuItem {
                width: parent.width

                height: 50
                clip: true

                Text {
                    id:menuText
                    anchors.fill: parent
                    text: modelData.menuTitle
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 15
                }

                background: Rectangle {
                    id: backRect
                    color: "#699903"
                }
                onTriggered: {
                    App.menuAction(index)
                }

                onHoveredChanged: {
                    if(hovered) {
                        backRect.color = Qt.lighter("#699903")
                    }
                    else {
                        backRect.color = "#699903"
                    }
                }
            }
        }
    }

    RowLayout {
        height: 50
        width: currentMode.paintedWidth + 20 + menuButton.width
        anchors {
            top: parent.top
            right: parent.right
        }
        Text {
            id: currentMode
            font.pixelSize: menuButton.height / 2
            Layout.fillHeight: true
            Layout.fillWidth: true
            text: "Режим перегляду: " + (App.viewMode === App.INTERACTIVE ? "Інтерактивний" : "Допомога")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            color: "white"
        }

        Button {
            id:menuButton
            Layout.fillHeight: true
            width: 50
            text: "\uF0C9"
            font.family: "Font Awesome 5 Pro Regular"
            onPressed: {
                if(!mOpened)
                    menu.open()
                else menu.close()
                mOpened = !mOpened
            }
        }

        VolumeSliderPopup {
            width: 50
            Layout.fillHeight: true
            slider.value: App.client.volume

            onVChanged: (value) => {
                            App.client.changeVolume(value)
                        }
        }
    }






    Popup {
        id: errorMessagePopup

        x: parent.width / 2 - width / 2
        y: -height

        height: parent.height / 5
        width: parent.width / 5 * 2

        property alias message: messageText.text
        property bool goodOrBad: true

        background: Rectangle {
//            anchors.fill: parent

            color: parent.goodOrBad ? "#9CFC97" : "#CC5803"

            radius: 5
        }

        Text {
            id: messageText
            font.pointSize: 11
            anchors.fill: parent

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }

        Timer {
            id: showTimer
            interval: 5000
            repeat: false
            onRunningChanged: {
                if(!running) {
                    errorMessagePopup.close()
                }
            }
        }

        onOpened: {
            showTimer.restart()
        }

        enter: Transition {

            ParallelAnimation {
                NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
                NumberAnimation {target: errorMessagePopup; property: "y"; from: -height; to: 20; duration: 800}
             }


        }

        exit: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
                NumberAnimation {target: errorMessagePopup; property: "y"; from: 20; to: -height; duration: 800}
             }

        }
    }

    property var c

    Component.onCompleted: {
        console.log("aaaaa", App.HELP, App.INTERACTIVE)
        var comp = Qt.createComponent("SimDataConsole.qml");
        c = comp.createObject(rootWindow, {"x": x, "y": y});
        c.show();
    }

}
