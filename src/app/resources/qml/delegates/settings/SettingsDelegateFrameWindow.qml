import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

import KLib 1.0
import CQML 1.0 as C

Rectangle {
    id: root
    color: "#1f2936"
    property int currentIndex: 0
    ColumnLayout{
        id: layouts
        anchors.fill: parent
        spacing: 0

        Text {
            id: typeText
            Layout.fillWidth: true
            Layout.minimumHeight: 40
            color: "#6ab3f3"
            text: "SETTINGS"
            font.pixelSize: 17
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap

            Shape{
                Layout.fillWidth: true
                Layout.preferredHeight: 5
                ShapePath {
                    fillColor: "transparent"
                    strokeColor: "#7f8c92"
                    strokeWidth: 1
                    startX: 0
                    startY: typeText.height - 1
                    PathLine { x: typeText.width; y: typeText.height - 1 }
                }
            }

            Rectangle{
                color: "white"
                anchors.fill: typeText
                opacity: 0.1
            }

            C.IconButton{
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 3
                anchors.bottomMargin: 5
                width: height
                buttonRadius: width / 2
                opacity: root.currentIndex > 0 ? 1 : 0
                enabled: opacity
                iconCode: C.IconHelper.icon["arrow-left"]
                buttonBackground: "#1f2936"
                textColor: "#6ab3f3"
                elevation: 0
                font.family: currentChecked ? C.IconHelper.iconsPressed.name : C.IconHelper.icons.name
                onReleased: {
                    if (root.currentIndex !== 0)
                    settingsStackView.replace(windowsRepeater.itemAt(0))
                    root.currentIndex = 0
                }
                onDoubleClicked:{

                }
                Behavior on opacity { NumberAnimation { duration: 200 } }
            }

            Repeater{
                id: windowsRepeater
                model: {
                    var arr = sceneArea.scene.getComponents("settingsWindow")
                    arr.unshift(initialWindow)
                    return arr
                }

                delegate: Loader{
                    id: windowLoader
                    sourceComponent: index === 0 ? initialWindow : frame
                    onLoaded: {
                        if (index !== 0){
                            windowLoader.item.cmpnnt = modelData
                        }
                    }
                    Binding{
                        target: windowLoader.item
                        property: "visible"
                        value: (windowLoader.item.state === "scene" ||
                                    (root.currentIndex === index && windowLoader.item.state === "default") ||
                                    (windowLoader.item.state === "out" ) || root.currentIndex === index )
                    }
                }
            }

        }

        StackView{
            id: settingsStackView
            Layout.fillHeight: true
            Layout.fillWidth: true

            replaceEnter: Transition {
                NumberAnimation { property: "opacity"; to: 1.0; duration: 300 }
            }
            replaceExit: Transition {
                NumberAnimation { property: "opacity"; to: 0.0; duration: 300 }
            }
            Component.onCompleted: settingsStackView.push(windowsRepeater.itemAt(0))
        }

        Component{
            id: initialWindow
            Item{
                ListView{
                    id: buttonsListView
                    anchors.fill: parent
                    anchors.topMargin: 10
                    anchors.bottomMargin: 10
                    model: sceneArea.scene.getComponents("settingsButton")
                    delegate: Loader{
                        id: buttonLoader
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 5
                        height: 30
                        sourceComponent: modelData
                        Connections{
                            target: buttonLoader.item
                            function onPressed(){
                                root.currentIndex = index + 1
                                settingsStackView.replace(windowsRepeater.itemAt(index + 1))
                            }
                        }
                    }
                }
            }
        }

        Component{
            id: frame
            Item{
                id: container
                property Component cmpnnt
                property Window win
                layer.enabled: true
                layer.samples: 4

                SettingsFrame{
                    id: frameSettings
                    component: container.cmpnnt ? container.cmpnnt : null
                    onStateChanged: {
                        if (frameSettings.state === "window"){
                                        var comp = Qt.createComponent("SettingsFrameWindow.qml");
                                        container.win = comp.createObject(root);
                                        frameSettings.parent = container.win.internalParent
                                        container.win.frameParentItem = container
                                        container.win.frameItem = frameSettings
                                        container.win.show();
                                    }
                    }
                }

                DropArea{
                    anchors.fill: parent
                    enabled: frameSettings.frameAvailable
                    onEntered: {
                        if (frameSettings.source === drag.source)
                        frameSettings.enteredHome = true
                    }
                    onExited: {
                        if (frameSettings.source === drag.source)
                        frameSettings.enteredHome = false
                    }
                }
                //dash lines
                Shape{
                    id: shape
                    anchors.fill: parent
                    anchors.margins: 10
                    opacity: frameSettings.parent !== parent && frameSettings.enteredHome ? 1 : 0
                    property real r: 30

                    Behavior on opacity { NumberAnimation { duration: 200 } }

                    ShapePath {
                        strokeColor: "#6ab3f3"
                        fillColor: "transparent"
                        startX: 0
                        startY: 30
                        strokeWidth: 3
                        strokeStyle: ShapePath.DashLine
                        dashOffset: 10
                        PathArc{
                            x: shape.r
                            y: 0
                            radiusX: shape.r
                            radiusY: shape.r
                        }
                        PathLine{
                            x: shape.width - shape.r
                            y: 0
                        }
                        PathArc{
                            x: shape.width
                            y: shape.r
                            radiusX: shape.r
                            radiusY: shape.r
                        }
                        PathLine{
                            x: shape.width
                            y: shape.height - shape.r
                        }

                        PathArc{
                            x: shape.width - shape.r
                            y: shape.height
                            radiusX: shape.r
                            radiusY: shape.r
                        }

                        PathLine{
                            x: shape.r
                            y: shape.height
                        }

                        PathArc{
                            x: 0
                            y: shape.height - shape.r
                            radiusX: shape.r
                            radiusY: shape.r
                        }

                        PathLine{
                            x: 0
                            y: shape.r
                        }
                    }
                }

                ColumnLayout{
                    anchors.centerIn: shape
                    width: !shape.opacity ? shape.width : shape.width - 30
                    height: !shape.opacity ? shape.height : shape.height - 30
                    Behavior on width { NumberAnimation { duration: 200 } }
                    Behavior on height { NumberAnimation { duration: 200 } }

                    RowLayout{
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30

                        Text{
                            visible: frameSettings.state !== "default"
                            Layout.preferredWidth: 70
                            Layout.preferredHeight: 30
                            text: "X: " + xWindowSlider.value.toFixed(0)
                            verticalAlignment: Text.AlignVCenter
                            color: "#6ab3f3"
                        }

                        C.Slider{
                            id: xWindowSlider
                            visible: frameSettings.state !== "default"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 30
                            from: 0
                            to: frameSettings.state === "scene" || frameSettings.state === "default" ? window.width - frameSettings.width : 2560 - frameSettings.width
                            value: frameSettings.state === "scene" || frameSettings.state === "default" ? frameSettings.x : container.win ? container.win.x : 0
                            onValueChanged: {
                                if (frameSettings.state === "scene"){
                                    if (value !== frameSettings.x ){
                                        frameSettings.x = value
                                    }
                                }else{
                                    if (container.win && value !== container.win.x ){
                                        container.win.x = value
                                    }
                                }
                            }
                        }
                    }

                    RowLayout{
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text{
                            visible: frameSettings.state !== "default"
                            Layout.preferredWidth: 70
                            Layout.preferredHeight: 30
                            text: "Y: " + yWindowSlider.value.toFixed(0)
                            verticalAlignment: Text.AlignVCenter
                            color: "#6ab3f3"
                        }

                        C.Slider{
                            id: yWindowSlider
                            visible: frameSettings.state !== "default"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 30
                            from: 0
                            to: frameSettings.state === "scene" || frameSettings.state === "default" ? window.height - frameSettings.height : 1440 - frameSettings.height
                            value: frameSettings.state === "scene" || frameSettings.state === "default" ? frameSettings.y : container.win ? container.win.y : 0
                            onValueChanged: {
                                if (frameSettings.state === "scene"){
                                    if (value !== frameSettings.y ){
                                        frameSettings.y = value
                                    }
                                }else{
                                    if (container.win && value !== container.win.y ){
                                        container.win.y = value
                                    }
                                }
                            }
                        }
                    }

                    RowLayout{
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text{
                            visible: frameSettings.state !== "default"
                            Layout.preferredWidth: 70
                            Layout.preferredHeight: 30
                            text: "Width: " + widthWindowSlider.value.toFixed(0)
                            verticalAlignment: Text.AlignVCenter
                            color: "#6ab3f3"
                        }

                        C.Slider{
                            id: widthWindowSlider
                            visible: frameSettings.state !== "default"
                            Layout.fillWidth: true
                            Layout.preferredHeight: 30
                            from: 0
                            to: 2560
                            value: frameSettings.state === "scene" || frameSettings.state === "default" ? frameSettings.width :  container.win ? container.win.width : 0
                            onValueChanged: {
                                if (frameSettings.state === "scene"){
                                    if (value !== frameSettings.width ){
                                        frameSettings.width = value
                                    }
                                }else{
                                    if (container.win && value !== container.win.width ){
                                        container.win.width = value
                                    }
                                }
                            }
                        }
                    }

                    Item{
                        Layout.fillHeight: true
                    }
                }
            }
        }
    }
}
