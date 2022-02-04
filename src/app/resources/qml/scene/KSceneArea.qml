import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls 1.4 as Old
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.15

import KLib 1.0
import CQML 1.0
import PluginsBase 1.0

Item {
    id: sceneArea
    required property KScene scene
    readonly property DomenMap domenMap: DomenMap{}
    property var currentItem: scene
    property string workMode: "DEFAULT"
    property bool extraInfo: false
    signal moveToCenter()
    signal scenePressed(var point)

    property var distance
    property var courceAngle
    property var rotation
    property var currentDistance
    property var currentTime
    property bool goalFound : false

    Connections{
        target: scene
        function onSceneItemsChanged(){
            domenMap.clear()
        }
    }

    Connections{
        target: scene.piService
        function onCurrentChoosedChanged(){
            if (scene.piService.currentChoosed)
            {
                workMode = "ADD"
            }
            else
            {
                workMode = "DEFAULT"
            }
        }
    }

    Connections{
        target: sceneArea.scene.scenePlayer
        function onStartSignal(){
            addItem.close()
        }
    }

    onCurrentItemChanged: { log(currentItem, "choosed"); }

    Rectangle{
        color: "silver"
        anchors.fill: parent
        clip: true

        SceneRect {
            id: sceneRect
            anchors.fill: parent
            antialiasing: true
            color: "silver"
        }

        StackMenu {
            id: menu
            width: 300
            anchors{
                left: parent.left
                top: parent.top
                bottom: playerItem.top
                leftMargin: -menu.offset
            }

            tabButtons: sceneArea.scene.adminMode ? scene.getComponents("menuButton") : scene.getComponents("menuButton") //tabs.tabButtons
            tabWindows: sceneArea.scene.adminMode ? scene.getComponents("menuWindow") : scene.getComponents("menuWindow")//tabs.tabWindows
            backgroundColor:  "#16222d"

            minimumWindowWidth: 240
            maximumWindowWidth: 530
            initialButton: IconButton{
                buttonBackground: "#16222d"
                iconCode: qsTr("\uF0C9")
                textColor: "#6ab3f3"
                font.family: IconHelper.icons.name
                description: "Menu"
                elevation: menu.opened ? 0 : 3
                onPressed: {
                    if (menu.opened)
                    {
                        menu.close()
                    }
                    else
                    {
                        menu.open()
                    }
                }
            }
        }


//        Timer {
//            id: deleteTimer
//            interval: 5000
//            repeat: false
//            onTriggered: {
//                log("deleting")
////                notificationWorker.sendMessage({"type": "remove", "model" : notification})
//                notification.remove(0)
//            }
//        }


//        WorkerScript {
//            id: notificationWorker
//            source: "pushnotification.mjs"
//        }

//        ListModel {
//            id: notification
////            ListElement {text: "adadadad"}
////            ListElement {text: "adadadad"}
////            ListElement {text: "adadadad"}
//        }

//        ListView {
//            id: notificationStack
//            model: notification
//            width: 200
//            height: 500
//            anchors.right: addItem.left
//            anchors.top: addItem.top
//            anchors.topMargin: 7
//            spacing: 7
//            delegate: Rectangle {
//                color: "#212326"
//                radius: 30
//                width: parent.width ? parent.width : 0
//                height: 40
//                Text {
//                    anchors.fill: parent
//                    text: message
//                    wrapMode: Text.WordWrap
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    color: "white"
//                }
//            }

//            Connections {
//                target: DevLogger
//                function onFancyLogTextChanged(fancyLog) {
//                    log(fancyLog, "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
////                    notificationWorker.sendMessage({'type' : "create", "text" : fancyLog, "model": notification})
//                    notification.append({'message':fancyLog})
//                    deleteTimer.start()
//    //                notification.append({"text": fancyLog})
//                }
//            }

//        }


        StackMenu{
            id: addItem
            orientation: Qt.RightToLeft
            property real topOffset: sceneArea.scene.scenePlayer.mode === KScenePlayer.STOP ?  0 : 40*2
            width: 300
            anchors{
                right: parent.right
                top: parent.top
                bottom: playerItem.top
                rightMargin: addItem.offset
                topMargin: -addItem.topOffset
            }

            Behavior on topOffset { NumberAnimation { duration : 200 } }

            tabButtons: sceneArea.scene.adminMode ? scene.getComponents("addItemMenuButton") : scene.getComponents("addItemMenuButton")//tabs.tabButtons
            tabWindows: sceneArea.scene.adminMode ? scene.getComponents("addItemMenuWindow") : scene.getComponents("addItemMenuWindow")//tabs.tabWindows
            backgroundColor:  "#16222d"

            initialButton: IconButton{
                buttonBackground: "#16222d"
                iconCode: IconHelper.icon["plus"]
                textColor: "#6ab3f3"
                font.family: IconHelper.icons.name
                description: "Add item"
                elevation: addItem.opened ? 0 : 3
                onPressed: {
                    if (addItem.opened){
                        addItem.close()
                        if (sceneArea.scene.piService.currentChoosed)
                            sceneArea.scene.piService.currentChoosed.pressed()
                    }else{
                        addItem.open()
                    }
                }
            }

            onOpenedChanged: {
                if (!opened){
                    if (sceneArea.scene.piService.currentChoosed)
                        sceneArea.scene.piService.currentChoosed.pressed()
                    sceneArea.scene.piService.proxy.invalidate()
                }
            }
        }

        KScenePlayerItem{
            id: playerItem
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -playerItem.offset
            player: scene.scenePlayer
            onStart: initialProperties.open()

            Connections{
                target: scene.scenePlayer
                function onModeChanged(m){
                    sceneArea.workMode = "DEFAULT"
                }
            }
        }
    }

    function createItem(coords, itemType){
        scene.addItem(coords, itemType)
    }
}
