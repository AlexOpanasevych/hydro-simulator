import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../controls" as C
Item {
    id: root
    implicitWidth: buttonView.width + stackView.width
    //implicitHeight: stackView.height

    property list<Component> tabButtons
    property list<Component> tabWindows
    property Component initialButton: C.IconButton { }
    readonly property int tabButtonsCount: tabButtons.length
    readonly property int tabsCount: tabButtons.length
    property color backgroundColor: "transparent"

    readonly property QtObject properties: QtObject{
        readonly property QtObject tabButton: QtObject{
            property real width: 60
            property real height: 60
        }
        readonly property QtObject tabWindow: QtObject{
            property real width: 300
            property real height: parent.height
        }
    }

    //property int initialZ: tabButtons.size
    property real offset: 0
    readonly property bool opened: state !== "hide"

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
        anchors.left: parent.left
        anchors.leftMargin: hiden.tabWindowOffset
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: properties.tabWindow.width

        StackLayout{
            id: stackLayout
            anchors.right: dragArea.right
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
                        sourceComponent: modelData
                    }
                }
            }
        }

        MouseArea{
            id: dragArea
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
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
                {    console.log(mouseX)
                    properties.tabWindow.width += (mouseX - startX)
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
            anchors.right: stackLayout.left
            anchors.top: stackLayout.top
        }
    }

    Item{
        id: buttonView
        width: properties.tabButton.width
        height: stackView.height

        Repeater{
            model: tabButtons
            anchors.left: parent.left
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

                    function onPressed(mouse){
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

    function open()
    {
        root.state = "show"
    }

    function close()
    {
        root.state = "hide"
    }
}
