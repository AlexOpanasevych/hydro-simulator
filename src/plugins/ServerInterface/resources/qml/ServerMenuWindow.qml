import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import CQML 1.0 as C
import KLib 1.0

Rectangle{
    id: rootWindow
    color: "#1f2936"
    property string type: ""
    property bool moving: false
    property bool movingEnabled: true
    property int expandIndex: -1

    Connections{
        target: sceneArea.scene.scenePlayer
        function onModeChanged(){
            menu.close(0)
        }
    }

    ColumnLayout {
        id: rootlayout
        anchors.fill: parent
        spacing: 10
        Label {
            text: "course angle"
            color: "white"
        }

        Slider {
            id: course

            Text {
                anchors.bottomMargin: 3
                anchors.bottom: course.handle.top
                text: course.value.toFixed(2)
            }

            Layout.alignment: Qt.AlignHCenter

            from: 0
            to:180
        }

        Label {
            text: "board"
            color: "white"
        }

        ComboBox {
            id: board
            Layout.alignment: Qt.AlignHCenter
            property int value
            model: ["lb", "pb"]
            onAccepted: {
                value = currentIndex
            }
        }


        Label {
            text: "peleng"
            color: "white"
        }
        Slider {
            id: peleng

            Layout.alignment: Qt.AlignHCenter
            from: 0
            to: 360

            Text {
                anchors.bottomMargin: 3
                anchors.bottom: peleng.handle.top
                text: peleng.value.toFixed(2)
            }
        }

//        TextField {
//            id: peleng
//            property double value
//            validator: DoubleValidator {
//                bottom: 0
//                top:180
//            }
//            onTextChanged: {if(acceptableInput) value = text}
//        }

        Label {
            text: "distance"
            color: "white"
        }
        Slider {
            id: distance
            Layout.alignment: Qt.AlignHCenter
            from: 0
            to: 32000

            Text {
                anchors.bottomMargin: 3
                anchors.bottom: distance.handle.top
                text: distance.value.toFixed(2)
            }

        }
//        TextField {
//            property double value
//            validator: DoubleValidator {
//                bottom: 0
//                top:180
//            }
//            onTextChanged: {if(acceptableInput) value = text}
//        }

        Label {
            text: "speed"
            color: "white"
        }

        Slider {
            id: speed
            Layout.alignment: Qt.AlignHCenter
            from: 0
            to: 100

            Text {
                anchors.bottomMargin: 3
                anchors.bottom: speed.handle.top
                text: speed.value.toFixed(2)
            }
        }
//        TextField {
//            property double value
//            validator: DoubleValidator {
//                bottom: 0
//                top:180
//            }
//            onTextChanged: {if(acceptableInput) value = text}
//        }

        C.Button {
            height: 50
            Layout.fillWidth: true
            Layout.margins: 10
            textColor: "#1f2936"
            buttonBackground: "#6ab2f2"
            text: "Add simulation data"

            onPressed: {
                App.sConfigurator.sendSimulationData({
                                                         "course":course.value,
                                                         "peleng" : peleng.value,
                                                         "board": board.value,
                                                         "distance": distance.value,
                                                         "speed":speed.value
                                                     })
            }
        }

        C.Button {
            height:50
            Layout.fillWidth: true
            Layout.margins: 10
            textColor: "#1f2936"
            buttonBackground: "#6ab2f2"
            text: "Update logic"
            onPressed: {
                App.server.bronzeStation.updatePythonLogic()
            }
        }

        Item {
            Layout.fillHeight: true
        }

    }

}
