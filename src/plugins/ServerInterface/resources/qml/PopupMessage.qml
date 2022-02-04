import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

Item {
    id:root

    property string messageText;
    property color backgroundColor;
    property alias outerPopup : popup;
    width: 400
    height: 200

    anchors.horizontalCenter: parent.horizontalCenter
    Popup
    {
        id: popup
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        leftPadding: 0;
        rightPadding: 0;
        topPadding: 0;
        bottomPadding: 0;

        Rectangle
        {
            id: popRect;
            color: backgroundColor

            anchors.centerIn: parent
            width: parent.width;
            height: parent.height;

            Text {
                id: text
                text: root.messageText
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 12
                anchors.fill: parent
                color: "white"
                wrapMode: Text.WordWrap
            }
        }

        DropShadow
        {
            width: popup.width;
            height: popup.height;

            source: popRect;

            horizontalOffset: 0;
            verticalOffset: 5;
            radius: 10;
            samples: 7;
            color: "black";
        }
        onOpened: {
            showTimer.restart()
        }

        Timer {
            id: showTimer
            interval: 2000
            repeat: false
            onRunningChanged: {
                if(!running) {
                    popup.close()
                }
            }
        }



        enter: Transition {

            ParallelAnimation {
                NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
                NumberAnimation {target: popup; property: "y"; from: -height; to: 20; duration: 800}
             }
        }

        exit: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
                NumberAnimation {target: popup; property: "y"; from: 20; to: -height; duration: 800}
             }
        }
    }

//    FastBlur
//    {
//        id: theBlur;
//        visible: false;
//        anchors.fill: rectMain;
//        source: rectMain;
//        radius: 8
//    }


}
