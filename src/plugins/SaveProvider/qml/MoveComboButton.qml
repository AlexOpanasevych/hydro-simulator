import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import CQML 1.0 as C

Item {
    id: button
    width: 38
    height: 38
    property bool topEnabled: true
    property bool bottomEnabled: true
    ColumnLayout{
        anchors.fill: parent
        anchors.topMargin: 2
        anchors.bottomMargin: 2
        spacing: -2
        Text{
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: "\uF0D8"
            color: button.enabled ? "#6ab3f3" : Qt.darker("#6ab3f3", 3.0)
            font.family: topEnabled ? C.IconHelper.iconsPressed.name : C.IconHelper.icons.name
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom
            font.pointSize: 19
            Behavior on color { ColorAnimation { duration: 200 } }
        }
        Text{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 4
            text: "\uF0C9"
            color: button.enabled ? "#6ab3f3" : Qt.darker("#6ab3f3", 3.0)
            font.family: C.IconHelper.iconsPressed.name
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 12
            z: 10
            Behavior on color { ColorAnimation { duration: 200 } }
        }
        Text{
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: "\uF0D7"
            color:  button.enabled ? "#6ab3f3" : Qt.darker("#6ab3f3", 3.0)
            font.pointSize: 19
            font.family: bottomEnabled ? C.IconHelper.iconsPressed.name : C.IconHelper.icons.name
            horizontalAlignment: Text.AlignHCenter
            Behavior on color { ColorAnimation { duration: 200 } }
        }
    }
}
