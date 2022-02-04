import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.15
import "../base"
import "../controls" as C

Item{
    id: root
    focus: true
    required property real windowWidth
    required property real windowHeight
    required property Item source

    property color backgroundColor: "#223143"
    property color lineColor: "#538BBD"
    property bool showline: true
    property real bottomOffset: 700
    property string text: ""
    property real backgroundOpacity: 0.7
    property color textColor: "#e94040"

    signal confirmed()
    signal rejected()

    property var confirmFunction: () => { console.log("confirm"); }
    property var rejectFunction: () => { console.log("reject"); }

    Connections{
        target: root
        function onConfirmed(){
            confirmFunction()
        }
        function onRejected(){
            rejectFunction()
        }
    }

    x: -dialog.width / 2 + windowWidth / 2
    y: windowHeight - hiden.offset

    readonly property Item backgroundItem: backgroundLoader.item
    property Component background: BaseBackground{
        id: baseBackground
        border.width: 2
        border.color: "#6ab2f2"
        elevation: 3
        anchors.fill: parent
        clipContent: true
        radius: 0
        backgroundColor: "transparent"
        default property alias r: area.data

        ShaderEffectSource {
            id: effectSource

            sourceItem: root.source
            anchors.fill: parent
            anchors.margins: 2

            sourceRect: Qt.rect(root.x + 2, root.y + 2, width, height)

            Component.onCompleted: { if (!sourceItem)  console.log("Warning area: Missed source element")}
        }

        FastBlur{
            id: blur
            anchors.fill: effectSource

            source: effectSource
            radius: 64
            transparentBorder: true
        }

        OpacityMask{
            anchors.fill: parent
            anchors.topMargin: baseBackground.height * 0.9
            maskSource: baseBackground
            source: progBar
            visible: showline
        }

        WavedMouseArea{
            id: area
            anchors.fill: parent
            z: 0
        }

        BaseBackground{
            id: progBar
            elevation: 3
            anchors.fill: parent
            visible: showline
            backgroundColor: Qt.darker(lineColor)

            anchors.topMargin: baseBackground.height * 0.9

            BaseBackground{
                anchors.fill: parent
                elevation: 0
                backgroundColor: lineColor
                anchors.rightMargin: hiden.progress
            }
        }

    }
    readonly property Item contentItem: contentLoader.item
    property Component content: Item{
        anchors.fill: parent
        ColumnLayout{
            z: 1
            anchors.fill: parent
            anchors.topMargin: 2
            anchors.bottomMargin: parent.height * 0.085
            spacing: 0

            Rectangle{
                id: messageRect
                Layout.preferredHeight: childrenRect.height
                Layout.fillWidth: true
                color: "transparent"
                BaseBackground{
                    anchors.fill: textItem
                    backgroundColor: root.backgroundColor
                    elevation: 2
                    anchors.margins: -5
                    opacity: backgroundOpacity
                }

                Text {
                    id: textItem
                    text: root.text
                    font.pointSize: 14
                    color: textColor
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    onPaintedWidthChanged: {
                        if (paintedWidth > parent.width){
                            width = parent.width
                            wrapMode = Text.WordWrap
                        } else {
                            wrapMode = Text.NoWrap
                        }
                    }
                }
            }

            Item {
                Layout.fillHeight: true
            }

            RowLayout{
                Layout.fillWidth: true
                Layout.maximumHeight: 30
                Layout.margins: 2
                C.Button{
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    text: "Ні"
                    textColor: "#6ab3f3"
                    buttonBackground: root.backgroundColor
                    onPressed: { root.rejected(); root.hide()}
                }
                C.Button{
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    text: "Так"
                    textColor: "#6ab3f3"
                    buttonBackground: root.backgroundColor
                    onPressed: { root.confirmed(); root.hide() }
                    focus: true
                }
            }
        }
    }

    QtObject{
        id: hiden
        property real offset: 0
        property real progress: 0
        property bool closing: false

        onOffsetChanged: {
            //console.log(offset)
        }

        Behavior on offset { NumberAnimation { duration: 200 } }

        Behavior on progress { id: progressBehaviour; NumberAnimation { id: progressAnimation; }  }

    }

    Timer{
        id: timer
        onTriggered: {
            dialog.close()
            hiden.offset = 0
            root.rejectFunction()
        }
    }

    Popup {
        id: dialog
        width: root.width
        height: root.height
        z: root.z
        closePolicy: "NoAutoClose"

        exit: Transition {
            NumberAnimation { property: "opacity"; to: 0.0; duration: 200 }
            //NumberAnimation { target: hiden; property: "offset"; from: root.bottomOffset + dialog.height;  to: 0; duration: 200 }
        }

        enter: Transition {
            NumberAnimation { property: "opacity"; to: 1.0; duration: 200 }
            //NumberAnimation { target: hiden; property: "offset"; from: 0; to: root.bottomOffset + dialog.height; duration: 200  }
            onRunningChanged: if (!running) { timer.restart() }
        }

        background: Loader{
            id: backgroundLoader
            sourceComponent: background

            Loader{
                id: contentLoader
                z: 2
                anchors.fill: parent
                sourceComponent: root.content
                onLoaded: console.log("content", contentItem.width, contentItem.height)
            }
        }
    }

    function show(yesFunc, noFunc, text, duration)
    {
        if (duration < 400)
            duration = 400
        root.confirmFunction = yesFunc
        root.rejectFunction = noFunc
        root.text = text
        //! [set to initials]
        progressBehaviour.enabled = false
        timer.stop()
        timer.interval = duration + 500
        hiden.progress = 0
        hiden.offset = 0
        progressAnimation.duration = timer.interval
        progressBehaviour.enabled = true
        //! [set to initials]

        hiden.offset = root.bottomOffset + root.height
        hiden.progress = root.width;
        //! [set to initials]
        if (dialog.opened){
            hiden.progress = root.width
            timer.restart()
        }else{dialog.open()}
    }

    function hide()
    {
        timer.stop()
        dialog.close()
    }
}

