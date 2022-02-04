import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtQuick.Controls 2.15
import QtQml.Models 2.15

import CQML 1.0 as C
import SaveProvider 1.0
import QtQml 2.15

C.BaseBackground{
    id: additionalWindow
    property real offset: 45
    property C.Button addButtonAlias: addButton
    //anchors.bottomMargin: -offset
    backgroundColor: "#1f2936"
    //onOffsetChanged: console.log("offset", offset)

    //Behavior on offset { NumberAnimation { duration: 200 } }

    Binding{
        target: additionalWindow
        property: "offset"
        value: rootWindow.height - 40
        when: manager.mode === SaveManager.LOOKING || manager.mode === SaveManager.REPLACING
        //@disable-check M16
        restoreMode : Binding.RestoreBinding
    }

    ColumnLayout{
        anchors{
            fill: parent
            leftMargin: 5
            rightMargin: 5
        }
        spacing: 10

        RowLayout{
            Layout.fillWidth: true
            Layout.topMargin: 5
            Layout.bottomMargin: 5
            spacing: manager.mode === SaveManager.REPLACING ? 5 : 0
            C.Button {
                text: "CANCEL"
                textColor: "#1f2936"
                buttonBackground: "#6ab2f2"
                Layout.fillWidth: true
                Layout.maximumWidth: manager.mode === SaveManager.REPLACING ? 500 : 0
                Layout.preferredHeight: manager.mode !== SaveManager.REPLACING && manager.mode !== SaveManager.LOOKING ? 0 : 30
                buttonRadius: 3
                onPressed: {
                    replaceTarget = null
                    manager.cancelReplacing()
                }
            }
            C.Button {
                id: addButton
                text: manager.mode === SaveManager.REPLACING ? "REPLACE" : "ADD SAVE"
                enabled: manager.mode === SaveManager.REPLACING ? replaceTarget : true
                textColor: "#1f2936"
                buttonBackground: "#6ab2f2"
                Layout.preferredHeight: opacity === 1 ? 30 : 0
                Layout.fillWidth: true
                Layout.maximumWidth: 500
                buttonRadius: 3
                onPressed: {
                    if (manager.mode === SaveManager.REPLACING){
                        manager.replace(replaceTarget, manager.cacheData)
                    }else{
                        rootWindow.expandIndex = -1
                        manager.createSaveData()
                    }
                    replaceTarget = null
                }
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            Layout.maximumHeight: 30
            visible: manager.mode === SaveManager.CREATING || manager.mode === SaveManager.EDITING
            Text {
                verticalAlignment: Text.AlignVCenter
                Layout.preferredWidth: paintedWidth
                color: "#6ab2f2"
                text: "Name: "
            }
            C.TextField{
                id: nameField
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.leftMargin: 5
                Layout.rightMargin: 5
                text: " "
                color : "#6ab2f2"
                hintColor: Qt.darker("#6ab2f2", 1.2)
                helperText: ""

                onTextChanged: {
                    if (text === ""){
                        hasError = true
                        helperText = "name couldn't be an empty!"
                    }else{
                        hasError = manager.checkName(text)
                        if (hasError){
                            helperText = "name already exist!"
                        }else{
                            helperText = ""
                            manager.cacheData.saveName = text
                        }
                    }
                }
                Component.onCompleted: text = ""

                Connections{
                    target: manager
                    function onModeChanged(mode){
                        if (mode === SaveManager.EDITING){
                            nameField.text = manager.cacheData.saveName
                            textArea.text = manager.cacheData.description
                        }
                    }
                }
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            Layout.maximumHeight: 30
            Text {
                verticalAlignment: Text.AlignVCenter
                Layout.preferredWidth: paintedWidth
                color: "#6ab2f2"
                text: "Date: "
            }
            C.TextField{
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.leftMargin: 5
                Layout.rightMargin: 5
                color : "#6ab2f2"
                readOnly: true
                text: manager.cacheData ? manager.cacheData.date : "null"
                hintColor: Qt.darker("#6ab2f2", 1.2)
            }
        }

        ColumnLayout{
            Layout.fillWidth: true
            Layout.preferredHeight: 170
            Layout.maximumHeight: 170
            Text {
                verticalAlignment: Text.AlignVCenter
                Layout.preferredWidth: paintedWidth
                color: "#6ab2f2"
                text: "Description: "
            }

            Flickable {
                id: flickable
                Layout.preferredHeight: 150
                Layout.fillWidth: true
                Layout.leftMargin: 4
                Layout.rightMargin: 4
                clip: true
                enabled: opacity == 1

                flickableDirection: Flickable.VerticalFlick

                ScrollBar.vertical:  ScrollBar { policy: ScrollBar.AlwaysOn }
                Behavior on Layout.preferredHeight { NumberAnimation { duration: 200; easing.type: Easing.OutQuad } }

                TextArea.flickable: TextArea{
                    id: textArea
                    placeholderText: "DESCRIPTION"
                    selectByMouse: true
                    color: "#6ab2f2"
                    text: ""
                    background: Rectangle {
                        implicitWidth: 200
                        implicitHeight: 20
                        color: "#2a5378"
                        border.color: "#1f2936"
                        radius: 3
                    }
                }
            }
        }

        C.Button {
            id: confirmButton
            text: "CONFIRM"
            textColor: "#1f2936"
            enabled: !nameField.hasError
            buttonBackground: "#6ab2f2"
            Layout.preferredHeight: opacity ? 30 : 0
            Layout.fillWidth: true
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            buttonRadius: 3
            onPressed: {
                if (manager.mode === SaveManager.CREATING){
                    manager.cacheData.saveName = nameField.text
                    manager.cacheData.description = textArea.text
                    nameField.text = ""
                    manager.save(manager.cacheData)
                }else if (manager.mode === SaveManager.EDITING){
                    manager.cacheData.description = textArea.text
                    manager.updateCacheData()
                    manager.replace(manager.currentData, manager.cacheData)
                    manager.cacheData = null
                }
            }
        }

        C.Button {
            id: replaceButton
            text: "REPLACE"
            textColor: "#1f2936"
            enabled: !nameField.hasError
            buttonBackground: "#6ab2f2"
            Layout.preferredHeight: opacity ? 30 : 0
            Layout.fillWidth: true
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            buttonRadius: 3
            visible: manager.mode === SaveManager.CREATING
            onPressed: {
                manager.prepareReplacing()
            }
        }

        C.Button {
            id: deleteButton
            text: "DELETE"
            textColor: "#1f2936"
            enabled: !nameField.hasError
            buttonBackground: "#6ab2f2"
            Layout.preferredHeight: opacity ? 30 : 0
            Layout.fillWidth: true
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            buttonRadius: 3
            visible: manager.mode === SaveManager.EDITING
            onPressed: {
                manager.removeCurrentData()
            }
        }

        C.Button {
            text: "CANCEL"
            textColor: "#1f2936"
            buttonBackground: "#6ab2f2"
            Layout.preferredHeight: opacity ? 30 : 0
            Layout.fillWidth: true
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            buttonRadius: 3
            onPressed: {
                manager.cancelCreating()
            }
        }

        Item{
            Layout.fillHeight: true
        }
    }
}
