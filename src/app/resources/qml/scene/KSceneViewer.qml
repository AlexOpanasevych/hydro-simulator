import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import KLib 1.0
import CQML 1.0 as C
import "../topBar"

Rectangle {
    id: sceneViewer
    color: "#1f2936"
    height: 50
    required property string sceneID

    ColumnLayout{
        anchors.fill: parent
        spacing: 0
        RowLayout{
            id: rowLayout
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 45
            //clip: true
            spacing: 0

            Repeater{
                id: topBarRepeater
                Layout.fillHeight: true
                Layout.minimumWidth: height
                model: sceneArea.scene.adminMode ? sceneArea.scene.getComponents("topBar") : sceneArea.scene.getComponents("topBar")
                delegate: Item{
                    property real h: sceneArea.scene.scenePlayer.mode !== KScenePlayer.STOP ? topBarButtonLoader.item.hideable ? 0 : 45 : 45
                    Layout.preferredHeight: h
                    Layout.maximumWidth: topBarButtonLoader.item.objectName == "separator" ? 2 : 45
                    Layout.preferredWidth: topBarButtonLoader.item.objectName == "separator" ? 2 : 45
                    Behavior on h { NumberAnimation { duration : 200; easing.type: Easing.InOutQuad } }
                    opacity: h / rowLayout.height
                    visible: h !== 0
                    Loader{
                        id: topBarButtonLoader
                        anchors.fill: parent
                        anchors.margins: 1
                        sourceComponent: modelData
                    }
                }
            }

//            Item {
//                Layout.fillWidth: true
//            }

            Item {
                id: mode
//                property alias text: messageText.text
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                width: modeText.paintedWidth
                Layout.fillHeight: true
                Text {
                    id: modeText
                    anchors.fill: parent
                    text: "Цикл:"
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"/*"#212326"*/
                    font.pointSize: 14

                }
            }

            Item {
                id: message
//                property alias text: messageText.text
                Layout.rightMargin: 10
                width: 350
                Layout.fillHeight: true
                Text {
                    id: messageText
                    anchors.fill: parent
                    text: sceneArea.scene.scenePlayer.mode !== 2 ? DevLogger.fancyLogText : ""
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    color: "white"/*"#212326"*/
                    font.pointSize: 14

//                    Connections {
//                        target: sceneArea.scene.scenePlayer
//                        function onModeChanged(mode) {
//                            if(mode === 2) {
//                                messageText.text = ""
//                            }
//                        }
//                    }

                }
            }

            Item {
                id: currentDistance
//                property alias text: messageText.text
                Layout.rightMargin: 10
                width: 350
                Layout.fillHeight: true
                Text {
                    id: currentDistanceText
                    anchors.fill: parent
                    text: sceneArea.scene.scenePlayer.mode !== 2 ? "Пройдена дистанція: " + (sceneArea.currentDistance / 1000).toFixed(2) + " км" : "Пройдена дистанція:"
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    color: "white"/*"#212326"*/
                    font.pointSize: 14

                }
            }

            Item {
                id: ourCourse
//                property alias text: messageText.text
                Layout.rightMargin: 10
                width: 150
                Layout.fillHeight: true
                Text {
                    id: ourCourseText
                    anchors.fill: parent
                    text: sceneArea.scene.scenePlayer.mode !== 2 ? "Курс: " + (sceneArea.rotation < 0 ? sceneArea.rotation.toFixed(2) : ' ' + sceneArea.rotation.toFixed(2)) + "\u00B0" : "Курс:"
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    color: "white"/*"#212326"*/
                    font.pointSize: 14

                }
            }

            Item {
                id: currentTime
//                property alias text: messageText.text
                Layout.rightMargin: 10
                width: 200
                Layout.fillHeight: true
                Text {
                    id: currentTimeText
                    anchors.fill: parent
                    text: sceneArea.scene.scenePlayer.mode !== 2 ? "Час: " + (sceneArea.currentTime).toFixed(2) + " с" : "Час:"
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    color: "white"/*"#212326"*/
                    font.pointSize: 14

                }
            }

            Item {
                id: courseAngle
                Layout.rightMargin: 10
                width: 150
                Layout.fillHeight: true
                enabled: sceneArea.goalFound
                visible: sceneArea.goalFound
                Text {
                    id: courceAngleText
                    anchors.fill: parent
                    text: sceneArea.scene.scenePlayer.mode !== 2 ? "КК: " + (sceneArea.courceAngle < 0 ? sceneArea.courceAngle.toFixed(2) : ' ' + sceneArea.courceAngle.toFixed(2)) + "\u00B0" : "КК:"
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    color: "white"/*"#212326"*/
                    font.pointSize: 14

                }
            }

            Item {
                id: distance
//                property alias text: messageText.text
                Layout.rightMargin: 10
                width: 300
                Layout.fillHeight: true
                enabled: sceneArea.goalFound
                visible: sceneArea.goalFound
                Text {
                    id: distanceText
                    anchors.fill: parent
                    text: sceneArea.scene.scenePlayer.mode !== 2 ? "Дистанція до цілі: " + (sceneArea.distance / 1000).toFixed(2) + " км" : "Дистанція до цілі:"
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    color: "white"/*"#212326"*/
                    font.pointSize: 14

//                    Connections {
//                        target: sceneArea.scene.scenePlayer
//                        function onModeChanged(mode) {
//                            if(mode === 2) {
//                                messageText.text = "Дистанція"
//                            }
//                        }
//                    }

                }
            }

            Item {
                Layout.fillWidth: true
            }



            MouseArea{
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumWidth: height
                onReleased: {
                    adminInput.open()
                }
            }
        }


        KSceneArea{
            id: sceneArea
            Layout.fillHeight: true
            Layout.fillWidth: true
            scene: KSceneProvider.getScene(sceneID, 100000, 100000)
        }
    }


    FPSMonitor{
        anchors.top: parent.top
        anchors.right: parent.right
        visible: sceneArea.scene.devMode
        working: visible
        width: 90
        height: 50
        anchors.rightMargin: 10
        Text {
            anchors.centerIn: parent
            text: "FPS: " + parent.fps.toFixed(2)
            color: parent.color
            font.pointSize: 17
        }
        z: 10
    }

    AdminInput{
        id: adminInput
        x: parent.width - width
        height: 45
    }

}

