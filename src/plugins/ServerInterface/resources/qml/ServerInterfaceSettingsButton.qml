import QtQuick 2.0
import QtQuick.Controls 2.0

Button {

    background: Rectangle {
        color: "#333333"
    }
    Text {
        anchors.fill: parent

        id: buttonText
        text: qsTr("Налаштування аудіоплагіна")
        font.pointSize: parent.height / 3
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: "#e7e7e7"
    }

    //onPressed: {
    //    settingsStackView.push("ServerInterfaceSettingsWindow.qml")
    //}
}
