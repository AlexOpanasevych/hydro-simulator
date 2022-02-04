import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

TextField {
    id: control

    color: "green"
    property color errorColor: "red"
    property string helperText: ""
    property bool floatingLabel: true
    property bool hasError: characterLimit && length > characterLimit
    property int characterLimit
    property bool showBorder: true
    property color hintColor: "green"
    property int mult: 1
    hoverEnabled: true
    echoMode: TextInput.Normal
    font.pixelSize: height * 0.5
    selectByMouse: true
    implicitHeight: 30
    property string floatingText: ""
    placeholderTextColor: "green"
    placeholderText: ""
    selectedTextColor: "white"
    selectionColor: control.hasOwnProperty("color") ? control.color : "blue"

    //textColor: "black"

    background : Item {
        id: background

        property color color: control.color
        property color errorColor: control.hasOwnProperty("errorColor")
                                   ? control.errorColor : "red"
        property string helperText: control.hasOwnProperty("helperText") ? control.helperText : ""
        property bool floatingLabel: control.hasOwnProperty("floatingLabel") ? control.floatingLabel : ""
        property bool hasError: control.hasOwnProperty("hasError")
                                ? control.hasError : characterLimit && control.length > characterLimit
        property int characterLimit: control.hasOwnProperty("characterLimit") ? control.characterLimit : 0
        property bool showBorder: control.hasOwnProperty("showBorder") ? control.showBorder : true

        Rectangle {
            id: underline
            color: background.hasError ? background.errorColor
                                       : control.activeFocus ? background.color
                                                             : control.hintColor

            height: (control.activeFocus ? 2 : 1) * parent.height * 0.05
            visible: background.showBorder

            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            Behavior on color { ColorAnimation { duration: 200 } }
            Behavior on height { NumberAnimation { duration: 200 } }
        }


        Label {
            id: fieldPlaceholder
            anchors.verticalCenter: parent.verticalCenter
            text: control.floatingText
            font.pixelSize: control.font.pixelSize
            anchors.margins: -12
            color: background.hasError ? background.errorColor
                                       : control.activeFocus && control.text !== ""
                                         ? background.color : control.hintColor
            renderType: Text.QtRendering
            states: [
                State {
                    name: "floating"
                    when: (control.displayText.length > 0 && background.floatingLabel) || (control.activeFocus && background.floatingLabel)
                    AnchorChanges {
                        target: fieldPlaceholder
                        anchors.verticalCenter: undefined
                        anchors.top: parent.top
                    }
                    PropertyChanges {
                        target: fieldPlaceholder
                        font.pixelSize: control.font.pixelSize * 0.7
                    }
                },
                State {
                    name: "hidden"
                    when: control.displayText.length > 0 && !background.floatingLabel
                    PropertyChanges {
                        target: fieldPlaceholder
                        visible: false
                    }
                }
            ]

            transitions: [
                Transition {
                    id: floatingTransition
                    enabled: false
                    AnchorAnimation {
                        duration: 200
                    }
                    NumberAnimation {
                        duration: 200
                        property: "font.pixelSize"
                    }
                }
            ]

            Component.onCompleted: floatingTransition.enabled = true
        }

        RowLayout {
            anchors {
                left: parent.left
                right: parent.right
                top: underline.top
                topMargin: 4 * mult
            }

            Label {
                id: helperTextLabel
                visible: background.helperText && background.showBorder
                text: background.helperText
                font.pixelSize: 12 * mult
                color: background.hasError ? background.errorColor
                                           : Qt.darker("gray")
                renderType: Text.QtRendering
                Behavior on color {
                    ColorAnimation { duration: 200 }
                }

                property string helperText: control.hasOwnProperty("helperText")
                                            ? control.helperText : ""
            }

            Label {
                id: charLimitLabel
                renderType: Text.QtRendering
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                visible: background.characterLimit && background.showBorder
                text: control.length + " / " + background.characterLimit
                font.pixelSize: 12 * mult
                color: background.hasError ? background.errorColor : "black"
                horizontalAlignment: Text.AlignLeft

                Behavior on color {
                    ColorAnimation { duration: 200 }
                }
            }
        }
    }
}
