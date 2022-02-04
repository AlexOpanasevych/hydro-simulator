import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

import KLib 1.0

Window {

    height: 300
    width: 300

    Rectangle {
        color: "#091A32"
        anchors.fill: parent
        ColumnLayout {
            anchors.fill: parent

            Repeater {
                id: dataRepeater
                model: [
                    "board: " + SimDataManager.board,
                    "distance: " + SimDataManager.distance,
                    "courseAngle: " + SimDataManager.courseAngle,
                    "bearing: " + SimDataManager.pelengAngle,
                    "speed: " + SimDataManager.speed
                ]

                Text {
                    id: text
                    Layout.fillWidth: true
                    height: 10
                    text: modelData
                    color: "white"
                }

            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
}
