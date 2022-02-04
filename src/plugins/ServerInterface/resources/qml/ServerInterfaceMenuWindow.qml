import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import KLib 1.0

Rectangle {
    id: consoleRoot
    color: "#1f2936"

    ColumnLayout {
        id: inputLayout
        Label {text: "course"}
        TextField {
            id: course
            property int value
            validator: IntValidator{
                bottom: 0
                top: 360
            }

            onTextChanged: {if(acceptableInput) value = text}
        }
        Label {text: "peleng"}
        TextField {
            id: peleng
            property int value
            validator: IntValidator{
                bottom: 0
                top: 360
            }
            onTextChanged: {if(acceptableInput) value = text}
        }
        Label {text: "distance"}
        ComboBox {
            id: distance
            model: [4000, 8000, 16000, 32000]
        }
        Label {text: "speed"}
        TextField {id: speed
            property int value
            validator: IntValidator{
                bottom: 0
                top: 100
            }
            onTextChanged: {if(acceptableInput) value = text}
        }
        Label {text: "pressure"}
        TextField {
            id: pressure
            property double value
            validator: DoubleValidator{
                bottom: 0
                top: 100
            }
            onTextChanged: {if(acceptableInput) value = text}
        }
        Button {
            height: 50
            text: "Set emulation data"
            onClicked: {
                App.client.sCommunicator.sendEmulationData({"course" : course.text,
                                                            "peleng" : peleng.text,
                                                            "distance" : distance.currentValue,
                                                            "speed" :speed.value,
                                                            "pressure" : pressure.value})
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }
}
