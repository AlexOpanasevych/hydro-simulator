import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQml.Models 2.15
import QtQuick.Layouts 1.15

import "../controls"
import "../base"

/*          BUG:                                                    FIXED:
 *  1. Tabs can't be different width                                false
 *  2. Tabs can't be resizing dynamically                           false
 *  3. When add new tab, visual pos doesn't changed to normal       false
 *  4. When tabs shaked, we can see bug, when del some tabs         true
 *  5. When delete non current tab, current tab = prevCurrent       true
 *  6. Many bugs with description                                   false
 *  7. If removing middle item, when currentIndex is last           true
 *  -> currentIndex doesn't change
 *  8. Can't be dynamically change tab's width                      false
 */

/*          TODO:
 *  1. Same object, which use only c++ models instead list<Tab>
 *  2. Optimize some js function calls
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
    property bool withDescription: false
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
        font.family: IconHelper.icons.name
    }

    property Component defaultTab: Tab{
        width: 30
        height: 30
        background: BaseBackground{
            backgroundColor: "#16222d"
            tintColor: "#17212b"
        }
    }

    onItemsListChanged: { privated.__updatePos() }

    onTabWidthChanged: {
        privated.updating = true;
        privated.__updateTabWidth()
        //privated.__updatePos()
        privated.updating = false;
    }

    Loader{
        id: backgroundLoader
        anchors.fill: parent
        sourceComponent: background
    }

    /*
     * mouse area needs only for checking contains event
     */
    MouseArea{
        preventStealing: false
        anchors.fill: parent
        hoverEnabled: true

        onContainsMouseChanged: {
            if (containsMouse) showDescriptionTimer.restart()
            else { showDescriptionTimer.stop(); descriptionPopup.close() }
        }

        RowLayout{
            id: rowLayout
            anchors.fill: parent
            spacing: 0
            Item{
                Layout.fillHeight: true
                height: rowLayout.height
                Layout.minimumWidth: 0
                Layout.preferredWidth: 10000
                Layout.maximumWidth: flickable.contentX < 10 ? 0 : 30
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
                    font.family: IconHelper.icons.name
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
                            anchors.topMargin: holding ? 5 : 0
                            anchors.bottomMargin:  holding ? 5 : 0
                            width: modelData.width
                            text: modelData.text
                            x: modelData.x
                            z: privated.currentItem === modelData ? itemsList.length : 0
                            removeable: modelData.removeable
                            background: modelData.background
                            content: modelData.content
                            gradientEnabled: modelData.gradientEnabled
                            xButton: modelData.xButton
                            onPressed: { privated.currentIndex = index; }
                            onReleased: { if (!button.dragging) x = modelData.x; privated.movingTab = false; }
                            onPressedAndHold: { privated.movingTab = true;}
                            onClose: { privated.__tryClose(index) }
                            onXChanged: { if (!button.moving) { flickable.swap(modelData, x) } }
                            onHoldingChanged: if (holding) { pageUpdateTimer.restart(); flickable.swap(modelData, x) }
                            onDraggingChanged: { modelData.dragging = dragging; if (dragging) descriptionPopup.close() }

                            Timer{
                                id: pageUpdateTimer
                                running: false
                                interval: 400
                                repeat: button.holding
                                onTriggered: {
                                    //right moving when page ends
                                    if (flickable.contentX + flickable.width < button.x + modelData.width && privated.currentItem !== privated.lastItem)
                                    {
                                        flickable.contentX += modelData.width
                                    }
                                    //left moving when page ends
                                    else if (flickable.contentX > button.x && privated.currentItem !== privated.firstItem)
                                    {
                                        flickable.contentX -= modelData.width
                                    }
                                }
                            }

                            Connections{
                                target: modelData
                                function onXChanged(){
                                    if (!button.dragging) button.x = modelData.x
                                }
                            }

                            Connections{
                                target: button
                                function onShowDescription() {
                                    privated.showDescription = true
                                    privated.descriptionIndex = index
                                    showDescriptionTimer.restart()
                                }
                            }

                            //highlight item
                            Loader{
                                id: highlightLoader
                                anchors.fill: parent
                                active: privated.currentIndex === index && !button.isHold && !button.dragging
                                sourceComponent: highlight
                            }
                        }
                    }

                    Timer{
                        id: showDescriptionTimer
                        interval: 1000
                        onTriggered: if (currentItem && !currentItem.dragging ) descriptionPopup.open()
                    }

                    Rectangle{
                        id: descriptionPopup
                        width: childrenRect.width
                        height: childrenRect.height
                        visible: false
                        y: root.height
                        color: "transparent"
                        Behavior on x { NumberAnimation { duration: 200 }  }
                        Behavior on width { NumberAnimation { duration: 200 } }
                        Behavior on height { NumberAnimation { duration: 200 } }
                        //onXChanged: console.log(x, flickable.width, width)

                        Connections{
                            target: privated
                            function onDescriptionIndexChanged() {
                                if (root.withDescription)
                                {
                                    let item = itemsList[privated.descriptionIndex]
                                    //right
                                    if (item.x >= flickable.contentX + flickable.width - item.width)
                                    {
                                        descriptionPopup.x = flickable.contentX + flickable.width - item.width
                                    }
                                    //left
                                    else if (item.x < flickable.contentX)
                                    {
                                        descriptionPopup.x = flickable.contentX - 40
                                    }
                                    else
                                    {
                                        descriptionPopup.x = item.x
                                    }
                                }
                            }
                        }

                        Loader{
                            id: descriptionLoader
                            active: privated.showDescription && root.withDescription && itemsList[privated.descriptionIndex]
                            sourceComponent: !itemsList[privated.descriptionIndex] ? defaultTab : itemsList[privated.descriptionIndex].description
                            onLoaded: if (itemsList[privated.descriptionIndex].descriptionText !== "") {
                                          descriptionLoader.item.text = itemsList[privated.descriptionIndex].descriptionText
                                          descriptionLoader.item.width = itemsList[privated.descriptionIndex].width
                                      }
                        }

                        function close()
                        {
                            descriptionPopup.visible = false
                        }

                        function open()
                        {
                            descriptionPopup.visible = true
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
                            privated.__updateVisualPos()
                            break
                        }
                        //current item moving left
                        else if (it.x + it.width >= x + item.width / 2 && it.x + it.width <= x + item.width)
                        {
                            let tmp = item.x
                            item.move(it.x)
                            it.move(tmp)

                            privated.__findFirst()
                            privated.__findLast()
                            privated.__updateVisualPos()
                            break
                        }
                    }
                }

                function moveRight(){
                    let nx = flickable.contentX + flickable.width
                    //finding pos
                    for (let i = 0; i < itemsList.length; i++)
                    {
                        var it = itemsList[i]
                        if (it.x <= nx && it.x + it.width >= nx)
                        {
                            nx = it.x
                            break
                        }
                        else if (it.x > nx)
                        {
                            nx = it.x
                            break
                        }
                    }

                    if (flickable.contentX + flickable.width * 2 > flickable.contentWidth ) nx = flickable.contentWidth - flickable.width

                    flickable.contentX = nx
                    privated.__updateVisualPos()
                }

                function moveLeft(){
                    let nx = flickable.contentX - flickable.width

                    for (let i = itemsList.length - 1; i >= 0 ; i--)
                    {
                        var it = itemsList[i]
                        if (it.x <= flickable.contentX && it.x + it.width >= flickable.contentX)
                        {
                            nx =  (it.x + it.width + moveRightButton.width) - flickable.width
                            break
                        }
                        else if (it.x <= nx) break
                    }

                    if (flickable.contentX - flickable.width < 0) nx = 0
                    flickable.contentX = nx
                    privated.__updateVisualPos()
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
                font.family: IconHelper.icons.name

                onPressed: flickable.moveRight()
            }
        }
    }

    function closeTab(index)
    {
        let prevX = flickable.contentX
        let visualIndex = privated.visualPos.indexOf(index)
        privated.updating = true

        privated.descriptionIndex = 0
        showDescriptionTimer.stop()
        descriptionPopup.close();

        //offseting
        var it = itemsList[index]
        for (let i = 0; i < itemsList.length; i++)
        {
            if (itemsList[i].x > it.x)
                itemsList[i].x -= it.width
        }
        //removing
        var arr = Array.from(itemsList).filter( item => { return item !== it })
        itemsList = arr
        //clear created items
        it.kill()
        privated.__updateVisualPos()
        //changing current index to new index
        if (privated.currentIndex === index && privated.visualPos.length > 0)
        {
            if (visualIndex === itemsList.length) privated.currentIndex = privated.visualPos[visualIndex - 1]
            else if (visualIndex - 1 >= -1)  privated.currentIndex = privated.visualPos[visualIndex]
            else if (itemsList.length === 1) privated.currentIndex = -1
        }
        else if (privated.visualPos.length > 0)
            if (privated.currentIndex === itemsList.length)
                privated.currentIndex = privated.visualPos[privated.visualPos.length - 1]

        //if empty -> creates new tab
        privated.updating = false
        if (itemsList.length === 0) openTab()
        if (currentIndex > -1 && itemsList.length > 0 )
        privated.currentItem = itemsList[currentIndex]

        //! [bug fix]
        flickable.anim = false
        flickable.contentX = prevX
        flickable.anim = true

        if (prevX + flickable.width > flickable.contentWidth )
        {
            if (flickable.contentWidth - flickable.width >= 0) flickable.contentX = flickable.contentWidth - flickable.width
            else flickable.contentX = 0
        }
        //! [bug fix]
    }

    /*
     * create new tab
     * return created tab
     */
    function openTab()
    {
        let prevX = flickable.contentX
        privated.updating = true
        var t = defaultTab.createObject(root, {visible: false, x : root.tabWidth * (itemsList.length)/*view.childrenRect.width*/})
        itemsList.push(t)

        root.created(t)

        flickable.anim = false
        flickable.contentX = prevX
        flickable.anim = true

        privated.updating = false

        privated.__updateVisualPos()
        privated.currentIndex = itemsList.length - 1
        return t
    }

    function tab(index)
    {
        if (index < 0 || index >= itemsList.length)
        {
            return null
        }

        return itemsList[index]
    }

    //Component for adding
    //Component{
    //    id: tabComponent
    //    Tab{
    //        width: 30
    //        height: 30
    //        background: BaseBackground{
    //            backgroundColor: "#16222d"
    //            tintColor: "#17212b"
    //        }
    //    }
    //}

    QtObject{
        id: privated
        property bool updating: false
        property bool movingTab: false
        property int currentIndex: -1
        property int currentVisualIndex: 0
        property bool showDescription: false
        property int descriptionIndex: 0
        property var pos: new Map()
        property var visualPos: []
        property Item currentItem
        property Item lastItem
        property Item firstItem

        Component.onCompleted: privated.__updateVisualPos()

        onCurrentIndexChanged: {
            if (currentIndex < 0) currentIndex = 0
            else if (currentIndex >= itemsList.length) currentIndex = itemsList.length - 1
            currentItem = itemsList[currentIndex]
        }

        function __updateTabWidth(){
            if (itemsList.length > 0)
            {
                let offset = itemsList[0].width - tabWidth
                for (let i = 0; i < itemsList.length; i++)
                {
                    let tmp = -offset * (i + 1)
                    itemsList[i].width += tmp
                    itemsList[i].x += tmp
                }
            }
        }

        function __updatePos(){
            if (!privated.currentItem) { privated.currentIndex = 0}
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

        function __updateVisualPos(){
            //privated.visualPos = []
            var tmp = []
            var list = Array.from(itemsList)

            list.sort((item1, item2) => {
                          if (item1.x > item2.x) return 1;
                          else if (item1.x < item2.x) return -1;
                          else return 0;
                      })

            for (let j = 0; j < list.length; j++)
            {
                tmp[j] = Array.from(itemsList).indexOf(list[j])
            }
            privated.visualPos = tmp
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
