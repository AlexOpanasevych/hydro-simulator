import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15
import "../base"
import "../controls"

Item{
    id: button
    width: 20
    height: 20
    clip: true

    readonly property Item backgroundItem: backgroundLoader.item
    property Component background: Rectangle{
        anchors.fill: parent
        color: "transparent"
    }

    readonly property Item contentItem: contentLoader.item
    property Component content: Item {
        anchors.fill: parent
        Text {
            anchors.fill: parent
            text: button.text
            color: "#6ab3f3"
            verticalAlignment: Text.AlignVCenter
        }
    }

    readonly property Item xButtonItem: buttonXLoader.item
    property Component xButton: Item{
        id: b
        width: 20
        signal close()
        IconButton{
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width
            height: width
            iconCode: IconHelper.icon["times"]
            textColor: "#6ab3f3"
            buttonBackground: button.backgroundItem ?
                                  button.backgroundItem.hasOwnProperty("color") ?
                                      button.backgroundItem.color
                                    : button.backgroundItem.hasOwnProperty("backgroundColor") ?
                                          button.backgroundItem.backgroundColor
                                        : "transparent"
                                : "transparent"
            buttonRadius: width / 2
            onPressed: b.close()
        }
    }

    property string descriptionText: "Some description"
    property Component description: Rectangle{
        id: description
        property string text: ""
        height: 40
        color: "yellow"
    }

    property bool dragging: false
    property bool moving: false
    property bool removeable: true
    property bool holding: false
    property string text: ""
    property bool gradientEnabled: true

    signal close()
    signal pressed()
    signal released()
    signal pressedAndHold()
    signal showDescription()
    signal hideDescription()

    //Behavior on x { NumberAnimation { duration: 100; easing.type: Easing.InQuad } enabled: !button.dragging }

    Behavior on width { NumberAnimation { duration: 100; easing.type: Easing.InQuad } }
    Behavior on height { NumberAnimation { duration: 100; easing.type: Easing.InQuad } }

    Loader{
        id: backgroundLoader
        anchors.fill: parent
        sourceComponent: background

        WavedMouseArea{
            anchors.fill: backgroundLoader.item
            property real xPressed: 0
            hoverEnabled: true

            onPressed: {
                xPressed = mouseX
                fastPressAndHold.restart()
                button.pressed()
            }

            onReleased: {
                dragging = false
                holding = false
                xPressed = 0
                fastPressAndHold.stop()
                button.released()
            }

            onPressAndHold: {
                button.pressedAndHold()
            }

            //onDoubleClicked: close()

            onMouseYChanged:  { if (dragging && movable) { button.x +=  mouseX - xPressed } }

            onContainsMouseChanged: {
                if (containsMouse) { button.showDescription() }
                else { button.hideDescription() }
            }

            Timer{
                id: fastPressAndHold
                interval: 150
                onTriggered: {
                    dragging = true
                    holding = true
                    button.pressedAndHold()
                }
            }
        }
    }

    Loader{
        id: contentLoader
        anchors.fill: parent
        anchors.margins: 2
        sourceComponent: content
    }

    Loader{
        id: gradientLoader
        anchors.fill: parent
        //active: gradientEnabled
        sourceComponent: gradientComponent
    }

    Loader{
        id: buttonXLoader
        active: button.removeable && !holding
        sourceComponent: xButton

        Binding{
            target: buttonXLoader.item
            property: "height"
            value: button.height
        }

        Binding{
            target: buttonXLoader.item
            property: "x"
            value: buttonXLoader.item ? button.width - buttonXLoader.item.width : 0
        }

        Connections{
            target: buttonXLoader.item
            function onClose(){
                button.close()
            }
        }
    }

    Component{
        id: gradientComponent
        LinearGradient{
            anchors.fill: parent
            start: Qt.point(0,0)
            end: Qt.point(width, 0)
            visible: gradientEnabled
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: "transparent";
                }
                GradientStop {
                    position: 1 - ( (xButtonItem ? xButtonItem.width / 2 : button.width * 0.125 ) * 100 / button.width) * 0.01;
                    color: button.backgroundItem ?
                               button.backgroundItem.hasOwnProperty("color") ?
                                   button.backgroundItem.color
                                 : button.backgroundItem.hasOwnProperty("backgroundColor") ?
                                       button.backgroundItem.backgroundColor
                                     : "transparent"
                             : "transparent"
                }
            }
        }
    }

    function kill()
    {
        button.destroy()
    }

    function move(x)
    {
        moving = true
        button.x = x
        moving = false
    }
}
