import QtQuick 2.0
import QtQuick.Controls 2.0

Popup {
    id: disconnectPopup
    modal: true
    height: parent.height / 2
    width: parent.width / 5 * 3

    closePolicy: Popup.NoAutoClose

    anchors.centerIn: parent

    background: Rectangle {

        id: discBackRect

        anchors.fill: parent

        color: "#BE6E46"

        radius: 5
    }

    Text {
        id: errorDesc
        anchors.fill: parent
        anchors.margins: 10
        text: "Втрачено підключення з сервером! Сервер вимкнено або від'єднано Ethernet-кабель. Клієнт відновить з'єднання автоматично, перевірте виконання цих умов"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap

        font.pointSize: 15

    }

//        Button {
//            x: parent.width / 2 - width / 2
//            height: parent.height / 5
//            width: parent.width / 3
//            anchors.margins: 10
//            anchors.bottom: parent.bottom
//            background: Rectangle {
//                anchors.fill: parent
//                color: "#CDE7B0"
//                radius: 5
//            }

//            text: "Під'єднатися до сервера"
//            font.pixelSize: height / 4

//            onPressed: {
//                if(App.client.connect()) {
//                    disconnectPopup.close();
//                    errorMessagePopup.goodOrBad = true
//                    errorMessagePopup.message = "Під'єднано!"
//                    errorMessagePopup.open()
//                }
//                else {
//                    errorMessagePopup.goodOrBad = false
//                    errorMessagePopup.message = "Не вдалося під'єднатися до сервера!"
//                    errorMessagePopup.open()
//                }
//            }

//        }

}
