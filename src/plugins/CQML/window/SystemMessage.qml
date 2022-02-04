import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0
import "../base"

Item{
    id: root
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

    x: -dialog.width / 2 + windowWidth / 2
    y: /*mapToItem(null, 0, 0).y +*/ windowHeight - hiden.offset

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

        OpacityMask{
            anchors.fill: parent
            anchors.topMargin: baseBackground.height * 0.9
            maskSource: baseBackground
            source: progBar
            visible: showline
        }

        WavedMouseArea{
            anchors.fill: parent
            //radius: baseBackground.radius
        }

    }
    readonly property Item contentItem: contentLoader.item
    property Component content: Rectangle{
        anchors.fill: parent
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
        }
    }

    Popup {
        id: dialog
        width: root.width
        height: root.height
        closePolicy: "NoAutoClose"
        z: root.z

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
                sourceComponent: content
            }
        }
    }

    function show(duration)
    {
        if (duration < 400)
            duration = 400
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
        if (dialog.opened){
             timer.restart()
        }else{dialog.open()}
    }

    function hide()
    {
        timer.stop()
        dialog.close()
    }
}

