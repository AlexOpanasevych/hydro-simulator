import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CQML 1.0 as C
import KLib 1.0

Popup {
    width: 60
    modal: true
    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
    }
    exit: Transition {
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
    }

    onOpened:{
        textField.text = ""
        textField.forceActiveFocus()
        sceneArea.scene.adminMode = false
    }

    background: C.BaseBackground{
        elevation: 3
        backgroundColor: "#1f2936"
        C.TextField{
            id: textField
            color: "#6ab3f3"
            hintColor:"#6ab3f3"
            placeholderTextColor: "#6ab3f3"
            echoMode: TextField.Password
            anchors.fill: parent
            onTextChanged: {
                if (text === "5428"){
                    sceneArea.scene.adminMode = true
                    close()
                }
            }
        }
    }
}
