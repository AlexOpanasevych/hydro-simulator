import QtQuick 2.0
import KLib 1.0
import QtGraphicalEffects 1.15

Item {

    property var modelData;
    x: modelData.x
    y: modelData.y
    height: modelData.height
    width: modelData.width

//    Rectangle{
//        id: mask
//        anchors.fill: parent
//        radius: width / 2
//        color: "transparent"
//    }

    MGKCircleMonitorPrivate {
        anchors.fill: parent
        backgroundColor: "#808080"
//        layer.enabled: true
//        layer.effect: OpacityMask {
//            maskSource: mask
//        }
    }

    function updateHScale(scale) {
        y *= scale
        height *= scale
    }

    function updateWScale(scale) {
        x *= scale
        width *= scale
    }
}
