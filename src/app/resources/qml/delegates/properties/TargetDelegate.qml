import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import KLib 1.0

import "kdelegates"
import "trajectory"

Rectangle {
    color: "#1f2936"

    WarningArea {
        id: warningArea

        trueFunc: () => { return  (sceneArea.currentItem === sceneArea.scene ) }
        falseFunc: () => { return (sceneArea.currentItem !== sceneArea.scene ) }
        warningText: "Choose Item"
        sourceItem: layouts
    }

    ColumnLayout{
        id: layouts
        anchors.fill: parent

        Text {
            id: typeText
            Layout.fillWidth: true
            Layout.minimumHeight: 60
            color: "#6ab3f3"
            text: "TARGET: " + sceneArea.currentItem.type
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

        Repeater{
            model: sceneArea.currentItem
            delegate: Loader{
                Layout.fillHeight: true
                Layout.fillWidth: true
                sourceComponent: KDelegateChooser{
                    anchors.fill: parent
                    delegates: [ ShipKDelegate{ anchors.fill: parent}, TorpedoKDelegate{ anchors.fill: parent }, SubmarineKDelegate{ anchors.fill: parent } ]
                }
                onLoaded: console.log("loaded")
            }
            onModelChanged: console.log("modelChanged")
        }

    }

}
