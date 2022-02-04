import QtQuick 2.0

import KLib 1.0

import "qml"

Item {
    id: root



    property int prevWidth : 1920
    property int prevHeight: 1080
    property real scaleWidth : 1.0;
    property real scaleHeight : 1.0;
    property alias qmlRep: qmlRepeater
    property alias backgroundLoader: backgroundLoader

    Loader {
        id: backgroundLoader
        property var modelData
        Binding {
            target: backgroundLoader.item
            property: "modelData"
            value: backgroundLoader.modelData
        }
    }


    Repeater {
        id: qmlRepeater
        model: App.client.itemsModel
        delegate: Loader{
            id: itemLoader

            sourceComponent: {
                switch(modelData.controlType){
                case "Button":
                    return buttonComponent
                case "Dial":
                    return dialComponent
                case "Lamp":
                    return lampComponent
                case "ButtonGroup":
                    return buttonGroupComponent
                case "Light":
                    return lightComponent
                case "MultiSwitch":
                    return multiSwitchComponent
                case "Switch":
                    return switchComponent
                case "Background":
                    backgroundLoader.modelData = modelData
                    backgroundLoader.sourceComponent = backgroundComponent
                    return null;
                case "Text":
                    return textComponent
                case "NumberView":
                    return numberViewComponent
                case "RectMonitor":
                    return rectMonitorComponent
                case "TucMonitor":
                    return tucMonitorComponent
                case "Recorder":
                    return recorderComponent
                case "Helm":
                    return helmComponent
                case "RoundButton":
                    return roundButtonComponent
                case "CircleIndicator":
                    return circleIndicatorComponent
                case "BronzeRectMonitor":
                    return bronzeRectMonitorComponent
                default:
                    break;
                }
            }

            Binding {
                target: itemLoader.item
                property: "modelData"
                value: modelData
            }

            onLoaded: {

                console.log("loaded component, index:", index)

            }
        }

        Component.onCompleted: {
//            scaleWidth = parent.width / prevWidth
//            scaleHeight = parent.height / prevHeight

//            prevWidth = parent.width
//            prevHeight = parent.height

//            for(let i = 0; i < qmlRepeater.count; i++) {
//                console.log(qmlRepeater.itemAt(i).item)
//                qmlRepeater.itemAt(i).item.updateHScale(scaleHeight)
//                qmlRepeater.itemAt(i).item.updateWScale(scaleWidth)
//            }

        }

    }
    Connections {
        target: parent
        function onWidthChanged() {
            scaleWidth = parent.width / prevWidth

            prevWidth = parent.width

            for(let i = 0; i < qmlRepeater.count; i++) {
                console.log(qmlRepeater.itemAt(i).item)
                if (qmlRepeater.itemAt(i).item)
                qmlRepeater.itemAt(i).item.updateWScale(scaleWidth)
            }

            //backgroundLoader.item.updateHScale(scaleWidth)

        }

        function onHeightChanged() {
            scaleHeight = parent.height / prevHeight

            prevHeight = parent.height

            for(let i = 0; i < qmlRepeater.count; i++) {
                console.log(qmlRepeater.itemAt(i).item)
                if (qmlRepeater.itemAt(i).item)
                    qmlRepeater.itemAt(i).item.updateHScale(scaleHeight)
            }

            //backgroundLoader.item.updateHScale(scaleHeight)

        }
    }

    Component {
        id: buttonComponent
        MGKButton {
        }

    }

    Component {
        id: dialComponent
        MGKDial {

        }

    }

    Component {
        id: lampComponent

        MGKLamp {}

    }

    Component {
        id : numberViewComponent

        MGKNumberView {

        }
    }

//    Component {
//        id : progressBarComponent
//        MGKProgressBar {

//        }
//    }

    Component {
        id : lightComponent
        MGKLight {

        }
    }

    Component {
        id: switchComponent
        MGKSwitch {

        }
    }

    Component {
        id: multiSwitchComponent

        MGKMultiSwitch {

        }
    }

    Component {
        id: buttonGroupComponent
        MGKButtonGroup {

        }
    }
    Component {
        id: backgroundComponent

        Background {
            z: -100
        }
    }

    Component {
        id: textComponent
        MGKText {

        }
    }
    Component {
        id: blockComponent
        Block {

        }
    }
    Component {
        id: rectMonitorComponent
        MGKRectMonitor {

        }
    }

    Component {
        id: tucMonitorComponent
        MGKTucMonitor {

        }
    }

    Component {
        id: recorderComponent
        MGKRecorder {

        }
    }
    Component {
        id: circleIndicatorComponent
        CircleIndicator {

        }
    }

    Component {
        id: helmComponent
        MGKHelm {

        }
    }

    Component {
        id: roundButtonComponent
        MGKRoundButton {

        }
    }
    Component {
        id: bronzeRectMonitorComponent
        BronzeRectMonitor {

        }
    }
}

