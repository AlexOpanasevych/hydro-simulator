import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import KLib 1.0
import CQML 1.0 as C
import QtQml 2.15

Item {
    id: frameWindow
    Drag.active: frameWindow.held && frameWindow.state === "scene"
    Drag.source: source
    Drag.hotSpot.x: dragArea.previousX
    Drag.hotSpot.y: dragArea.previousY
    clip: true
    readonly property bool resizing: content.resizing
    readonly property alias source: dragArea
    property Component component
    property bool held: false
    property bool frameAvailable: windowLoader.item ? windowLoader.item.hasOwnProperty("frameAvailable") ? windowLoader.item.frameAvailable : true : true
    property bool enteredHome: true
    property real minumumWidth: 60
    property real minumumHeight: 30
    property real savedWidth: 0
    property real savedHeight: 0
    property bool preparing: false
    state: "default"

    Binding{
        target: frameWindow
        when: frameWindow.state === "default"
        property: "width"
        value: menu.windowWidth
        //@disable-check M16
        restoreMode : Binding.RestoreBinding
    }

    Binding{
        target: frameWindow
        when: frameWindow.state === "default"
        property: "height"
        value: menu.windowHeight - 40
        //@disable-check M16
        restoreMode : Binding.RestoreBinding
    }
    Behavior on width { enabled: !resizing; NumberAnimation { duration: 150 } }
    Behavior on height { enabled: !resizing; NumberAnimation { duration: 150 } }

    states: [
        State {
            name: "scene";
            when: frameWindow.held
            ParentChange {
                target: frameWindow;
                parent: sceneArea;
            }
            onCompleted: {
                if (frameWindow.savedHeight === 0){
                    frameWindow.height = 300
                }else{
                    frameWindow.height = frameWindow.savedHeight
                }
                if (frameWindow.savedWidth === 0){
                    frameWindow.width = 200
                }else{
                    frameWindow.width = frameWindow.savedWidth
                }
            }
        },
        State{
            name: "default"
            when: !frameWindow.held
            onCompleted: {
                frameWindow.height = menu.windowHeight - 40
                frameWindow.width = menu.windowWidth
            }
        },
        State{
            name: "window"
            ParentChange {
                target: frameWindow;
                parent: container.win ? container.win.internalParent : container
            }
            onCompleted: { frameWindow.x = 0; frameWindow.y = 0 }
        }
    ]

    transitions: [
        Transition {
            from: "scene"
            to: "default"
            NumberAnimation{
                target: frameWindow
                properties: "x,y"
                duration: 300
            }
        }
    ]

    Component.onCompleted: {
        savedHeight = menu.height
        savedWidth = menu.windowWidth
    }

    Item{
        id: content
        width: frameWindow.width
        height: frameWindow.height
        enabled: frameWindow.state !== "window" && frameWindow.frameAvailable
        property real previousX: 0
        property real previousY: 0
        property bool resizing: false

        C.BaseBackground{
            anchors.fill: parent
            backgroundColor: "transparent"
            elevation: 5
            visible: frameWindow.state === "scene"
        }

        MouseArea{
            anchors.fill: parent
            anchors.topMargin: 20
            preventStealing: true
            onWheel: {}
        }
        //! left edge
        MouseArea{
            enabled: frameWindow.state !== "default" && frameWindow.state !== "window"
            anchors{
                left: parent.left
                top: parent.top
                bottom: parent.bottom
                topMargin: 4
                bottomMargin: 4
                leftMargin: -2
            }
            cursorShape: Qt.SizeHorCursor
            width: 4

            onPressed: {
                content.resizing = true
                content.previousX = mouseX
            }

            onReleased: {
                content.resizing = false
            }

            onMouseXChanged: {
                var dx = mouseX - content.previousX
                if (frameWindow.width - dx >= minumumWidth){
                    frameWindow.x += dx
                    frameWindow.width -= dx
                }
            }
        }
        //! right edge
        MouseArea{
            enabled: frameWindow.state !== "default" && frameWindow.state !== "window"
            anchors{
                right: parent.right
                top: parent.top
                bottom: parent.bottom
                topMargin: 4
                bottomMargin: 4
                rightMargin: -2
            }
            cursorShape: Qt.SizeHorCursor
            width: 4

            onPressed: {
                content.resizing = true
                content.previousX = mouseX
            }

            onReleased: {
                content.resizing = false
            }

            onMouseXChanged: {
                var dx = mouseX - content.previousX
                if (frameWindow.width + dx >= minumumWidth){
                    frameWindow.width += dx
                }
            }
        }
        //! top edge
        MouseArea{
            enabled: frameWindow.state !== "default" && frameWindow.state !== "window"
            anchors{
                right: parent.right
                left: parent.left
                top: parent.top
                topMargin: -2
                rightMargin: 4
                leftMargin: 4
            }
            cursorShape: Qt.SizeVerCursor
            height: 4

            onPressed: {
                content.resizing = true
                content.previousY = mouseY
            }

            onReleased: {
                content.resizing = false
            }

            onMouseYChanged: {
                var dy = mouseY - content.previousY
                if (frameWindow.height - dy >= frameWindow.minumumHeight){
                    frameWindow.y += dy
                    frameWindow.height -= dy
                }
            }
        }
        //! bottom edge
        MouseArea{
            enabled: frameWindow.state !== "default" && frameWindow.state !== "window"
            anchors{
                right: parent.right
                left: parent.left
                bottom: parent.bottom
                rightMargin: 4
                leftMargin: 4
                bottomMargin: -2
            }
            cursorShape: Qt.SizeVerCursor
            height: 4

            onPressed: {
                content.previousY = mouseY
                content.resizing = true
            }

            onReleased: {
                content.resizing = false
            }

            onMouseYChanged: {
                var dy = mouseY - content.previousY
                //console.log(dy)
                if (frameWindow.height + dy >= frameWindow.minumumHeight){
                    frameWindow.height += dy
                }
            }
        }
        //! corner topLeft
        MouseArea{
            enabled: frameWindow.state !== "default" && frameWindow.state !== "window"
            anchors{
                left: parent.left
                top: parent.top
                topMargin: -4
                leftMargin: -4
            }
            cursorShape: Qt.SizeFDiagCursor
            height: 8
            width: 8

            onPressed: {
                content.previousY = mouseY
                content.previousX = mouseX
                content.resizing = true
            }

            onReleased: {
                content.resizing = false
            }

            onMouseYChanged: {
                var dy = mouseY - content.previousY
                if (frameWindow.height - dy >= frameWindow.minumumHeight){
                    frameWindow.y += dy
                    frameWindow.height -= dy
                }
            }

            onMouseXChanged: {
                var dx = mouseX - content.previousX
                if (frameWindow.width - dx >= minumumWidth){
                    frameWindow.x += dx
                    frameWindow.width -= dx
                }
            }
        }
        //! corner top right
        MouseArea{
            enabled: frameWindow.state !== "default" && frameWindow.state !== "window"
            anchors{
                right: parent.right
                top: parent.top
                topMargin: -4
                rightMargin: -4
            }
            cursorShape: Qt.SizeBDiagCursor
            height: 8
            width: 8

            onPressed: {
                content.previousY = mouseY
                content.previousX = mouseX
                content.resizing = true
            }

            onReleased: {
                content.resizing = false
            }

            onMouseYChanged: {
                var dy = mouseY - content.previousY
                if (frameWindow.height - dy >= frameWindow.minumumHeight){
                    frameWindow.y += dy
                    frameWindow.height -= dy
                }
            }

            onMouseXChanged: {
                var dx = mouseX - content.previousX
                if (frameWindow.width + dx >= minumumWidth){
                    frameWindow.width += dx
                }
            }
        }
        //! corner bottom left
        MouseArea{
            enabled: frameWindow.state !== "default" && frameWindow.state !== "window"
            anchors{
                left: parent.left
                bottom: parent.bottom
                bottomMargin: -4
                leftMargin: -4
            }
            cursorShape: Qt.SizeBDiagCursor
            height: 8
            width: 8

            onPressed: {
                content.previousY = mouseY
                content.previousX = mouseX
                content.resizing = true
            }

            onReleased: {
                content.resizing = false
            }

            onMouseYChanged: {
                var dy = mouseY - content.previousY
                //console.log(dy)
                if (frameWindow.height + dy >= frameWindow.minumumHeight){
                    frameWindow.height += dy
                }
            }

            onMouseXChanged: {
                var dx = mouseX - content.previousX
                if (frameWindow.width - dx >= minumumWidth){
                    frameWindow.x += dx
                    frameWindow.width -= dx
                }
            }
        }
        //! corner borrom right
        MouseArea{
            enabled: frameWindow.state !== "default" && frameWindow.state !== "window"
            anchors{
                right: parent.right
                bottom: parent.bottom
                bottomMargin: -4
                rightMargin: -4
            }
            cursorShape: Qt.SizeFDiagCursor
            height: 8
            width: 8

            onPressed: {
                content.previousY = mouseY
                content.previousX = mouseX
                content.resizing = true
            }

            onReleased: {
                content.resizing = false
            }

            onMouseYChanged: {
                var dy = mouseY - content.previousY
                //console.log(dy)
                if (frameWindow.height + dy >= frameWindow.minumumHeight){
                    frameWindow.height += dy
                }
            }

            onMouseXChanged: {
                var dx = mouseX - content.previousX
                if (frameWindow.width + dx >= minumumWidth){
                    frameWindow.width += dx
                }
            }
        }
    }

    Loader{
        id: windowLoader
        anchors.fill: parent
        anchors.topMargin: dragArea.height === 0 ? 0 : dragArea.height + 2
        opacity: (frameWindow.state === "scene" && frameWindow.preparing) || dragArea.drag.active ? 0.3 : 1
        //enabled: frameWindow.component
        sourceComponent: frameWindow.component

        Binding{
            target: windowLoader.item
            when: windowLoader.enabled
            property: "width"
            value: frameWindow.width
        }
        Binding{
            target: windowLoader.item
            when: windowLoader.enabled
            property: "height"
            value: frameWindow.height
        }
    }

    Text{
        id: windowWarningText
        anchors.fill: parent
        text: "Separate Window Mode"
        font.pointSize: 17
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        opacity: frameWindow.preparing && frameWindow.state === "scene"
        wrapMode: Text.WordWrap
        Behavior on opacity { NumberAnimation { duration: 200 } }
    }

    MouseArea{
        id: dragArea
        enabled: frameWindow.state !== "window" && frameWindow.frameAvailable
        y: 2
        width: parent.width
        height: frameWindow.frameAvailable ? frameWindow.state === "window" ? 0 : 18 : 0
        visible: height !== 0
        drag.target: frameWindow.held ? frameWindow : undefined
        drag.axis: Drag.XAndYAxis
        drag.minimumX: 0
        drag.maximumX: sceneArea.width - frameWindow.width
        drag.minimumY: 0
        drag.maximumY: sceneArea.height - frameWindow.height
        cursorShape: Qt.SizeAllCursor
        property real previousX: 0
        property real previousY: 0

        onPressed: {
            previousX = mouseX
            previousY = mouseY
            frameWindow.held = true
        }

        onMouseXChanged: {
            if ((mouseX < 0 || mouseX > frameWindow.width) && frameWindow.state === "scene"){
                 frameWindow.preparing = true
            }else{
                 frameWindow.preparing = false
            }
            console.log("X", mouseX, frameWindow.width, frameWindow.preparing)
        }

        onMouseYChanged: {
            if ((mouseY < 0 || mouseY > frameWindow.height) && frameWindow.state === "scene"){
                 frameWindow.preparing = true
            }else if ((mouseX < 0 || mouseX > frameWindow.width) && frameWindow.state === "scene"){
                frameWindow.preparing = true
           }else{
                frameWindow.preparing = false
           }
        }

        //fixed scene wheel effect
        onWheel: {}
        onReleased: {
            if (enteredHome){
                frameWindow.savedHeight = frameWindow.height
                frameWindow.savedWidth  = frameWindow.width
                frameWindow.held = false
            }else if (frameWindow.preparing){
                frameWindow.preparing = false
                frameWindow.state = "window"
            }
        }

        Rectangle{
            anchors.fill: parent
            anchors.topMargin: -2
            color: "green"
        }
        MouseArea{
            anchors.right: parent.right
            anchors.top:  parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: -2
            width: frameWindow.state !== "default" ? height : 0
            Behavior on width { NumberAnimation { duration: 150 } }
            Rectangle { anchors.fill: parent; color: "red" }
            onPressed: {
                frameWindow.savedHeight = frameWindow.height
                frameWindow.savedWidth  = frameWindow.width
                frameWindow.held = false
            }
        }
        MouseArea{
            anchors.right: parent.right
            anchors.rightMargin: height
            anchors.top:  parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: -2
            width: frameWindow.state !== "default" ? height : 0
            Behavior on width { NumberAnimation { duration: 150 } }
            Rectangle { anchors.fill: parent; color: "blue" }
            onPressed: {
                frameWindow.savedHeight = frameWindow.height
                frameWindow.savedWidth  = frameWindow.width
                frameWindow.held = false
            }
        }
    }
}


