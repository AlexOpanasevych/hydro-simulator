import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../controls" as C
Item {
    id: root
    implicitWidth: buttonView.width + stackView.width

    property list<Component> tabButtons
    property list<Component> tabWindows
    property int orientation : Qt.LeftToRight
    property Component initialButton: C.IconButton { }
    readonly property int tabButtonsCount: tabButtons.length
    readonly property int tabsCount: tabButtons.length
    property color backgroundColor: "transparent"
    property real minimumWindowWidth: 130
    property real maximumWindowWidth: 600
    readonly property real windowWidth: properties.tabWindow.width
    readonly property real windowHeight: stackLayout.height

    readonly property QtObject properties: QtObject{
        property QtObject tabButton: QtObject{
            property real width: 60
            property real height: 60
        }
        property QtObject tabWindow: QtObject{
            property real width: 300
            property real height: parent.height
        }
    }
    readonly property int currentIndex: hiden.currentIndex
    readonly property bool opened: state !== "hide"
    signal menuOpened(int index)
    signal menuClosed()


    state: "hide"

    states: [
        State {
            name: "hide"
            PropertyChanges {
                target: root
            }
        },
        State {
            name: "show"
            PropertyChanges {
                target: root
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "show"

            NumberAnimation {
                target: hiden
                property: "tabButtonOffset"
                to: 0
                duration: 200
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: hiden
                property: "tabWindowOffset"
                to: root.properties.tabButton.width
                duration: 200
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: stackLayout
                property: "opacity"
                from: 0
                to: 1
                duration: 200
                easing.type: Easing.InOutQuad
            }
        },
        Transition {
            from: "*"
            to: "hide"

            NumberAnimation {
                target: hiden
                property: "tabButtonOffset"
                to: root.properties.tabButton.height;
                duration: 200
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: hiden
                property: "tabWindowOffset"
                to: -root.properties.tabWindow.width
                duration: 200
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: stackLayout
                property: "opacity"
                from: 1
                to: 0
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
    ]

    Item {
        id: hiden;
        property int tabButtonOffset: properties.tabButton.height
        property int tabWindowOffset: 0
        property int currentIndex: 0
        Component.onCompleted: tabWindowOffset = -properties.tabWindow.width
    }

    Item {
        id: stackView
        anchors.left: root.orientation === Qt.LeftToRight ? parent.left : undefined
        anchors.leftMargin: root.orientation === Qt.LeftToRight ? hiden.tabWindowOffset : 0
        anchors.right:  root.orientation === Qt.LeftToRight ? undefined : parent.right
        anchors.rightMargin: root.orientation === Qt.LeftToRight ? 0 : hiden.tabWindowOffset
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: properties.tabWindow.width

        StackLayout{
            id: stackLayout
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: properties.tabWindow.width
            currentIndex: hiden.currentIndex

            Repeater{
                model: tabWindows
                delegate: Item{
                    Layout.fillHeight: true
                    Layout.preferredWidth: properties.tabWindow.width
                    Loader{
                        id: loaderWindows
                        anchors.fill: parent
                        property bool showed: stackLayout.currentIndex === index
                        sourceComponent: modelData
                    }
                }
            }
        }

        MouseArea{
            id: dragArea
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: root.orientation === Qt.LeftToRight ? parent.right : undefined
            anchors.left: root.orientation === Qt.LeftToRight ?  undefined : parent.left
            width: 2
            cursorShape: Qt.SizeHorCursor
            property real startX: 0
            property bool draging: false
            onPressed:
            {
                draging = true
                startX = mouseX
            }

            onMouseXChanged: {
                if (draging)
                {
                    let tmp = 0
                    if (root.orientation === Qt.LeftToRight)
                    {
                        tmp = properties.tabWindow.width + (mouseX - startX)
                    }
                    else
                    {
                        tmp = properties.tabWindow.width - (mouseX - startX)
                    }

                    if (tmp > minimumWindowWidth && tmp < maximumWindowWidth)
                    {
                        properties.tabWindow.width = tmp
                    }
                }
            }

            onReleased: {
                draging = false
            }
        }

        Rectangle{
            color: "#16222d"
            width: properties.tabButton.width
            height: stackView.height
            anchors.right: root.orientation === Qt.LeftToRight ? stackLayout.left : undefined
            anchors.left: root.orientation === Qt.LeftToRight ? undefined : stackLayout.right
            anchors.top: stackLayout.top
        }
    }

    Item{
        id: buttonView
        width: properties.tabButton.width
        height: stackView.height
        anchors.left: root.orientation === Qt.LeftToRight ? parent.left : undefined
        anchors.right: root.orientation === Qt.LeftToRight ? undefined : parent.right

        Repeater{
            model: tabButtons
            anchors.top: initialLoader.bottom
            delegate: Item{
                Binding{
                    target: loaderButtons.item
                    property: "y"
                    value: (properties.tabButton.height - hiden.tabButtonOffset) * (index + 1)
                }

                Binding{
                    target: loaderButtons.item
                    property: "visible"
                    value: (hiden.tabButtonOffset !== properties.tabButton.height)
                }

                Binding{
                    target: loaderButtons.item
                    property: "currentChecked"
                    value: index === stackLayout.currentIndex
                }

                Connections{
                    target: loaderButtons.item

                    function onReleased(mouse){
                        hiden.currentIndex = index
                    }
                }

                Loader{
                    id: loaderButtons
                    sourceComponent: modelData
                    onLoaded: {
                        item.width = properties.tabButton.width;
                        item.height = properties.tabButton.height;
                        item.z = tabButtons.length - index
                        modelData.objectName = item.objectName
                        item.buttonBackground = "#16222d"
                    }
                }
            }
        }
        Loader{
            id: initialLoader
            sourceComponent: initialButton
            onLoaded: {
                item.width = properties.tabButton.width;
                item.height = properties.tabButton.height;
                //item.text = "test";
                item.y = 0
                item.z = tabButtons.length + 1
            }
        }
    }

    function open(index)
    {
        if (typeof(index) === "number"){
            if ( index < tabButtons.length){
                hiden.currentIndex = index
                root.menuOpened(hiden.currentIndex)
            }
        }else if (typeof(index) === "string"){
            for (let i = 0; i < tabButtons.length; i++){
                if (tabButtons[i].objectName === index){
                    hiden.currentIndex = i
                    root.menuOpened(hiden.currentIndex)
                    break
                }
            }
        }
        root.menuOpened(hiden.currentIndex)
        root.state = "show"
    }

    function close(index)
    {
        if (typeof(index) === "number"){
            if ( index < tabButtons.length)
                hiden.currentIndex = index
        }else if (typeof(index) === "string"){
            for (let i = 0; i < tabButtons.length; i++){
                if (tabButtons[i].objectName === index){
                    hiden.currentIndex = i
                    break
                }
            }
        }
        root.menuClosed()
        root.state = "hide"
    }
}
