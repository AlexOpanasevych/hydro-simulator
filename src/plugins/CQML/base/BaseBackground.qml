import QtQuick 2.15
import QtGraphicalEffects 1.0

Item {
    id: item
    property int elevation: 0
    property real radius: 0
    property bool inverted: false
    property string style: "default"
    property alias childRect: rect.childrenRect
    property color backgroundColor: "transparent"
    property color tintColor: "transparent"
    property bool internalShadow: false
    property bool fast: false

    property alias border: rect.border

    property bool fullWidth
    property bool fullHeight
    antialiasing: radius > 0

    property alias clipContent: rect.clip

    default property alias res: rect.data

    property bool elevationInverted: false

    property var topShadow: [
        {
            "opacity": 0,
            "offset": 0,
            "blur": 0
        },

        {
            "opacity": 0.12,
            "offset": 1  * inverted ? -1 : 1,
            "blur": 1.5
        },

        {
            "opacity": 0.16,
            "offset": 3  * inverted ? -1 : 1,
            "blur": 3
        },

        {
            "opacity": 0.19,
            "offset": 10 * inverted ? -1 : 1,
            "blur": 10
        },

        {
            "opacity": 0.25,
            "offset": 14  * inverted ? -1 : 1,
            "blur": 14
        },

        {
            "opacity": 0.30,
            "offset": 19  * inverted ? -1 : 1,
            "blur": 19
        }
    ]

    property var bottomShadow: [
        {
            "opacity": 0,
            "offset": 0,
            "blur": 0
        },

        {
            "opacity": 0.24,
            "offset": 1 * inverted ? -1 : 1,
            "blur": 1
        },

        {
            "opacity": 0.23,
            "offset": 3 * inverted ? -1 : 1,
            "blur": 3
        },

        {
            "opacity": 0.23,
            "offset": 6 * inverted ? -1 : 1,
            "blur": 3
        },

        {
            "opacity": 0.22,
            "offset": 10 * inverted ? -1 : 1,
            "blur": 5
        },

        {
            "opacity": 0.22,
            "offset": 15 * inverted ? -1 : 1,
            "blur": 6
        }
    ]

    RectangularGlow {
        property var elevationInfo: bottomShadow[Math.min(elevation, 5)]
        property real horizontalShadowOffset: elevationInfo.offset * Math.sin((2 * Math.PI) * (parent.rotation / 360.0))
        property real verticalShadowOffset: elevationInfo.offset * Math.cos((2 * Math.PI) * (parent.rotation / 360.0))

        anchors.centerIn: parent
        width: parent.width + (fullWidth ? 10 : 0)
        height: parent.height + (fullHeight ? 20 : 0)
        anchors.horizontalCenterOffset: horizontalShadowOffset * (elevationInverted ? -1 : 1)
        anchors.verticalCenterOffset: verticalShadowOffset * (elevationInverted ? -1 : 1)
        glowRadius: elevationInfo.blur
        opacity: elevationInfo.opacity
        spread: 0.05
        color: "black"
        cornerRadius: item.radius + glowRadius * 2.5
        visible: parent.opacity >= 0.5 && parent.visible && !internalShadow
    }

    RectangularGlow {
        property var elevationInfo: topShadow[Math.min(elevation, 5)]
        property real horizontalShadowOffset: elevationInfo.offset * Math.sin((2 * Math.PI) * (parent.rotation / 360.0))
        property real verticalShadowOffset: elevationInfo.offset * Math.cos((2 * Math.PI) * (parent.rotation / 360.0))

        anchors.centerIn: parent
        width: parent.width + (fullWidth ? 10 : 0)
        height: parent.height + (fullHeight ? 20 : 0)
        anchors.horizontalCenterOffset: horizontalShadowOffset * (elevationInverted ? -1 : 1)
        anchors.verticalCenterOffset: verticalShadowOffset * (elevationInverted ? -1 : 1)
        glowRadius: elevationInfo.blur
        opacity: elevationInfo.opacity
        spread: 0.05
        color: "black"
        cornerRadius: item.radius + glowRadius * 2.5
        visible: parent.opacity >= 0.5 && parent.visible && !internalShadow
    }

    Rectangle {
        id: rect
        anchors.fill: parent
        color: Qt.tint(backgroundColor, tintColor)
        radius: item.radius
        antialiasing: parent.rotation || radius > 0 ? true : false
        //visible: !internalShadow
        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }

    InnerShadow{
        anchors.fill: rect
        fast: item.fast
        property var elevationInfo: topShadow[Math.min(elevation, 5)]
        property real horizontalShadowOffset: elevationInfo.offset * Math.sin((2 * Math.PI) * (parent.rotation / 360.0))
        property real verticalShadowOffset: elevationInfo.offset * Math.cos((2 * Math.PI) * (parent.rotation / 360.0))

        spread:  0.05
        radius: 8.0
        samples: 24
        horizontalOffset: elevation
        verticalOffset: -elevation
        color: "black"
        source: rect
        opacity: elevationInfo.opacity
        visible: parent.opacity >= 0.5 && parent.visible && internalShadow && elevationInfo.opacity !== 0
    }

    InnerShadow{
        anchors.fill: rect
        fast: item.fast
        property var elevationInfo: bottomShadow[Math.min(elevation, 5)]
        property real horizontalShadowOffset: elevationInfo.offset * Math.sin((2 * Math.PI) * (parent.rotation / 360.0))
        property real verticalShadowOffset: elevationInfo.offset * Math.cos((2 * Math.PI) * (parent.rotation / 360.0))

        spread:  0.05
        radius: 8.0
        samples: 24
        horizontalOffset: -elevation
        verticalOffset: elevation
        color: "black"
        source: rect
        opacity: elevationInfo.opacity
        visible: parent.opacity >= 0.5 && parent.visible && internalShadow && elevationInfo.opacity !== 0
    }
}
