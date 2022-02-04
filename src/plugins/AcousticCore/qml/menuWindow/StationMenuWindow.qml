import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.15

import CQML 1.0 as C
import AcousticCore 1.0
import KLib 1.0
import PluginsBase 1.0

Rectangle{
    color: "#1f2936"
    property string type: ""

    MouseArea{
        anchors.fill: parent
        onWheel: {}
        preventStealing: true
    }

    Connections{
        target: menu
        function onCurrentIndexChanged(){
            if (menu.currentIndex === index){
                sceneArea.workMode = "STATION_LOOK"
            }else if (sceneArea.workMode === "STATION_LOOK"){
                sceneArea.workMode = "DEFAULT"
            }
        }
        function onMenuOpened(idx){
            if (idx === index){
                sceneArea.workMode = "STATION_LOOK"
            }else if (sceneArea.workMode === "STATION_LOOK"){
                sceneArea.workMode = "DEFAULT"
            }
        }

        function onMenuClosed(){
            if (sceneArea.workMode === "STATION_LOOK"){
                sceneArea.workMode = "DEFAULT"
            }
        }
    }

    ColumnLayout{
        anchors.fill: parent
        spacing: 0

        Text {
            id: typeText
            Layout.fillWidth: true
            Layout.minimumHeight: 40
            color: "#6ab3f3"
            text: ("Stations").toUpperCase()
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

        ListView{
            id: stationsListView
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            Layout.topMargin: 5
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            Layout.bottomMargin: 5
            spacing: 5
            model: AcousticSimulationCore.stationsCore.stationsModel
            delegate: C.BaseBackground{
                elevation: 5
                internalShadow: true
                width: stationsListView.width
                height: 100
                backgroundColor: "#1f2936"

                MouseArea{
                    anchors.fill: parent
                    onReleased: {
                        if (AcousticSimulationCore.stationsCore.currentItem === modelData){
                            AcousticSimulationCore.stationsCore.currentItem = null
                        }else{
                            AcousticSimulationCore.stationsCore.currentItem = modelData
                            sceneRect.moveToItem(AcousticSimulationCore.stationsCore.currentItem.sceneItem)
                        }
                    }
                }

                Rectangle{
                    anchors.fill: parent
                    color: "lightblue"
                    opacity: AcousticSimulationCore.stationsCore.currentItem === modelData ? 0.3 : 0
                    Behavior on opacity { NumberAnimation { duration: 200 } }
                }

                ColumnLayout{
                    anchors.fill: parent
                    anchors.margins: 5
                    Text{
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.minimumHeight: 30
                        color: "#6ab3f3"
                        text: "type: " + (modelData.mode === StationData.BRONZE ? "Bronze" : "Platina")
                    }

                    Text{
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.minimumHeight: 30
                        color: "#6ab3f3"
                        text: "xnCount: " + modelData.xnCount
                    }

                    Text{
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.minimumHeight: 30
                        color: "#6ab3f3"
                        text: "Rotation: " + modelData.stationRotation
                    }
                    Item {
                        Layout.fillHeight: true
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 4
            Layout.rightMargin: 4
            Layout.bottomMargin: 4
            Layout.maximumHeight: 30
        }
    }

    C.BaseBackground{
        id: addStationPage
        property real topOffset: sceneArea.workMode !== "STATION_ADD" ? stationsListView.count > 0 ? parent.height - 80 : parent.height - 40 : typeText.height
        property var sceneItem
        backgroundColor: "#1f2936"
        anchors.fill: parent
        anchors.topMargin: topOffset
        visible: sceneArea.scene.scenePlayer.mode === 2 // KScenePlayer.stop
        Behavior on topOffset { NumberAnimation { duration: 200 } }

        ColumnLayout{
            anchors.fill: parent
            anchors.topMargin: sceneArea.workMode === "STATION_ADD" ? 10 : 0
            anchors.bottomMargin:  sceneArea.workMode === "STATION_ADD" ? 10 : 5
            spacing: 0
            C.Button {
                id: buttonAddStation
                text: "ADD STATION"
                textColor: "#1f2936"
                buttonBackground: "#6ab2f2"
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 5
                Layout.rightMargin: 5
                Layout.topMargin: 5
                buttonRadius: 5
                Layout.maximumHeight: 30
                Layout.minimumHeight: 30
                visible: sceneArea.workMode !== "STATION_ADD"
                onPressed: {
                    sceneArea.workMode = "STATION_ADD"
                }
            }

            C.Button {
                id: clearStation
                text: "CLEAR STATIONS"
                textColor: "#1f2936"
                buttonBackground: "#6ab2f2"
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 5
                Layout.rightMargin: 5
                Layout.topMargin: 5
                buttonRadius: 5
                Layout.maximumHeight:  stationsListView.count > 0 && sceneArea.workMode !== "STATION_ADD" ? 30 : 0
                Layout.minimumHeight: stationsListView.count > 0 && sceneArea.workMode !== "STATION_ADD" ? 30 : 0
                visible: stationsListView.count > 0 && sceneArea.workMode !== "STATION_ADD"
                onPressed: {
                    AcousticSimulationCore.stationsCore.clearStations()
                    buttonConfirm.enabled = !AcousticSimulationCore.stationsCore.hasStation(stationTypeCombobox.displayText, addStationPage.sceneItem) && (typeof(addStationPage.sceneItem) !== 'undefined')
                }
            }

            RowLayout{
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 5
                Layout.rightMargin: 5
                Layout.maximumHeight: sceneArea.workMode === "STATION_ADD" ? 40 : 0
                visible: sceneArea.workMode === "STATION_ADD"
                Text{
                    Layout.fillHeight: true
                    Layout.preferredWidth: paintedWidth
                    text: "Type: "
                    verticalAlignment: Text.AlignVCenter
                    color: "#6ab2f2"
                }
                C.ComboBox{
                    id: stationTypeCombobox
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    model: ["bronze", "platina"]
                    backgroundColor: "#1f2936"
                    internalShadow: true
                    elevation: 3
                    onDisplayTextChanged: {
                        buttonConfirm.enabled = !AcousticSimulationCore.stationsCore.hasStation(stationTypeCombobox.displayText, addStationPage.sceneItem) && (typeof(addStationPage.sceneItem) !== 'undefined')
                    }
                }
            }

            RowLayout{
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumHeight: sceneArea.workMode === "STATION_ADD" ? 40 : 0
                Layout.leftMargin: 5
                Layout.rightMargin: 5
                visible: sceneArea.workMode === "STATION_ADD"
                Text{
                    Layout.fillHeight: true
                    Layout.preferredWidth: paintedWidth
                    text: "Item: "
                    verticalAlignment: Text.AlignVCenter
                    color: "#6ab2f2"
                }
                Text{
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    text: addStationPage.sceneItem ? addStationPage.sceneItem.type : ""
                    verticalAlignment: Text.AlignVCenter
                    color: "#6ab2f2"
                }
            }

            C.Button {
                id: buttonConfirm
                text: "CONFIRM"
                textColor: "#1f2936"
                buttonBackground: "#6ab2f2"
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 5
                Layout.rightMargin: 5
                Layout.topMargin: 5
                buttonRadius: 5
                enabled: false
                Layout.maximumHeight: 30
                visible: sceneArea.workMode === "STATION_ADD"
                onPressed: {
                    AcousticSimulationCore.stationsCore.addStation(stationTypeCombobox.displayText, addStationPage.sceneItem)
                    buttonConfirm.enabled = false
                    addStationPage.sceneItem = null
                    sceneArea.currentItem = sceneArea.scene
                    sceneArea.workMode = "STATION_LOOK"
                }
            }

            C.Button {
                id: buttonCancel
                text: "CANCEL"
                textColor: "#1f2936"
                buttonBackground: "#6ab2f2"
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 5
                Layout.rightMargin: 5
                Layout.topMargin: 5
                buttonRadius: 5
                Layout.maximumHeight: 30
                visible: sceneArea.workMode === "STATION_ADD"
                onPressed: {
                    buttonConfirm.enabled = false
                    addStationPage.sceneItem = null
                    sceneArea.workMode = "STATION_LOOK"
                }
            }

            Item{
                Layout.maximumHeight: sceneArea.workMode === "STATION_ADD" ? parent.height : 0
                Layout.fillHeight: true
            }
        }
    }

    Connections{
        target: sceneArea
        function onCurrentItemChanged(){
            var item = sceneArea.currentItem
            if (item === sceneArea.scene){
                buttonConfirm.enabled = false
                addStationPage.sceneItem = undefined
                if (sceneArea.workMode === "STATION_ADD")
                    sceneArea.workMode = "STATION_LOOK"
            }else{
                addStationPage.sceneItem = AcousticSimulationCore.cast(item)
                buttonConfirm.enabled = !AcousticSimulationCore.stationsCore.hasStation(stationTypeCombobox.displayText, addStationPage.sceneItem) && item
            }
        }
    }
}
