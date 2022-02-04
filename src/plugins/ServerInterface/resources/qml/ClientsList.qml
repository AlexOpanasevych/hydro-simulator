import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

import KLib 1.0
Rectangle {

    id: root

    property color itemColor;
    property alias list: list.model
    property string listHeader;
    property var clearCallback

    ColumnLayout {
        anchors.fill: parent
        Rectangle {
            id: title
            z: 1
            Layout.alignment: Qt.AlignTop
            color: itemColor
            height: txt.paintedHeight
            Layout.fillWidth: true
            Text {
                id: txt
                anchors.fill: parent

                text: listHeader
                font.pointSize: 12
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }
        }
        ListView {
            id: list

            Layout.margins: 5

            addDisplaced: Transition {
                NumberAnimation {properties: "x, y"; duration: 100}
            }
            moveDisplaced: Transition {
                NumberAnimation { properties: "x, y"; duration: 100 }
            }
            remove: Transition {
                NumberAnimation { properties: "x, y"; duration: 100 }
                NumberAnimation { properties: "opacity"; duration: 100 }
            }

            removeDisplaced: Transition {
                NumberAnimation { properties: "x, y"; duration: 100 }
            }

            displaced: Transition {
                NumberAnimation {properties: "x, y"; duration: 100}
            }

            Layout.fillWidth: true;
            Layout.preferredHeight: list.childrenRect.height
            Layout.maximumHeight: parent.height - title.height - clearButton.height - 10
            anchors.leftMargin: 3
            anchors.rightMargin: 3
            spacing: 5

            delegate: ListDragItem {
                id: delegate
//                anchors.leftMargin: 5
                itemColor: itemColor
            }


        }

        Button {
            id: clearButton
            Layout.alignment: Qt.AlignTop
            Layout.margins: 10

            Layout.fillWidth: true
            height: 100

            background: Rectangle {
                anchors.fill: parent
                color: "#272725"
                radius: 5
            }

            Text {
                id: text
                anchors.fill: parent
                color: "#c2c2c2"
                font.pointSize: 12
                text: qsTr("Очистити список")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            onPressed: {
                clearCallback()
            }
        }

        Item{
            Layout.fillHeight: true
        }
    }
}



