import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.0
import CQML 1.0 as C
import QtQml 2.15

Window {
    id: settingsWindow
    property Item frameParentItem
    property SettingsFrame frameItem
    property alias internalParent : internalParentItem
    property real previousX: 0
    property real previousY: 0
    property bool resizing: false
    color: "transparent"
//    flags: Qt.FramelessWindowHint | Qt.Window
    property real borderWidth: 5
    property real topBarHeight: 30
    minimumHeight: 50
    minimumWidth: 50
    onFrameItemChanged: {
        width = frameItem.width
        height = frameItem.height
        x = frameItem.x
        y = frameItem.y
    }

    onXChanged: {
        console.log(x)
    }

    Binding{
        target: frameItem
        property: "width"
        when: frameItem ? frameItem.state === "window" : false
        value: internalParentItem.width
        //@disable-check M16
        restoreMode : Binding.RestoreBinding
    }

    Binding{
        target: frameItem
        property: "height"
        when: frameItem ? frameItem.state === "window" : false
        value: internalParentItem.height
//        @disable-check M16
        restoreMode : Binding.RestoreBinding
    }

    Connections{
        target: settingsWindow
        function onClosing(close){
            if (frameParentItem && frameItem){
                frameItem.width = internalParentItem.width
                frameItem.height = internalParentItem.height
                frameItem.parent = frameParentItem
                frameItem.held = false
            }
            close.accepted = true
        }
    }

    Connections{
        target: frameItem
        enabled: frameItem
        function onStateChanged(){
            if (frameItem.state === "default"){
                frameItem.savedWidth = internalParentItem.width
                frameItem.savedHeight = internalParentItem.height
                frameItem.parent = frameParentItem
                frameItem.held = false
                settingsWindow.close()
            }
        }
    }

    Rectangle{
        id: bgRect
        color: "gray"
        anchors.fill: parent

        //MouseArea{
        //    anchors.fill: parent
        //    hoverEnabled: true
        //    onWheel: {}
        //    cursorShape: Qt.LeftArrow
        //    onMouseXChanged: {
        //        if (containsMouse){
        //            if (mouseX < borderWidth && mouseY < borderWidth){
        //                console.log("top left")
        //            }else if (mouseX < borderWidth && mouseY > height - borderWidth){
        //                console.log("bottom left")
        //            }else if (mouseX > width - borderWidth && mouseY < borderWidth){
        //                console.log("top right")
        //            }else if (mouseX > width - borderWidth && mouseY > height - borderWidth){
        //                console.log("bottom right")
        //            }else if (mouseX > borderWidth){
        //
        //            }else{
        //                console.log("exit")
        //            }
        //        }
        //    }
        //    onMouseYChanged: {
        //        if (containsMouse){
        //
        //        }
        //    }
        //}
    }

    DropShadow {
        anchors.fill: parent
        horizontalOffset: 2
        verticalOffset: 2
        radius: 5
        samples: 5
        source: bgRect
        color: "black"
    }

    Item{
        id: internalParentItem
        anchors.fill: parent
        anchors.margins: 5
        anchors.topMargin: 20
    }

    DragHandler {
        id: moveHandler
        onActiveChanged: if (active){
                             const p = moveHandler.centroid.position;
                             let e = 0;
                             if (p.x > borderWidth && p.x < width - borderWidth && p.y > borderWidth && p.y < height - borderWidth){
                                 settingsWindow.startSystemMove();
                             }else{
                                 if (p.x < borderWidth) e |= Qt.LeftEdge;
                                 if (p.x >= width - borderWidth) e |= Qt.RightEdge;
                                 if (p.y < borderWidth) e |= Qt.TopEdge;
                                 if (p.y >= height - borderWidth) e |= Qt.BottomEdge;
                                 settingsWindow.startSystemResize(e);
                             }
                         }
        grabPermissions: TapHandler.TakeOverForbidden
        target: null
    }



    Rectangle{
        color: "red"
        anchors{
            top: bgRect.top
            right: bgRect.right
        }
        height: 20
        width: 30

        MouseArea{
            anchors.fill: parent
            onReleased: {
                settingsWindow.close()
            }
        }
    }


}
