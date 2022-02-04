import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import "../base"

Controls.RadioButton {
    id: control
    spacing: 0
    property bool canToggle
    property color color: "transparent"
    property real mult: 1

    property bool darkBackground: false

    contentItem: Text {
        text: control.text
        color: control.enabled ? Theme.alpha("#fff", 0.30) :  Theme.alpha("#000", 0.26)
    }

    background: Rectangle {
        color: "transparent"
    }

    indicator: Rectangle {
        implicitWidth: 48 * mult
        implicitHeight: 48 * mult
        radius: implicitHeight / 2
        color: control.activeFocus ? control.color : "transparent"

        Rectangle {
            anchors.centerIn: parent

            implicitWidth: 20 * mult
            implicitHeight: 20 * mult
            radius: implicitHeight / 2
            color: "transparent"

            border.color: control.enabled ? "black" : "black"

            border.width: 2 * mult
            antialiasing: true

            Behavior on border.color {
                ColorAnimation { duration: 200}
            }

            Rectangle {
                anchors {
                    centerIn: parent
                    alignWhenCentered: false
                }
                implicitWidth: control.checked ? 10 * mult : 0
                implicitHeight: control.checked ? 10 * mult : 0
                color: control.enabled ? "black" : "black"
                radius: implicitHeight / 2
                antialiasing: true

                Behavior on implicitWidth {
                    NumberAnimation { duration: 200 }
                }

                Behavior on implicitHeight {
                    NumberAnimation { duration: 200 }
                }
            }
        }
    }

    WavedMouseArea {
            id: wavedMouseArea
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 4 * Units.dp
            }

            width: 40 * Units.dp
            height: 40 * Units.dp
            color: control.checked ? "black" : Qt.rgba(0,0,0,0.1)

            onClicked: {
                if(control.canToggle || !control.checked)
                    control.checked = !control.checked
                control.clicked()
            }

            circular: true
            centered: true
        }

        MouseArea {
            anchors {
                left: wavedMouseArea.right
                top: parent.top
                right: parent.right
                bottom: parent.bottom
            }
            onClicked: {
                if(control.canToggle || !control.checked)
                    control.checked = !control.checked
                control.clicked()
            }
        }
}
