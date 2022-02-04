import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: root

    property alias slider: volumeSlider

    property var onVChanged
    Button {
        id: rootButton

        property bool isOpen: false

        font.family: "Font Awesome 5 Pro Regular"

        anchors.fill: parent

        text: volumeSlider.value <= 33 ? "\uF026" : volumeSlider.value <= 66 && volumeSlider.value > 33 ? "\uF027" : "\uF028"

        onClicked: {
            isOpen = !isOpen
        }
    }

    Slider {

        anchors.top: rootButton.bottom

        orientation: Qt.Vertical

        id: volumeSlider

        visible: rootButton.isOpen

        from: 0
        to: 100

        states: [State {
            name: "opened"; when: visible
            PropertyChanges { target: volumeSlider; opacity: 100}
        },
            State {
                name: "closed"; when: !visible
                PropertyChanges {
                    target: volumeSlider
                    opacity: 0
                }
            }]
        transitions: [
            Transition {
                from: "closed"
                to: "opened"
                NumberAnimation {property: "opacity"; easing.type: Easing.InOutQuad}
            },
            Transition {
                from: "opened"
                to: "closed"
                NumberAnimation {property: "opacity"; easing.type: Easing.InOutQuad}
            }
        ]

        onValueChanged: {
            onVChanged(value)
        }
    }
}
