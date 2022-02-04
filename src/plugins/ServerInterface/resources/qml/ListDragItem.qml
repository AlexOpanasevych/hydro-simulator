import QtQuick 2.0
import QtQuick.Layouts 1.0

import KLib 1.0
Item {
    id: root

    width: parent.width
    height: 50
    required property var modelData
    required property int index
    property alias itemColor: tile.color
    property var dropCallback
    property var dropCanselCallback
    MouseArea {
        id: mouseArea

        anchors.fill: parent

        drag.target: tile

        Drag.active: modelData.alive ? mouseArea.drag.active : false

        onReleased: {
            if(tile.caught) {
                App.sConfigurator.bindClientToMode(modelData)
            }
            backAnimX.from = tile.x;
            backAnimX.to = tile.beginDrag.x;
            backAnimY.from = tile.y;
            backAnimY.to = tile.beginDrag.y;
            backAnim.start()
        }

        onPressed: {
            tile.beginDrag = Qt.point(tile.x, tile.y);
        }

        Rectangle {
            id: tile
            property bool caught: false
            property point beginDrag
            property bool alive : modelData.alive
            //            property var prevParent: parent
            border { width:2; color: "white" }
            radius: 5
            width: parent.width
            height: parent.height
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }


            Drag.active: alive ? mouseArea.drag.active : false
            Drag.hotSpot.x: 32
            Drag.hotSpot.y: 32

            RowLayout {
                anchors.fill: parent

                Text {
                    id: whoUsing

                    text: "Used by: " + (modelData.mode !== null ? modelData.mode.name : "")
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 9
                    color: tile.alive ? "black" : "lightgray"
                }

                Text {
                    id: addr
//                    width: parent.width / 2
//                    height: parent.height
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    text: modelData.addr
                    font.pointSize: 10
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    color: tile.alive ? "black" : "lightgray"
                }

                Text {
                    id: port
//                    width: parent.width / 2
//                    height: parent.height
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    text: modelData.port
                    font.pointSize: 10
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    color: tile.alive ? "black" : "lightgray"
                }

            }





            states: [State {
                    when: mouseArea.drag.active
                    //ParentChange { target: tile; parent: undefined}
                    AnchorChanges { target: tile; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined }
                    PropertyChanges { target: tile; width: addr.paintedWidth + port.paintedWidth + 10 }
                    PropertyChanges { target: tile; height: 100 }
                    PropertyChanges {target: tile; x: mouseArea.mouseX - tile.width / 2; y: mouseArea.mouseY - tile.height / 2; z: 100}
//                    StateChangeScript {
//                        name: "copyCreation"

//                        script: {
//                            rectLoader.sourceComponent = dragRectComponent
//                        }
//                    }
                }
            ]

//            transitions: [
//                Transition {
//                    from: "inDrag"
//                    to: "*"
//                    PropertyAnimation {
//                        target: tile
//                        properties: "scale, opacity"
//                        easing.overshoot: 1.5
//                        easing.type: "OutBack"
//                        from: 0.0
//                        to: 1.0
//                        duration: 750
//                    }
//            }
//            ]

//            Behavior on width { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
//            Behavior on height { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
            ParallelAnimation {
                id: backAnim
                NumberAnimation { id: backAnimX; target: tile; property: "x"; duration: 500;}
                NumberAnimation { id: backAnimY; target: tile; property: "y"; duration: 500;}
            }

        }
    }

    function unbindClient(){
        //console.log("unbinding client", index)
        App.sConfigurator.unbindClientFromMode(index)
    }

    function bindClient(){
        var cl = DragHelper.draggedClient
          App.sConfigurator.bindClientToMode(DragHelper.draggedClient)
    }
}
