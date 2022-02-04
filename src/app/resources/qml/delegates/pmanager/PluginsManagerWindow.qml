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
    property bool frameAvailable: false
    DelegateModel {
        id: visualModel
        model: PluginsManager.plugins
        delegate: PluginsManagerDragDelegate{}
    }

    ColumnLayout{
        id: layouts
        anchors.fill: parent
        spacing: 0

        C.BaseBackground{
            Layout.minimumHeight: 40
            backgroundColor: "#1f2936"
            elevation: 3
            Layout.fillWidth: true

            Text {
                id: typeText
                anchors.fill: parent
                color: "#6ab3f3"
                text: "PLUGINS MANAGER"
                font.pixelSize: 17
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap

                Shape{
                    Layout.fillWidth: true
                    Layout.preferredHeight: 5
                    ShapePath {
                        fillColor: "transparent"
                        strokeColor: "#7f8c92"
                        strokeWidth: 1
                        startX: 0
                        startY: typeText.height - 1
                        PathLine { x: typeText.width; y: typeText.height - 1 }
                    }
                }

                Rectangle{
                    color: "white"
                    anchors.fill: typeText
                    opacity: 0.1
                }
            }
        }

        ColumnLayout{
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView{
                id: savesListView
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 5
                Layout.rightMargin: 5
                model: visualModel
                clip: true
            }

            C.BaseBackground{
                backgroundColor: "#1f2936"
                elevation: 3
                inverted: true
                Layout.preferredHeight: 35
                Layout.fillWidth: true
            }
        }
    }
}
