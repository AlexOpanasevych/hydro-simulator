import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtQml.Models 2.15
import QtQml 2.15
import KLib 1.0
import CQML 1.0 as C

Rectangle {
    id: rootWindow
    color: "#1f2936"
    Flickable {
        id: flickable
        anchors.fill: parent
        anchors.margins: 5
        clip: true

        flickableDirection: Flickable.VerticalFlick

        ScrollBar.vertical:  ScrollBar { policy: ScrollBar.AlwaysOn }
        Behavior on Layout.preferredHeight { NumberAnimation { duration: 200; easing.type: Easing.OutQuad } }

        TextArea.flickable: TextArea{
            id: textArea
            wrapMode: TextArea.WordWrap
            selectByMouse: true
            readOnly: true
            text: DevLogger.logText
            background: Rectangle {
                implicitWidth: 200
                implicitHeight: 20
                color: "#2a5378"
                border.color: "#1f2936"
                radius: 3
            }
            color: "#1f2936"
            textFormat: Text.RichText
            onTextChanged: textArea.cursorPosition = textArea.length - 1
            onHeightChanged: textArea.cursorPosition = textArea.length - 1
        }
    }
}
