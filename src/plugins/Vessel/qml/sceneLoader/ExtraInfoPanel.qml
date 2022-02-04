import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.0
import Trajectory 1.0
import PluginsBase 1.0
import CQML 1.0 as C

Item{
    Item{
        id: extraInfo
        anchors.left: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: itemsLoader.item.height / 2
        anchors.leftMargin: -30
        width: 170
        height: 200
        Behavior on opacity { NumberAnimation { duration: 200 } }

        RowLayout{
            anchors.fill: parent
            spacing: 0
            Item{
                Layout.preferredHeight: 40
                Layout.preferredWidth: 40
                Layout.alignment: Qt.AlignBottom
                Image {
                    id: label
                    source: "qrc:/resources/icons/msgpart.png"
                    sourceSize.width: parent.width * 1.8
                    sourceSize.height: parent.height * 1.95
                }

                ColorOverlay {
                    anchors.fill: label
                    source: label
                    color: "#1f2936"
                }
            }

            C.BaseBackground{
                Layout.fillHeight: true
                Layout.fillWidth: true
                backgroundColor: "#1f2936"
                radius: 7
                elevation: 3
                MouseArea{
                    anchors.fill: parent
                    //onWheel: {}
                    onReleased: {
                        itemsLoader.item.pressed()
                    }
                }
                ColumnLayout{
                    anchors.fill: parent
                    spacing: 0
                    C.BaseBackground{
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.maximumHeight: 35
                        elevation: 5
                        radius: 7
                        elevationInverted: true
                        backgroundColor: Qt.lighter("#1f2936", 1.5)
                        height: 35
                        Text{
                            anchors.fill: parent
                            text: modelData.objectName + " " + root.z
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            elide: Text.ElideRight
                            font.pixelSize: parent.height / 2
                            color: "#6ab2f2"
                        }
                    }
                    Text{
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.leftMargin: 5
                        Layout.maximumHeight: 30
                        text: "x: " + modelData.x.toFixed(2)
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        font.pixelSize: 15
                        color: "#6ab2f2"
                    }
                    Text{
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.leftMargin: 5
                        Layout.maximumHeight: 30
                        text: "y: " + -modelData.y.toFixed(2)
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        font.pixelSize: 15
                        color: "#6ab2f2"
                    }
                    Text{
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.leftMargin: 5
                        Layout.maximumHeight: 30
                        text: "шв: " + -modelData.velocity.toFixed(2) + " вуз."
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        font.pixelSize: 15
                        color: "#6ab2f2"
                    }
                    Item{
                        Layout.fillHeight: true
                    }
                }
            }
        }
    }
}
