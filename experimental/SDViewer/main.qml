import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.4 as Old
import KLib 1.0

Rectangle {
    id: root
    color: "#CAE4F2"
    Old.SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal
        //process tree
        ScrollView
        {
            id : scrollView
            Layout.maximumWidth: 400
            Layout.minimumWidth: 300
            clip: true
            ScrollBar.vertical.policy:ScrollBar.AlwaysOn
            ScrollBar.vertical.interactive: true
            ColumnLayout {
                id: column
                anchors.fill: parent
                Button
                {
                    id: button1
                    text: isOpened ? "close":"open"
                    property bool isOpened: false
                    onClicked:
                    {
                        TestObject.load();
                        isOpened = !isOpened
                    }
                    // @disable-check M16
                    contentItem: Text {
                        anchors.fill: parent
                        text: button1.text
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 15
                        padding: 10
                    }
                    // @disable-check M16
                    background: Rectangle {
                        implicitWidth: 100
                        implicitHeight: 40
                        color: "yellow"
                    }

                }
                Repeater {
                    model: button1.isOpened ? TestObject.processes : []
                    delegate: processDelegate
                }
                Item {
                    Layout.fillHeight: true
                }
            }
        }
        //table
        MyTable {
            id: mytable
            Layout.fillWidth: true
        }
    }
    //process delegate
    Component {
        id: processDelegate
        ColumnLayout{
            //header
            Rectangle {
                width: 300
                height: 20
                color: processExpandBtn.isOpen ? "#52C2FA"  : "#CAE4F2"
                RowLayout {
                    anchors.fill: parent
                    MyButton{
                        size: 20
                        id: processExpandBtn
                        property int procid: modelData.procid
                        onIsOpenChanged: {
                            modelData.load()
                        }
                    }
                    Text{
                        Layout.fillWidth: true
                        text: modelData.name
                        font.pixelSize: 15
                    }
                }
            }
            //childlist
            Repeater {
                model: processExpandBtn.isOpen ? modelData.pi: []
                delegate: piDelegate
            }
        }
    }

    //pi delegate
    Component {
        id: piDelegate
        ColumnLayout{
            //header
            Rectangle {
                width: 300
                height: 20
                color: piExpandBtn.isOpen ? "#52C2FA"  : "#CAE4F2"
                RowLayout {
                    anchors.fill: parent
                    //anchors.margins: 2
                    anchors.leftMargin: 20
                    MyButton{
                        id: piExpandBtn
                        size: 20
                        onIsOpenChanged: {
                            modelData.load(modelData.procid , modelData.pid)
                        }
                    }
                    Text{
                        Layout.fillWidth: true
                        text: modelData.pid
                        font.pixelSize: 15
                    }
                }
            }
            //childlist
            Repeater {
                model : piExpandBtn.isOpen ? modelData.sd : []
                delegate: sdDelegate
            }
        }
    }

    //sd delegate
    Component {
        id: sdDelegate
        ColumnLayout{
            id: sss
            property int piid: modelData.pid
            property int tyype: modelData.type
            property int reecordposition: modelData.recordposition
            property int proc: modelData.procid
            Rectangle {
                width: 300
                height: 20
                color: sdExpandBtn.isOpen ? "#52C2FA"  : "#CAE4F2"
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 40
                    MyButton{
                        id: sdExpandBtn
                        size: 20
                    }
                    Text{
                        Layout.fillWidth: true
                        text: modelData.type
                        font.pixelSize: 15
                    }
                }
            }
            //loader
            Repeater
            {
                model:sdExpandBtn.isOpen ? 1 : []
                delegate: Rectangle {
                    width: 300
                    height: 20
                    color: TableManager.filesMap[sss.proc+"_"+sss.piid+"_"+sss.tyype+"_"+sss.reecordposition] ? "#B3FF84" : "#FF8181"
                    RowLayout
                    {
                        anchors.fill: parent
                        anchors.leftMargin: 60
                        Rectangle {
                            width: 30
                            height: 20
                            color: "yellow"
                            Text {
                                anchors.fill: parent
                                text: "load"
                                font.pixelSize: 17
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    TableManager.getUnit({ "process" : sss.proc,
                                                             "pid" : sss.piid,
                                                             "type" : sss.tyype,
                                                             "recordposition" : sss.reecordposition })
                                }
                            }
                        }
                        Text {
                            text: sss.piid+"_"+sss.tyype+"_"+sss.reecordposition+".data"
                            font.pixelSize: 15
                        }
                    }
                }
            }
        }
    }
}
/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
