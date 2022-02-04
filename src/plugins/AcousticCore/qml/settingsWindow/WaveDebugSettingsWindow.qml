import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.15

import CQML 1.0 as C
import AcousticCore 1.0

Rectangle{
    color: "#1f2936"
    property string type: ""

    ColumnLayout{
        anchors.fill: parent
        C.CheckBox{
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            text: "Create Wave"
            onCheckStateChanged: {
                AcousticSimulationCore.waveCreationDebug.creatingWave = checked
            }
        }
        Text{
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            text: "Start Angle"
        }
        C.Slider{
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            from: 0
            to: 360
            value: 0
            stepSize: 0.1
            onValueChanged: {
                AcousticSimulationCore.waveCreationDebug.startAngle = value
            }
        }
        Text{
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            text: "ARC Angle"
        }
        C.Slider{
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            from: 0
            to: 360
            value: 0
            stepSize: 0.1
            onValueChanged: {
                AcousticSimulationCore.waveCreationDebug.arcAngle = value
            }
        }
        Text{
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            text: "Line Width"
        }

        C.Slider{
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            from: 0
            to: 1000
            value: 0
            stepSize: 1
            onValueChanged: {
                AcousticSimulationCore.waveCreationDebug.lineWidth = value
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }
}

