import QtQuick 2.0
import KLib 1.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    property var modelData;
    x: modelData.x
    y: modelData.y
    height: modelData.height
    width: modelData.width

    MGKRectMonitorPrivate {
        id: monitorPrivate
        anchors.fill: parent
        backgroundColor: modelData.additionalQml.backgroundColor !== undefined  ? modelData.additionalQml.backgroundColor : backgroundColor
//        color: modelData.color !== Qt.rgba(0, 0, 0, 1) ? modelData.color : color
        afterShiningIntencity: modelData.additionalQml.afterShiningIntencity !== undefined ? modelData.additionalQml.afterShiningIntencity : afterShiningIntencity
        onColorChanged: {
            console.log(color);
        }
    }

    Connections {
        target: modelData
        function onValueChanged(value) {
            console.log("---------------------------")
            console.log("received:", JSON.stringify(value))
            monitorPrivate.brightness = value.brightness !== undefined ? value.brightness : monitorPrivate.brightness
            monitorPrivate.amplifier = value.amplifier !== undefined ? value.amplifier : monitorPrivate.amplifier

            if(value.scale !== undefined)
                monitorPrivate.searchScale = parseInt(value.scale)

            monitorPrivate.searchAngleMode = value.searchAngleMode !== undefined ? value.searchAngleMode : monitorPrivate.searchAngleMode
            monitorPrivate.boardMode = value.lbpb !== undefined ? value.lbpb : monitorPrivate.boardMode

            monitorPrivate.epMode = value.epMode !== undefined ? value.epMode : monitorPrivate.epMode
            monitorPrivate.screenCursor.changePosition(value)
            if(value.soundSpeed !== undefined)
                monitorPrivate.soundSpeed = KMath.dmap(value.soundSpeed, 0, 1, 1415, 1535)
            if(value.monitorWork !== undefined)
                monitorPrivate.monitorWork = value.monitorWork

            if(value.dpbp !== undefined)
                monitorPrivate.searchMode = value.dpbp
        }
    }

    function updateHScale(scale) {
        y *= scale
        height *= scale
    }

    function updateWScale(scale) {
        x *= scale
        width *= scale
    }

    ColumnLayout {
        height: 100
        width: 100
        anchors.left: parent.right
        anchors.leftMargin: 10

        Repeater {
            model: ["brightness " + monitorPrivate.brightness,
                "amplifier " + monitorPrivate.amplifier,
                "searchScale " + monitorPrivate.searchScale,
                "searchAngleMode " + monitorPrivate.searchAngleMode,
                "boardMode " + monitorPrivate.boardMode,
                "epMode " + monitorPrivate.epMode,
                "soundSpeed " + monitorPrivate.soundSpeed,
                "screenCursor.x " + monitorPrivate.screenCursor.x,
                "screenCursor.y " + monitorPrivate.screenCursor.y,
                "screenCursor.brightness " + monitorPrivate.screenCursor.cursorBrightness,
                "maxDistance" + monitorPrivate.maxDistance,
                "maxTime" + monitorPrivate.maxTime
//                "" + monitorPrivate.screenCursor.y,
//                "screenCursor.y " + monitorPrivate.screenCursor.y,
//                "screenCursor.y " + monitorPrivate.screenCursor.y,
//                "screenCursor.y " + monitorPrivate.screenCursor.y,
            ]
            delegate: Text {
                text: modelData
            }
        }
    }
}
