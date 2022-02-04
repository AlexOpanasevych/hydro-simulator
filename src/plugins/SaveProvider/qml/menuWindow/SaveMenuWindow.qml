import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtQuick.Controls 2.15
import QtQml.Models 2.15

import CQML 1.0 as C
import SaveProvider 1.0
import QtQml 2.15

Rectangle{
    id: rootWindow
    color: "#1f2936"
    property string type: ""
    property SaveData replaceTarget
    property bool moving: false
    property bool movingEnabled: true
    property int expandIndex: -1

    SaveManager{ id: manager; Component.onCompleted: sceneAccess.setScene(sceneArea.scene) }

    Connections{
        target: sceneArea.scene.scenePlayer
        function onModeChanged(){
            menu.close(0)
        }
    }

    Connections{
        target: manager
        function onModeChanged(m){
            if (m === SaveManager.LOOKING){
                state = "looking"
            }else if (m === SaveManager.EDITING){
                state = "editing"
            }else if (m === SaveManager.CREATING){
                state = "creating"
            }else if (m === SaveManager.REPLACING){
                state = "replacing"
            }
        }
    }
    state: "looking"
    states: [
        State{
            name: "looking"
        },
        State {
            name: "editing"
        },
        State {
            name: "creating"
        },
        State {
            name: "replacing"
        }
    ]

    transitions: [
        Transition {
            from: "looking"
            to: "editing"
            NumberAnimation{
                target: additionalWindow
                property: "offset"
                from: height - 40
                to: 40
                duration: 200
            }

            NumberAnimation{
                target: additionalWindow.addButtonAlias
                property: "opacity"
                to: 0
                duration: 200
            }

        },
        Transition {
            from: "editing"
            to: "looking"
            NumberAnimation{
                target: additionalWindow
                property: "offset"
                from: 40
                to: height - 40
                duration: 200
            }

            NumberAnimation{
                target: additionalWindow.addButtonAlias
                property: "opacity"
                to: 1
                duration: 200
            }
        },
        Transition {
            from: "looking"
            to: "creating"
            NumberAnimation{
                target: additionalWindow
                property: "offset"
                from: height - 40
                to: 40
                duration: 200
            }
            NumberAnimation{
                target: additionalWindow.addButtonAlias
                property: "opacity"
                to: 0
                duration: 200
            }

        },
        Transition {
            from: "replacing"
            to: "looking"
            NumberAnimation{
                target: additionalWindow
                property: "offset"
                to: height - 40
            }

        },
        Transition {
            from: "creating"
            to: "looking,replacing"
            NumberAnimation{
                target: additionalWindow
                property: "offset"
                from: 40
                to: height - 40
                duration: 200
            }

            NumberAnimation{
                target: additionalWindow.addButtonAlias
                property: "opacity"
                to: 1
                duration: 200
            }
        }
    ]

    MouseArea{
        preventStealing: true
        hoverEnabled: true
        onContainsMouseChanged: {}
        anchors.fill: parent
        onWheel: {}
    }

    DelegateModel {
        id: visualModel
        model: manager.saves
        delegate: DragDelegate{}
    }

    ColumnLayout{
        id: layouts
        anchors.fill: parent
        spacing: 0

        C.BaseBackground{
            Layout.minimumHeight: 40
            backgroundColor: "#1f2936"
            elevation: 3
            Layout.fillWidth: true

            Text {
                id: typeText
                anchors.fill: parent
                color: "#6ab3f3"
                text: "SAVES"
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
        }

        ColumnLayout{
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView{
                id: savesListView
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: visualModel
                clip: true
            }

            C.BaseBackground{
                backgroundColor: "#1f2936"
                elevation: 3
                inverted: true
                Layout.preferredHeight: 35
                Layout.fillWidth: true
            }
        }
    }

    AdditionalWindow {
        id: additionalWindow
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: offset
    }
}
