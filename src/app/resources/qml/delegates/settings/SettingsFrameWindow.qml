import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.0
import CQML 1.0 as C
import QtQml 2.15

ApplicationWindow {
    id: settingsWindow
    property Item frameParentItem
    property SettingsFrame frameItem
    property alias internalParent : internalParentItem
    property real previousX: 0
    property real previousY: 0
    property bool resizing: false
    color: "transparent"
    //flags: Qt.FramelessWindowHint | Qt.Window
    property real borderWidth: 0
    property real topBarHeight: 0
    minimumHeight: 50
    minimumWidth: 50

    header: Rectangle{

    }

    opacity: frameItem ? frameItem.enteredHome ? 0.5 : 1 : 1

    onFrameItemChanged: {
        width = frameItem.width
        height = frameItem.height
        x = frameItem.x
        y = frameItem.y
    }

    onYChanged: {
        raise()
        //var pos = internalParentItem.mapToItem(container, x, y)
        //if (pos.x > 0 && pos.x < container.width){
        //    if (pos.y > 0 && pos.y < container.height){
        //        if (frameItem)frameItem.enteredHome = true
        //    }
        //}else{
        //    if (frameItem) frameItem.enteredHome = false
        //}
    }
    onWidthChanged: raise()
    onHeightChanged: raise()
    onXChanged: raise()

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
        //@disable-check M16
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

    Item{
        id: internalParentItem
        anchors.fill: parent
    }
}
