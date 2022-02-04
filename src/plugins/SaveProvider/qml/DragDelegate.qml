import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtQuick.Controls 2.15
import QtQml.Models 2.15

import CQML 1.0 as C
import SaveProvider 1.0
import QtQml 2.15

Component {
    id: dragDelegate
    MouseArea {
        id: dragArea
        property bool held: false
        anchors {
            right: parent ? parent.right : undefined;
            left: parent ? parent.left : undefined
        }
        height: content.height
        drag.target: held ? content : undefined
        drag.axis: Drag.YAxis
        onPressed: {
            if (manager.mode === SaveManager.REPLACING){
                if (replaceTarget === modelData) replaceTarget = null
                else replaceTarget = modelData
            }else{
                if (mouseX > width - 27 && mouseX < width - 10 && mouseY > 10 && mouseY < height - 10){
                    //savesListView.currentIndex = index
                    if (rootWindow.expandIndex === -1)
                        held = true
                }else if (!rootWindow.moving){
                    //savesListView.currentIndex = index
                    if (rootWindow.expandIndex === index) rootWindow.expandIndex = -1
                    else rootWindow.expandIndex = index
                }
            }
        }

        onHeldChanged: {
            if (held) rootWindow.moving = held
        }

        onReleased: held = false

        Binding{
            target: modelData
            property: "idx"
            value: dragArea.DelegateModel.itemsIndex
        }

        DropArea {
            anchors { fill: parent; margins: 10 }

            onEntered: {
                visualModel.items.move(
                            drag.source.DelegateModel.itemsIndex,
                            dragArea.DelegateModel.itemsIndex)
            }
        }

        C.BaseBackground {
            id: content
            clip: true
            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
            }
            width: dragArea.width;
            height: rootWindow.expandIndex === index? 210 : 40
            Behavior on height { NumberAnimation { duration: 200 } }
            Behavior on descriptionOffset { NumberAnimation { duration: 200 } }
            onParentChanged: {
                if (parent === dragArea)
                    rootWindow.moving = false
            }

            //border.width: 1
            //border.color: "lightsteelblue"

            backgroundColor: "transparent"
            elevation: dragArea.held ? 5 : 0
            radius: 2
            Drag.active: dragArea.held
            Drag.source: dragArea
            Drag.hotSpot.x: width / 2
            Drag.hotSpot.y: height / 2
            property real descriptionOffset: description.width
            states:[
                State {
                    when: dragArea.held
                    ParentChange { target: content; parent: root }
                    AnchorChanges {
                        target: content
                        anchors { horizontalCenter: undefined; verticalCenter: undefined }
                    }
                },
                State {
                    when: rootWindow.expandIndex === index
                    PropertyChanges {
                        target: content
                        descriptionOffset: -5
                    }
                    PropertyChanges{
                        target: descriptionField
                        descriptionOffset: -5
                    }
                    AnchorChanges{
                        target: date
                        anchors.left: undefined
                        anchors.right: parent.right
                    }
                },
                State {
                    when: rootWindow.expandIndex !== index
                    PropertyChanges {
                        target: content
                        descriptionOffset: description.width
                    }
                    PropertyChanges{
                        target: descriptionField
                        descriptionOffset: descriptionField.width
                    }
                }
            ]
            transitions: Transition {
                enabled: rootWindow.expandIndex !== index && !moving
                AnchorAnimation { duration: 200; }
            }

            Shape{
                anchors.fill: parent
                ShapePath{
                    startX: 0
                    startY: height - 1
                    strokeColor: Qt.darker("#6ab2f2", 3)
                    strokeWidth: 2
                    PathLine{
                        x: width
                        y: height - 1
                    }
                }
            }

            Rectangle{
                anchors.fill: parent
                color: "lightsteelblue"
                opacity: dragArea.held || replaceTarget === modelData ? 0.3 : 0
                Behavior on opacity { NumberAnimation { duration: 200 } }
            }

            Text {
                id: name
                anchors.topMargin: rootWindow.expandIndex === index ? 20 : 0
                anchors.top: parent.top
                anchors.left: parent.left
                width: parent.width * 0.8
                elide: Text.ElideRight
                height: 20
                text: "Name: " + modelData.saveName
                anchors.leftMargin: 5
                verticalAlignment: Text.AlignVCenter
                Behavior on anchors.topMargin { NumberAnimation { duration: 200 } }
                color: "#6ab2f2"
            }

            Text{
                id: description
                width: paintedWidth
                height: 20
                text: "Description: "
                verticalAlignment: Text.AlignVCenter
                anchors.left: parent.left
                anchors.leftMargin: -content.descriptionOffset
                anchors.top: name.bottom
                color: "#6ab2f2"
            }

            Text {
                id: date
                anchors.top: description.top
                anchors.left: description.right
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                text: modelData ? rootWindow.expandIndex === index ? modelData.date : "Date: " + modelData.date : "null"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                width: 177
                elide: Text.ElideLeft
                color: "#6ab2f2"
            }

            RowLayout{
                id: loadEdit
                anchors.right: parent.right
                anchors.bottom: parent.top
                anchors.bottomMargin: rootWindow.expandIndex === index ? -loadEdit.height : 0
                height: 30
                width: 70
                Behavior on anchors.bottomMargin { NumberAnimation { duration: 200 } }
                C.IconButton{
                    textColor: "#6ab2f2"
                    iconCode: "\uF044"
                    font.family: C.IconHelper.icons.name
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    buttonBackground: "#1f2936"
                    elevation: 0
                    onReleased: {
                        manager.edit(modelData)
                    }
                }
                C.IconButton{
                    textColor: "#6ab2f2"
                    iconCode: "\uF310"
                    font.family: C.IconHelper.icons.name
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    buttonBackground: "#1f2936"
                    elevation: 0
                    onReleased: {
                        sceneArea.currentItem = sceneArea.scene
                        manager.load(modelData)
                    }
                }
            }

            MoveComboButton{
                anchors.right: parent.right
                anchors.rightMargin: -(description.width - content.descriptionOffset)
                anchors.top: parent.top
                scale: 0.95
                enabled: manager.mode !== SaveManager.REPLACING && rootWindow.expandIndex === -1
                topEnabled: index !== 0
                bottomEnabled: savesListView.count - 1 !== index
            }

            Text {
                visible: sceneArea.scene.devMode
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                text: index + ":" + dragArea.DelegateModel.itemsIndex
                color: "#6ab2f2"
            }
            Flickable {
                id: descriptionField
                property real descriptionOffset: descriptionField.width
                anchors.left: parent.left
                anchors.top: description.bottom
                anchors.leftMargin: -descriptionOffset
                width: parent.width - 10
                height: 145
                clip: true

                Behavior on descriptionOffset { NumberAnimation { duration: 200 } }

                flickableDirection: Flickable.VerticalFlick

                ScrollBar.vertical:  ScrollBar { policy: ScrollBar.AlwaysOn }

                TextArea.flickable: TextArea{
                    id: textArea
                    placeholderText: "DESCRIPTION"
                    selectByMouse: true
                    text: modelData.description
                    background: Rectangle {
                        implicitWidth: 200
                        implicitHeight: 20
                        color: "#2a5378"
                        border.color: "#1f2936"
                        radius: 3
                    }
                    readOnly: true
                    color: "#6ab2f2"
                }
            }
        }
    }
}
