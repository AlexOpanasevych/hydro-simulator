import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
//import AudioModule 1.0
import KLib 1.0

Rectangle {


    color: "transparent"
//    Button {
//        id: exitButton
//        anchors {
//            top: parent.top
//            left: parent.left
//        }
//        text: "X"
//        onPressed: {
//            settingsStackView.pop()
//        }
//    }

    ColumnLayout {
        anchors {
//            top: exitButton.bottom
            left: parent.left
            margins: 5
        }
        RowLayout {
            Text {
                id: mixerName1
                text: qsTr("Distance")
            }
            Slider {
                handle.height: height / 3
                handle.width: height / 3
                onValueChanged: {
                    AudioModule.audioService.distance = value
                }
            }
        }
        RowLayout {
            Text {
                id: mixerName2
                text: qsTr("Frequency")
            }
            Slider {
                id: frequencySlider
                from: 0
                to: 10000
                handle.height: height / 3
                handle.width: height / 3
                onValueChanged: {
                    AudioModule.audioService.rootSound.frequency = value
                }
            }
        }
        RowLayout {
            Text {
                id: mixerName3
                text: qsTr("Amplitude")
            }

            Slider {
                id: ampSlider
                from: 0
                to: 3
                value: 1
                handle.height: height / 3
                handle.width: height / 3
                onValueChanged: {
                    AudioModule.audioService.rootSound.amplitude = value
                }
            }
        }
        RowLayout {
            Text {
                id: mixerName4
                text: qsTr("Delay")
            }

            Slider {
                handle.height: height / 3
                handle.width: height / 3
                onValueChanged: {
                    AudioModule.audioService.interval = value
                }
            }
        }

//        RowLayout {
//            Text {
//                id: mixerName5
//                text: qsTr("Period")
//            }

//            Slider {
//                enabled: false
//                handle.height: height / 3
//                handle.width: height / 3
//                value: 1 / frequencySlider.value
//                onValueChanged: {
//                    AudioModule.audioService.period = value
//                }
//            }
//        }

        RowLayout {
            Text {
                id: mixerName6
                text: qsTr("Bearing")
            }

            Slider {
                handle.height: height / 3
                handle.width: height / 3
                onValueChanged: {
                    AudioModule.audioService.bearing = value
                }
            }
        }

        RowLayout {
            Text {
                id: mixerName7
                text: qsTr("Гучність")
            }

            Slider {
                value: AudioModule.audioService.volume
                handle.height: height / 3
                handle.width: height / 3
                onValueChanged: {
//                    console.log(value)
                    AudioModule.audioService.volume = value
                }
            }
        }
    }
}
