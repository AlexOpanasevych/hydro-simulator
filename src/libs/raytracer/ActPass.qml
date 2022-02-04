import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    id: row
    x: 0
    y: 0
    signal closePopup
    width: 800
    height: 400
    property variant item: []
    property double minPressure: minSize.value
    property double maxPressure: maxSize.value

    property double minFCoefficient: minSizeF.value
    property double maxFCoefficient: maxSizeF.value

    property double minNoiseCoefficient: minSizeN.value
    property double maxNoiseCoefficient: maxSizeN.value

    property double amplitude: amplitude.value
    property double frequency: frequency.value
    property double amplificationFactor: amplificationFactor.value

    Rectangle {
        id: active
        width: row.width / 2
        anchors.left: row.left
        anchors.top: row.top
        anchors.bottom: row.bottom

        Rectangle {
            x: 54
            y: 0
            width: 170
            height: 25
            anchors.topMargin: 12
            border.color: "#333333"
            border.width: 2
            color: "#DFD7C8"
            radius: 4
            anchors.horizontalCenter: parent.horizontalCenter

            //anchors.horizontalCenterOffset: -173
            Text {
                width: 86
                height: 28
                anchors.leftMargin: 5
                text: " Активний режим"
                anchors.horizontalCenterOffset: 0
                //horizontalAlignment: parent.horizontalAlignment
                //anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 20
            }
        }

        color: "#ca9e64"
        radius: 5
        border.color: "black"
        border.width: 2

        Rectangle {
            id: rectangle
            x: 8
            y: 38
            width: 384
            height: 124
            radius: 9
            color: "#ffffff"
            border.color: "black"

            Rectangle {
                x: 42
                y: 8
                width: 300
                height: 25
                anchors.topMargin: 12
                border.color: "#333333"
                border.width: 2
                color: "#DFD7C8"
                radius: 4
                anchors.horizontalCenterOffset: -173

                Text {
                    id: element2
                    width: 86
                    height: 28
                    text: "Сила цiлi"
                    anchors.horizontalCenterOffset: 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 20
                }
            }

            MySlider {
                id: minSize
                x: 8
                y: 39
                value: 0.01
                stepSize: 0.01
                from: 0.01
                to: 1

                Text {
                    width: 101
                    height: 38
                    text: qsTr("Мiнiмальне\nзначення ")
                    font.pixelSize: 15
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.right
                    anchors.leftMargin: 10

                    Text {
                        text: minSize.value.toFixed(2)
                        anchors.left: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 21
                        anchors.leftMargin: 5
                    }
                }
            }

            MySlider {
                id: maxSize
                x: 8
                y: 83
                value: 0.01
                stepSize: 0.01
                from: minSize.value
                to: 1

                Text {

                    width: 103
                    height: 38
                    text: qsTr("Максимальне\nзначення ")
                    anchors.verticalCenterOffset: 0
                    font.pixelSize: 15
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.right
                    anchors.leftMargin: 8

                    Text {
                        text: "0.01"
                        anchors.left: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 21
                        anchors.leftMargin: 5
                    }
                }
            }
        }

        CheckBoxGroup {
            id: checkBoxGroup
            offset: 0
            x: 8
            y: 168
            width: 140
            height: 124
            boxesText: ["Металеве \nпокриття", "Гумове \nпокриття"]
        }
    }

    Rectangle {
        id: passive
        anchors.top: row.top
        anchors.bottom: row.bottom
        anchors.right: row.right
        anchors.left: active.right
        color: "#642c2c"
        radius: 5
        border.color: "black"
        border.width: 2

        Rectangle {
            x: 54
            y: 0
            width: 170
            height: 25
            anchors.topMargin: 12
            border.color: "#333333"
            border.width: 2
            color: "#DFD7C8"
            radius: 4
            anchors.horizontalCenter: parent.horizontalCenter

            //anchors.horizontalCenterOffset: -173
            Text {
                width: 86
                height: 28
                anchors.leftMargin: 5
                text: " Пасивний режим"
                anchors.horizontalCenterOffset: 0
                font.pixelSize: 20
            }
        }

        ComboBox {
            visible: true
            x: comboBox.x
            y: comboBox.y
            width: comboBox.width
            height: comboBox.height
        }

        ComboBox {
            id: comboBox
            visible: !customCheckBox.checked
            x: 44
            y: 34
            width: 166
            height: 28
            model: ["1", "2", "3", "4"]
        }

        CheckBox {
            id: customCheckBox
            x: 228
            y: 35

            width: 26
            height: 26
            indicator: Rectangle {
                implicitWidth: 26
                implicitHeight: 26
                radius: 3
                border.color: "#333333"
                color: "#eeeeee"
                Rectangle {
                    width: 14
                    height: 14
                    x: 6
                    y: 6
                    radius: 2
                    color: "#FFC273"
                    border.color: "#333333"
                    visible: customCheckBox.checked
                }
            }
            //custom text
            contentItem: Text {
                text: "Автоматично \nзгенерований"
                width: 125
                verticalAlignment: Text.AlignVCenter
                height: 38
                font.pixelSize: 19
                anchors.left: parent.right
                anchors.leftMargin: 10
            }
        }

        Rectangle {
            id: rectangle2
            x: 8
            y: 67
            width: 384
            height: 150
            radius: 9
            color: "#ffffff"
            border.color: "black"

            Rectangle {
                x: 32
                y: 1
                width: 320
                height: 48
                anchors.topMargin: 12
                border.color: "#333333"
                border.width: 2
                color: "#DFD7C8"
                radius: 4
                anchors.horizontalCenterOffset: -135

                Text {
                    id: element
                    y: 0
                    width: 320
                    height: 40
                    font.pixelSize: 20
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTr("  Коефiцiент залежностi частоти\nвiд швидкостi пiдводного човна ")
                    anchors.horizontalCenterOffset: 8
                }
            }

            MySlider {
                id: minSizeF
                x: 8
                y: 49
                value: 2
                from: 1
                to: 100
                Text {
                    width: 103
                    height: 38
                    text: qsTr("Мiнiмальне\nзначення ")
                    font.pixelSize: 15
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.right
                    anchors.leftMargin: 10
                    Text {
                        text: minSizeF.value.toFixed(1)
                        font.pixelSize: 21
                        anchors.left: parent.right
                        anchors.leftMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    //anchors.verticalCenter: parent.verticalCenter
                }
            }

            MySlider {
                id: maxSizeF
                x: 8
                y: 79
                value: 10
                from: minSizeF.value
                to: 100
                Text {
                    width: 104
                    height: 38
                    text: qsTr("Максимальне\nзначення ")
                    font.pixelSize: 15
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.right
                    anchors.leftMargin: 10
                    Text {
                        text: maxSizeF.value.toFixed(1)
                        font.pixelSize: 21
                        anchors.left: parent.right
                        anchors.leftMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    //anchors.verticalCenter: parent.verticalCenter
                }
            }

            MySlider {
                id: amplificationFactor
                x: 8
                y: 109
                value: 1
                stepSize: 1
                from: 0
                to: 100
                Text {
                    width: 105
                    height: 38
                    text: qsTr("Пiдсилення \nзвуку ")
                    font.pixelSize: 15
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.right
                    anchors.leftMargin: 10
                    Text {
                        text: amplificationFactor.value.toFixed(1)
                        font.pixelSize: 21
                        anchors.left: parent.right
                        anchors.leftMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    //anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle {
            id: rectangle1
            x: 8
            y: 241
            width: 384
            height: 151
            radius: 9
            color: "#ffffff"
            border.color: "black"

            Rectangle {
                x: 42
                y: 0
                width: 300
                height: 25
                anchors.topMargin: 12
                border.color: "#333333"
                border.width: 2
                color: "#DFD7C8"
                radius: 4
                Text {
                    id: element1
                    width: 242
                    height: 28
                    text: qsTr("Рiвень випадкових шумiв")
                    anchors.horizontalCenterOffset: 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 20
                }
            }
            MySlider {
                id: minSizeN
                x: 8
                y: 20
                value: 0.9
                stepSize: 0.01
                to: 1
                from: 0.01
                Text {
                    width: 99
                    height: 22
                    text: qsTr("Частота")
                    font.pixelSize: 15
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.right
                    anchors.leftMargin: 10
                    Text {
                        text: minSizeN.value.toFixed(2)
                        font.pixelSize: 21
                        anchors.left: parent.right
                        anchors.leftMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    //anchors.verticalCenter: parent.verticalCenter
                }
            }

            MySlider {
                id: maxSizeN
                x: 8
                y: 50
                value: 0.01
                stepSize: 0.001
                to: 0.1
                from: 0.01

                Text {
                    width: 98
                    height: 22
                    text: qsTr("Рiвень шуму ")
                    anchors.verticalCenterOffset: 0
                    font.pixelSize: 15
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.right
                    anchors.leftMargin: 11
                    Text {
                        text: maxSizeN.value.toFixed(2)
                        font.pixelSize: 21
                        anchors.left: parent.right
                        anchors.leftMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    //anchors.verticalCenter: parent.verticalCenter
                }
            }

            MySlider {
                id: amplitude
                x: 8
                y: 80
                value: 0.01
                stepSize: 0.01
                to: 1
                from: 0

                Text {
                    width: 98
                    height: 22
                    text: qsTr("Амплiтуда ")
                    anchors.verticalCenterOffset: 0
                    font.pixelSize: 15
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.right
                    anchors.leftMargin: 11
                    Text {
                        text: amplitude.value.toFixed(2)
                        font.pixelSize: 21
                        anchors.left: parent.right
                        anchors.leftMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    //anchors.verticalCenter: parent.verticalCenter
                }
            }

            MySlider {
                id: frequency
                x: 8
                y: 113
                to: 4000
                from: 0
                stepSize: 1
                value: 2000
                Text {
                    width: 98
                    height: 22
                    text: "Частота шуму"
                    anchors.leftMargin: 11
                    anchors.left: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    Text {
                        text: frequency.value.toFixed(0)
                        anchors.leftMargin: 5
                        anchors.left: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 21
                    }
                    anchors.verticalCenterOffset: 0
                    font.pixelSize: 15
                }
            }
        }
    }

    Button {
        background: Rectangle {
            anchors.fill: parent
            border.color: "black"
            border.width: 2
        }

        x: 810
        y: 0
        width: 70
        height: 70
        text: "Закрити\nвiкно"
        onClicked: closePopup()
    }

    Popup {
        id: popup
        x: 100
        y: 100
        width: 200
        height: 300
        modal: true
        focus: true
        contentItem: Rectangle {
            color: "blue"
        }
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        exit: Transition {
            NumberAnimation { properties: "y"; from: 200; to: 1000 }

        }

        enter: Transition {

            NumberAnimation { properties: "y"; from: 1000; to: 200 }
        }
   }
}






