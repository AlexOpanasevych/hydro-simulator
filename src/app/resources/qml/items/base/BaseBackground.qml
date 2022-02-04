import QtQuick 2.15
import QtGraphicalEffects 1.0


Item {
    id: item


    property int elevation: 0
    property real radius: 0
    property real mult: 1

    property string style: "default"

    property color backgroundColor: elevation > 0 ? "white" : "transparent"
    property color tintColor: "transparent"

    property alias border: rect.border

    property bool fullWidth
    property bool fullHeight

    property alias clipContent: rect.clip

    //default property alias data: rect.data

    property bool elevationInverted: false

    property var topShadow: [
        {
            "opacity": 0,
            "offset": 0,
            "blur": 0
        },

        {
            "opacity": 0.12,
            "offset": 1 * mult,
            "blur": 1.5 * mult
        },

        {
            "opacity": 0.16,
            "offset": 3 * mult,
            "blur": 3 * mult
        },

        {
            "opacity": 0.19,
            "offset": 10 * mult,
            "blur": 10 * mult
        },

        {
            "opacity": 0.25,
            "offset": 14 * mult,
            "blur": 14 * mult
        },

        {
            "opacity": 0.30,
            "offset": 19 * mult,
            "blur": 19 * mult
        }
    ]

    property var bottomShadow: [
        {
            "opacity": 0,
            "offset": 0 * mult,
            "blur": 0 * mult
        },

        {
            "opacity": 0.24,
            "offset": 1 * mult,
            "blur": 1 * mult
        },

        {
            "opacity": 0.23,
            "offset": 3 * mult,
            "blur": 3 * mult
        },

        {
            "opacity": 0.23,
            "offset": 6 * mult,
            "blur": 3 * mult
        },

        {
            "opacity": 0.22,
            "offset": 10 * mult,
            "blur": 5 * mult
        },

        {
            "opacity": 0.22,
            "offset": 15 * mult,
            "blur": 6 * mult
        }
    ]

    RectangularGlow {
        property var elevationInfo: bottomShadow[Math.min(elevation, 5)]
        property real horizontalShadowOffset: elevationInfo.offset * Math.sin((2 * Math.PI) * (parent.rotation / 360.0))
        property real verticalShadowOffset: elevationInfo.offset * Math.cos((2 * Math.PI) * (parent.rotation / 360.0))

        anchors.centerIn: parent
        width: parent.width + (fullWidth ? 10 * mult : 0)
        height: parent.height + (fullHeight ? 20 * mult : 0)
        anchors.horizontalCenterOffset: horizontalShadowOffset * (elevationInverted ? -1 : 1)
        anchors.verticalCenterOffset: verticalShadowOffset * (elevationInverted ? -1 : 1)
        glowRadius: elevationInfo.blur
        opacity: elevationInfo.opacity
        spread: 0.05
        color: "black"
        cornerRadius: item.radius + glowRadius * 2.5
        //visible: parent.opacity == 1
    }

    RectangularGlow {
        property var elevationInfo: topShadow[Math.min(elevation, 5)]
        property real horizontalShadowOffset: elevationInfo.offset * Math.sin((2 * Math.PI) * (parent.rotation / 360.0))
        property real verticalShadowOffset: elevationInfo.offset * Math.cos((2 * Math.PI) * (parent.rotation / 360.0))

        anchors.centerIn: parent
        width: parent.width + (fullWidth ? 10 * mult : 0)
        height: parent.height + (fullHeight ? 20 * mult : 0)
        anchors.horizontalCenterOffset: horizontalShadowOffset * (elevationInverted ? -1 : 1)
        anchors.verticalCenterOffset: verticalShadowOffset * (elevationInverted ? -1 : 1)
        glowRadius: elevationInfo.blur
        opacity: elevationInfo.opacity
        spread: 0.05
        color: "black"
        cornerRadius: item.radius + glowRadius * 2.5
        //visible: parent.opacity == 1
    }

    Rectangle {
        id: rect
        anchors.fill: parent
        color: Qt.tint(backgroundColor, tintColor)
        radius: item.radius
        antialiasing: parent.rotation || radius > 0 ? true : false
        clip: true

        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }
}
