import QtQuick 2.15
import QtGraphicalEffects 1.0

Item {
    id: item
    property alias source: mask.source
    property alias radius: ripple.radius

    Rectangle {
        id: ripple
        anchors.fill: parent
        smooth: true
        visible: false
        radius: Math.max(width / 2, height / 2)
    }

    OpacityMask {
        id: mask
        anchors.fill: parent
        maskSource: ripple
    }
}

