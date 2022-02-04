import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.15

import CQML 1.0 as C

Rectangle{
    id: root
    color: "#1f2936"
    clip: true
    property bool somethingChanged: false
    visible: showed

    onVisibleChanged: {
        if (!visible) resetAllStates()
    }

    MouseArea{
        anchors.fill: parent
        onWheel: {}
        preventStealing: true
    }

    Text {
        anchors.fill: parent
        visible: listView.count === 0
        text: scene.proxy.count === 0 ? "" : "empty"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        color: "#6ab3f3"
    }

    Connections{
        target: sceneArea.scene.piService
        function onCurrentChoosedChanged(c){
            if (c){
                somethingChanged = false
                objectNameField.text =  c.objectName === "default_object_name" ? "" : c.objectName
                maxVelocityField.text = c.maxVelocity
                maxDepthField.text = c.maxDepth
                imagePath.path = sceneArea.scene.piService.currentChoosed.imagePath
            }
            else
            {
                sceneArea.scene.piService.editing = false
                sceneArea.scene.piService.cancelClone()
                sceneArea.scene.piService.proxy.invalidate()
            }
        }
    }

    ColumnLayout{
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        Item{
            id: title
            Layout.fillWidth: true
            Layout.minimumHeight: 60
            clip: true

            Text {
                id: typeText
                anchors{
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    bottom: currentType.top
                }

                text: ("TORPEDO ITEMS")
                color: "#6ab3f3"
                font.pixelSize: 17
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap

                Behavior on height { NumberAnimation { duration: 100 } }
            }

            Text{
                id: currentType
                anchors{
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                color: "#6ab3f3"
                text: scene.piService.currentChoosed ? ("Create: " + scene.piService.currentChoosed.objectName).toUpperCase() : ""
                font.pixelSize: 17
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                height: text === "" ? 0 : 30
                visible: height !== 0
                Behavior on height { NumberAnimation { duration: 100 } }
            }

            Shape{
                Layout.fillWidth: true
                Layout.preferredHeight: 5
                ShapePath {
                    fillColor: "transparent"
                    strokeColor: "#7f8c92"
                    strokeWidth: 1
                    startX: 0
                    startY: title.height - 1
                    PathLine { x: title.width; y: title.height - 1 }
                }
            }


            Rectangle{
                color: "white"
                anchors.fill: title
                opacity: 0.1
            }

            RowLayout
            {
                anchors{
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                }
                width: height * 1.4
                spacing: 0

                C.IconButton{
                    id: saveButton
                    iconCode: C.IconHelper.icon["save"]//"\uf21a"
                    description: "SAVE"
                    textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
                    buttonBackground: "#556476"
                    font.family: C.IconHelper.icons.name
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.margins: 0
                    Layout.bottomMargin: 2
                    opacity: sceneArea.scene.piService.readySave

                    Behavior on opacity { NumberAnimation { duration : 200 } }

                    onPressed:{
                        sceneArea.scene.piService.save()
                    }
                }

                C.IconButton{
                    id: undoButton
                    iconCode: C.IconHelper.icon["undo-alt"]//"\uf21a"
                    description: "UNDO"
                    textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
                    buttonBackground: "#556476"
                    font.family: C.IconHelper.icons.name
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.margins: 0
                    Layout.bottomMargin: 2

                    opacity: sceneArea.scene.piService.readySave

                    Behavior on opacity { NumberAnimation { duration : 200 } }

                    onPressed:{
                        sceneArea.scene.piService.load()
                    }
                }
            }
        }

        ListView {
            id: listView
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.minimumHeight: 0
            model: sceneArea.scene.piService.proxy
            clip: true
            currentIndex: 0
            highlightMoveDuration: 100

            delegate: ItemDelegate {
                id: delegate
                width: ListView.view.width
                height: 30
                highlighted: listView.highlightedIndex === index
                hoverEnabled: listView.hoverEnabled
                visible: !sceneArea.scene.piService.editing
                property bool selected: sceneArea.currentItem === modelData
                onSelectedChanged: if (selected) listView.currentIndex = index

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 4
                    anchors.rightMargin: 4
                    text: modelData.objectName + " " + modelData.removed
                    verticalAlignment: Text.AlignVCenter
                    color: "#fffdfe"
                    font.weight: listView.currentIndex === index ? Font.DemiBold : Font.Normal
                }

                background: Item {
                    LinearGradient {
                        anchors.fill: parent
                        visible: modelData === sceneArea.scene.piService.currentChoosed
                        start: Qt.point(0, 0)
                        end: Qt.point(0, delegate.height)
                        opacity: 0.3
                        gradient: Gradient {
                            GradientStop {
                                position: 0.0
                                color: "transparent"
                            }
                            GradientStop {
                                position: 1.0
                                color: "#fffdfe"
                            }
                        }
                    }

                    Shape {
                        anchors.fill: parent
                        ShapePath {
                            fillColor: "transparent"
                            strokeColor: "#7f8c92"
                            strokeWidth: 1
                            startX: 0
                            startY: delegate.height - 1
                            PathLine { x: delegate.width; y: delegate.height - 1 }
                        }
                    }
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        modelData.pressed()
                    }
                    onDoubleClicked: {}
                }
            }

            Behavior on height { NumberAnimation { duration : 200 } }
        }

        RowLayout{
            id: addNewLayout
            Layout.fillWidth: true
            Layout.leftMargin: 4
            Layout.rightMargin: 4
            Layout.bottomMargin: 4
            Layout.maximumHeight: listView.count > 0 && !sceneArea.scene.piService.editing ? 30 : listView.count === 0 ? 30 : 0
            spacing: 2
            clip: true

            C.Button {
                id: addNew
                text: "ADD NEW"
                textColor: "#1f2936"
                buttonBackground: "#6ab2f2"
                Layout.fillWidth: true
                buttonRadius: 3
                width: sceneArea.scene.piService.currentChoosed ? addNewLayout.width / 2 - 1 : addNewLayout.width
                Layout.fillHeight: true
                onPressed: {
                    sceneArea.scene.piService.add("Torpedo")
                }
            }

            C.Button {
                id: clone
                text: "CLONE"
                textColor: "#1f2936"
                buttonBackground: "#6ab2f2"
                Layout.fillWidth: true
                width: sceneArea.scene.piService.currentChoosed ? addNewLayout.width / 2 - 1 : 0
                Layout.fillHeight: true
                buttonRadius: 3
                visible: width !== 0
                onPressed: {
                    sceneArea.scene.piService.clone()
                }
            }
        }

        Item{
            id: editPanel
            Layout.fillWidth: true
            Layout.preferredHeight: sceneArea.scene.piService.editing ? parent.height - title.height - 5 :  sceneArea.scene.piService.currentChoosed ? 250 : 0

            Behavior on Layout.preferredHeight { NumberAnimation { duration: 200 }}

            ColumnLayout{
                anchors.fill: parent
                spacing: 0

                Rectangle{
                    id: separatorRect
                    Layout.fillWidth: true
                    Layout.preferredHeight: 5
                    color: "#364554"
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    ColumnLayout{
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottomMargin: 44
                        height: childrenRect.height
                        spacing: 0

                        RowLayout{
                            Layout.preferredHeight: 30
                            Layout.fillWidth: true

                            Text {
                                Layout.fillHeight: true
                                Layout.preferredWidth: 110
                                Layout.leftMargin: 3
                                color :"#6ab2f2"
                                text: "Object Name: "
                                verticalAlignment: Text.AlignVCenter
                            }

                            C.TextField{
                                id: objectNameField
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                Layout.minimumHeight: 30
                                hintColor : "#6ab2f2"
                                color :"#6ab2f2"
                                readOnly: !sceneArea.scene.piService.editing

                                onTextChanged: {
                                    root.somethingChanged = root.checkForChanges()
                                    if (sceneArea.scene.piService.isExist(objectNameField.text) && sceneArea.scene.piService.currentChoosed.objectName !== objectNameField.text)
                                    {
                                        objectNameField.hasError = true
                                        objectNameField.helperText = "name already exists"
                                    }
                                    else
                                    {
                                        objectNameField.hasError = false
                                        objectNameField.helperText = ""
                                    }

                                }
                            }
                        }

                        RowLayout{
                            Layout.preferredHeight: 30
                            Layout.fillWidth: true

                            Text {
                                Layout.fillHeight: true
                                Layout.preferredWidth: 110
                                Layout.leftMargin: 3
                                color :"#6ab2f2"
                                text: "MaxVelocity: "
                                verticalAlignment: Text.AlignVCenter
                            }

                            C.TextField{
                                id: maxVelocityField
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                Layout.minimumHeight: 30
                                hintColor : "#6ab2f2"
                                color :"#6ab2f2"
                                readOnly: !sceneArea.scene.piService.editing
                                onTextChanged: {
                                    root.somethingChanged = root.checkForChanges()
                                }
                            }
                        }

                        RowLayout{
                            Layout.preferredHeight: 30
                            Layout.fillWidth: true

                            Text {
                                Layout.fillHeight: true
                                Layout.preferredWidth: 110
                                Layout.leftMargin: 3
                                text: "MaxDepth: "
                                color :"#6ab2f2"
                                verticalAlignment: Text.AlignVCenter
                            }

                            C.TextField{
                                id: maxDepthField
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                Layout.minimumHeight: 30
                                hintColor : "#6ab2f2"
                                color :"#6ab2f2"
                                readOnly: !sceneArea.scene.piService.editing

                                onTextChanged: {
                                    root.somethingChanged = root.checkForChanges()
                                }
                            }
                        }

                        C.FileDialog{
                            id: imagePath
                            enabled: sceneArea.scene.piService.editing
                            Layout.fillWidth: true
                            Layout.preferredHeight: 30
                            title: "Please choose image"
                            nameFilters: ["SVG Image ( *.svg )", "PNG Image (*.png)", "JPEG Image (*.jpeg, *.jpg)"]
                            defaultSuffix: "svg"
                            folder: shortcuts.home
                            onAccepted: {
                                log("You chose: " + imagePath.fileUrls)
                                //if (sceneArea.scene.piService.currentChoosed)
                                //sceneArea.scene.piService.currentChoosed.imagePath = imagePath.fileUrls
                                somethingChanged = true
                                imagePath.close()
                            }
                            onRejected: {
                                log("Canceled")
                                imagePath.close()
                            }

                            onPathChanged:{
                                if (sceneArea.scene.piService.currentChoosed)
                                sceneArea.scene.piService.currentChoosed.imagePath = path
                                //sceneArea.scene.piService = path
                            }

                            pathPlaceholder: "choose *.svg file"
                        }
                    }
                }
            }
        }
    }

    ColumnLayout{
        id: additionalLayout
        anchors{
            left: root.left
            right: root.right
            leftMargin: 4
            rightMargin: 4
            bottomMargin: 4
            bottom: root.bottom
        }
        height: editPanel.height === 0 ? 0 : 64
        spacing: 2
        property bool removing: false

        RowLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true
            opacity: sceneArea.scene.piService.editing
            enabled : sceneArea.scene.piService.editing

            Behavior on opacity { NumberAnimation { duration : 200 } }
            spacing: 2
            C.Button{
                Layout.fillHeight: true
                Layout.minimumHeight: 30
                Layout.minimumWidth: additionalLayout.removing ?  additionalLayout.width / 2 - 1 : additionalLayout.width
                buttonRadius: 3
                text: additionalLayout.removing ? "CANCEL" : "REMOVE"
                textColor: additionalLayout.removing ? "#1f2936" : "#e94040"
                buttonBackground: "#6ab2f2"
                visible: editPanel.height !== 0
                Behavior on Layout.minimumWidth { NumberAnimation { duration : 100 } }
                Behavior on height { NumberAnimation { duration : 100 } }
                onPressed: {
                    additionalLayout.removing = !additionalLayout.removing
                }

            }

            C.Button{
                Layout.fillHeight: true
                Layout.minimumHeight: 30
                Layout.minimumWidth: additionalLayout.removing ? additionalLayout.width / 2 - 1  : 0
                width: additionalLayout.removing ? additionalLayout.width / 2 - 1  : 0
                buttonRadius: 3
                text: "ACCEPT"
                textColor: additionalLayout.removing ? "#e94040" : "#1f2936"
                buttonBackground: "#6ab2f2"
                visible: additionalLayout.removing && width !== 0
                Behavior on  width { NumberAnimation { duration : 100 } }
                Behavior on height { NumberAnimation { duration : 100 } }
                onPressed:{
                    if (additionalLayout.removing)
                    {
                        additionalLayout.removing = false
                        sceneArea.scene.piService.currentChoosed.removed = true
                        sceneArea.scene.piService.proxy.invalidate()
                        sceneArea.scene.piService.editing = !sceneArea.scene.piService.editing
                        sceneArea.scene.piService.currentChoosed.pressed()
                        sceneArea.scene.piService.readySave = true
                    }
                }
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 2
            C.Button{
                Layout.fillHeight: true
                Layout.minimumHeight: 30
                Layout.minimumWidth: sceneArea.scene.piService.editing && (root.somethingChanged || sceneArea.scene.piService.copying ) ?  additionalLayout.width / 2 - 1 : additionalLayout.width
                buttonRadius: 3
                text: sceneArea.scene.piService.editing ? "CANCEL" : "EDIT"
                textColor: "#1f2936"
                buttonBackground: "#6ab2f2"
                visible: editPanel.height !== 0
                Behavior on Layout.minimumWidth { NumberAnimation { duration : 100 } }
                Behavior on height { NumberAnimation { duration : 100 } }
                onPressed: {
                    //if (sceneArea.scene.piService.editing && root.somethingChanged) sceneArea.scene.piService.load()
                    sceneArea.scene.piService.editing = !sceneArea.scene.piService.editing
                    if (sceneArea.scene.piService.copying ) {
                        sceneArea.scene.piService.cancelClone()
                    }
                    sceneArea.scene.piService.proxy.invalidate()
                }
            }

            C.Button{
                Layout.fillHeight: true
                Layout.minimumHeight: 30
                Layout.minimumWidth: sceneArea.scene.piService.editing && (root.somethingChanged || sceneArea.scene.piService.copying ) ? additionalLayout.width / 2 - 1  : 0
                width: sceneArea.scene.piService.editing && (root.somethingChanged || sceneArea.scene.piService.copying ) ? additionalLayout.width / 2 - 1  : 0
                buttonRadius: 3
                text: "SAVE"
                textColor: "#1f2936"
                buttonBackground: "#6ab2f2"
                visible: sceneArea.scene.piService.editing && width !== 0
                Behavior on  width { NumberAnimation { duration : 100 } }
                Behavior on height { NumberAnimation { duration : 100 } }
                onPressed:{

                    let r = sceneArea.scene.piService.isExist(objectNameField.text)
                    if (!r || objectNameField.text === sceneArea.scene.piService.currentChoosed.objectName)
                    {
                        sceneArea.scene.piService.readySave = true
                        sceneArea.scene.piService.currentChoosed.objectName = objectNameField.text
                        sceneArea.scene.piService.currentChoosed.maxVelocity = parseFloat(maxVelocityField.text)
                        sceneArea.scene.piService.currentChoosed.maxDepth = parseFloat(maxDepthField.text)
                        sceneArea.scene.piService.editing = !sceneArea.scene.piService.editing
                        sceneArea.scene.piService.copying = false
                    }
                    else
                    {
                        objectNameField.hasError = true
                        objectNameField.helperText = "name already exists"
                    }
                }
            }
        }
    }

    function checkForChanges()
    {
        let ch = false
        if (sceneArea.scene.piService.currentChoosed.objectName !== objectNameField.text) ch = true
        if (sceneArea.scene.piService.currentChoosed.maxVelocity !== parseFloat(maxVelocityField.text)) ch = true
        if (sceneArea.scene.piService.currentChoosed.maxDepth !== parseFloat(maxDepthField.text)) ch = true
        return ch
    }

    function resetAllStates(){
        sceneArea.scene.piService.editing = false
        if (sceneArea.scene.piService.copying) { sceneArea.scene.piService.cancelClone() }
        sceneArea.scene.piService.copying = false
        if (sceneArea.scene.piService.currentChoosed) sceneArea.scene.piService.currentChoosed.pressed()
        root.somethingChanged = false
        additionalLayout.removing = false
    }
}

