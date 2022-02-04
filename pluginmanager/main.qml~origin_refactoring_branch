import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 1.5
import QtQuick.Layouts 1.15
import KLib 1.0

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Plugin Manager")
    property KFlexibleModel flexModel: pluginManager.model

    PluginManager { id: pluginManager }

    ColumnLayout{
        width: window.width
        height: window.height
        spacing: 0

        Rectangle{
            id: headerRect
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "#11581B"

            Loader{
                id: loaderHeader
                anchors.fill: parent
                property string l_DATE: "Date Modified"
                property string l_NAME: "Name"
                property string l_PATH: "Path"
                property string l_ACTIVE: "Active"
                property var l_ACTIVEF: (act) => { activeAll(act) }
                sourceComponent: Line { anchors.fill: parent }
            }
        }

        Rectangle{
            Layout.fillWidth: true
            Layout.preferredHeight: 2
            color: "#11581B"
        }

        Item{
            Layout.fillWidth: true
            Layout.fillHeight: true
            ListView{
                id: listView
                anchors.fill: parent
                model: flexModel
                clip: true
                spacing: 0
                delegate: Rectangle{
                    width: listView.width
                    height: 30
                    color: "#4E7A54"
                    Loader{
                        anchors.fill: parent
                        property string l_DATE: DATEMODIFIED
                        property string l_NAME: NAME
                        property string l_PATH: PATH
                        property bool l_ACTIVE: ACTIVE
                        property var l_ACTIVEF: (act) => { flexModel.setLineData({ "ACTIVE" : act }, index) }
                        sourceComponent: Line { anchors.fill: parent }
                    }
                }
            }
        }

        Item{
            Layout.preferredHeight: 35
            Layout.fillWidth: true
            RowLayout{
                anchors.fill: parent
                anchors.margins: 5
                Button{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: "save"
                    onClicked: {
                        pluginManager.save()
                    }
                }

                Button{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: "update"
                    onClicked: {
                        pluginManager.loadConfig()
                    }
                }
            }
        }
    }


    function activeAll(active)
    {
        for (let k = 0; k < flexModel.rowCount(); k++)
        {
            flexModel.setLineData({ "ACTIVE" : active}, k)
        }
    }
}
