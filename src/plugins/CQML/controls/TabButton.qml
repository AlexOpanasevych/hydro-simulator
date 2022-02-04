import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQml.Models 2.15
import QtQuick.Layouts 1.15

import "../controls"
import "../base"

/*          BUG:
 *  1. Tabs can't be different width
 *  2. Tabs can't resizing dynamically
 *  3. When adding new tab, visual position doesn't change | FIXED
 */

Item {
    id: root
    objectName: "TabBar"
    default property list<Tab> itemsList

    readonly property Item currentItem: privated.currentItem
    property alias currentIndex: privated.currentIndex
    readonly property int currentVisualIndex: privated.currentVisualIndex
    property bool movable: true
    property bool expandable: true
    property real tabWidth: 80

    signal close(var close)
    signal pressed(int index)
    signal created(var tab)

    readonly property Item backgroundItem: backgroundLoader.item
    property Component background: Rectangle{
        anchors.fill: parent
        color: "#16222d"
    }

    property Component highlight: Item{
        Rectangle{
            anchors.fill: parent
            anchors.topMargin: parent.height * 0.9
            anchors.leftMargin: parent.width * 0.1
            anchors.rightMargin: parent.width * 0.1
            anchors.bottomMargin: -10
            radius: height / 2
            color: !currentItem ? "transparent" : "#6ab3f3"
        }
    }

    property Component addButton: IconButton{
        anchors.fill: parent
        anchors.margins: 3
        buttonRadius: width / 2
        iconCode: IconHelper.icon["plus"]
        textColor: "#6ab3f3"
        buttonBackground: "#16222d"
    }

    onItemsListChanged: { privated.__updatePos() }

    Loader{
        id: backgroundLoader
        anchors.fill: parent
        sourceComponent: background
    }

    RowLayout{
        id: rowLayout
        anchors.fill: parent
        spacing: 0
        Item{
            Layout.fillHeight: true
            Layout.minimumWidth: 0
            Layout.preferredWidth: 100
            Layout.maximumWidth: flickable.contentX <= 10 ? 0 : 30
            //onWidthChanged: console.log("width", width, flickable.contentX)
            z: itemsList.length * 2
            clip: true
            visible: flickable.anim

            Behavior on width { NumberAnimation { duration: 100; easing.type: Easing.InQuad } }

            IconButton{
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                width: parent.width - 4
                height: parent.height
                iconCode: IconHelper.icon["chevron-left"]
                buttonBackground: backgroundItem ? backgroundItem.color : "transparent"
                textColor: "#6ab3f3"
                onPressed: flickable.moveLeft()
                onDoubleClicked: flickable.contentX = 0
            }
        }

        Flickable{
            id: flickable
            Layout.fillHeight: true
            Layout.fillWidth: true
            flickableDirection: Flickable.HorizontalFlick
            contentWidth: view.width + addItem.width
            interactive: false
            property bool anim: true

            Behavior on contentX { NumberAnimation { duration: 100; easing.type: Easing.InQuad } enabled: flickable.anim }

            Item {
                id: view
                height: parent.height
                width: childrenRect.width

                Repeater{
                    id: repeater
                    model: itemsList
                    objectName: "TabBarRepeater"
                    delegate: Tab{
                        id: button
                        clip: true
                        anchors.top: view.top
                        anchors.bottom: view.bottom
                        anchors.topMargin: isHold ? 5 : 0
                        anchors.bottomMargin:  isHold ? 5 : 0
                        width: modelData.width
                        x: modelData.x
                        z: privated.currentItem === modelData ? itemsList.length : 0
                        removeable: modelData.removeable
                        background: modelData.background
                        content: modelData.content
                        xButton: modelData.xButton
                        onPressed: { privated.currentIndex = index;}
                        onReleased: { if (!button.dragging) x = modelData.x; privated.movingTab = false; isHold = false }
                        onPressedAndHold: { privated.movingTab = true; isHold = true }
                        onClose: { privated.__tryClose(index) }
                        onXChanged: {
                            if (!button.moving) { flickable.swap(modelData, x) }
                            //right moving when page ends
                            if (flickable.contentX + flickable.width < button.x + modelData.width && privated.currentItem !== privated.lastItem)
                            {
                                flickable.contentX += modelData.width
                            }
                            else if (flickable.contentX > button.x && privated.currentItem !== privated.firstItem)
                            {
                                flickable.contentX -= modelData.width
                            }
                        }

                        Component.onCompleted: if (index === 0) privated.firstItem = modelData

                        property bool isHold: false

                        Connections{
                            target: modelData
                            function onXChanged(){
                                if (!button.dragging) button.x = modelData.x
                            }
                        }

                        //highlight item
                        Loader{
                            id: highlightLoader
                            anchors.fill: parent
                            active: privated.currentIndex === index && !button.isHold
                            sourceComponent: highlight
                        }
                    }
                }
            }

            //add items button
            Item{
                id: addItem
                x: view.width + 1
                height: view.height
                width: height
                clip: true
                visible: expandable && !privated.movingTab ? height : 0

                Loader{
                    id: addButtonLoader
                    anchors.fill: parent
                    sourceComponent: addButton

                    Connections{
                        target: addButtonLoader.item
                        function onPressed(){
                            root.openTab()
                        }
                    }
                }
            }

            //swap coords between items
            // item -> item, that currently moving
            // x -> visual position of currentItem
            function swap(item, x)
            {
                if (item !== privated.currentItem) return;
                for (let i = 0; i < itemsList.length; i++)
                {
                    var it = itemsList[i]
                    //current item moving right
                    if (it.x >= x && x + item.width / 2 > it.x)
                    {
                        let tmp = item.x
                        item.move(it.x)
                        it.move(tmp)

                        privated.__findFirst()
                        privated.__findLast()
                    }
                    //current item moving left
                    else if (it.x + it.width >= x + item.width / 2 && it.x + it.width <= x + item.width)
                    {
                        let tmp = item.x
                        item.move(it.x)
                        it.move(tmp)

                        privated.__findFirst()
                        privated.__findLast()
                    }
                }
            }

            function moveRight(){
                let nx = flickable.contentX + flickable.width
                if (nx + flickable.width > flickable.contentWidth ) nx = flickable.contentWidth - flickable.width
                flickable.contentX = nx
            }

            function moveLeft(){
                let nx = flickable.contentX - flickable.width
                if (nx < 0) nx = 0
                flickable.contentX = nx
            }
        }
    }

    Item{
        id: moveRightButton
        height: rowLayout.height
        width: flickable.contentX + flickable.width >= flickable.contentWidth ? 0 : 30
        anchors.right: rowLayout.right
        clip: true
        Behavior on width { NumberAnimation { duration: 100; easing.type: Easing.InQuad } }
        IconButton{
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            width: parent.width
            height: parent.height
            iconCode: IconHelper.icon["chevron-right"]
            buttonBackground: backgroundItem ? backgroundItem.color : "transparent"
            textColor: "#6ab3f3"

            onPressed: flickable.moveRight()
        }
    }

    function closeTab(index)
    {
        let prevX = flickable.contentX
        privated.updating = true
        if (index === itemsList.length - 1) privated.currentIndex = index - 1
        else if (index - 1 >= -1) { privated.currentIndex = index }
        else if (itemsList.length === 1) privated.currentIndex = -1
        var it = itemsList[index]
        for (let i = 0; i < itemsList.length; i++)
        {
            if (itemsList[i].x > it.x)
                itemsList[i].x -= it.width
        }
        var arr = Array.from(itemsList).filter( item => { return item !== it })
        itemsList = arr
        privated.updating = false
        if (itemsList.length === 0) openTab()
        privated.currentItem = itemsList[currentIndex]
        //clear created items
        it.kill()

        flickable.anim = false
        flickable.contentX = prevX
        flickable.anim = true
        if (prevX + flickable.width > flickable.contentWidth )
        {
           if (flickable.contentWidth - flickable.width >= 0) flickable.contentX = flickable.contentWidth - flickable.width
           else flickable.contentX = 0
        }
    }

    /*
     * create new tab
     * return created tab
     */
    function openTab()
    {
        let prevX = flickable.contentX
        privated.updating = true
        var t = tabComponent.createObject(root, {visible: false, x : view.childrenRect.width })
        itemsList.push(t)

        flickable.anim = false
        flickable.contentX = prevX
        flickable.anim = true

        privated.currentIndex = itemsList.length - 1
        privated.updating = false
        return t
    }

    //Component for adding
    Component{
        id: tabComponent
        Tab{
            width: 30
            height: 30
            background: Rectangle{
                width: 60
                height: 30
                color: "#16222d"
            }
        }
    }

    QtObject{
        id: privated
        property bool updating: false
        property bool movingTab: false
        property int currentIndex: -1
        property int currentVisualIndex: 0
        property var pos: new Map()
        property Item currentItem
        property Item lastItem
        property Item firstItem

        onCurrentIndexChanged: {
            if (currentIndex < 0) currentIndex = 0
            else if (currentIndex >= itemsList.length) currentIndex = itemsList.length - 1
            currentItem = itemsList[currentIndex]
        }

        function __updatePos(){
            if (!privated.currentItem) { privated.currentIndex = 0; privated.currentItem.z = 100 }
            for (let i = 0; i < itemsList.length; i++)
            {
                itemsList[i].width = root.tabWidth
                if (privated.updating)
                {
                    privated.pos[itemsList[i]] = itemsList[i].x;
                }
                else
                {
                    var x = 0
                    if (i > 0) x = itemsList[i - 1].width + itemsList[i - 1].x
                    privated.pos[itemsList[i]] = x;
                    itemsList[i].x = x;
                }
            }
        }

        function __findLast(){
            if (itemsList.length !== 0)
            {
                var last = itemsList[0]
                for (let i = 1; i < itemsList.length; i++)
                {
                    if (last.x < itemsList[i].x) last = itemsList[i]
                }
                privated.lastItem = last
            }
        }

        function __findFirst(){
            if (itemsList.length !== 0)
            {
                var first = itemsList[0]
                for (let i = 1; i < itemsList.length; i++)
                {
                    if (first.x > itemsList[i].x) first = itemsList[i]
                }
                privated.firstItem = first
            }
        }

        function __tryClose(index){
            var cl = new Map();
            cl["closeReady"] = true
            cl["index"] = index
            root.close(cl);
            if (cl["closeReady"]) root.closeTab(index)
        }
    }
}
