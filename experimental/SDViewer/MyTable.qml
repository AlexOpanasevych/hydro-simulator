import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.0
import KLib 1.0

ColumnLayout{
    id : mytable
    //creating tabbar with tabbuttons
    TabBar{
        id : tabbar
        Layout.fillWidth: true
        height:  mytable.height/20
        clip: true
        //creating custom tabbuttons
        Repeater{
            model : TableManager.model
            delegate: TabButton{
                text: "Record "+modelData.name
                width: Math.min(tabbar.width/6-1 ,tabbar.width/TableManager.tablesCount - 1)
                height: tabbar.height
                clip: true
                background: Rectangle{
                    color: "#4682b4"
                    clip: true
                    Rectangle{
                        anchors.fill: parent
                        anchors.leftMargin: parent.width  - 13
                        anchors.bottomMargin: parent.height - 13
                        width: 13
                        height: 13
                        color: "red"
                        Text {
                            id:txt
                            text: qsTr("x")
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        MouseArea{
                            preventStealing: false
                            anchors.fill: parent
                            onClicked: {
                                TableManager.deleteUnit(modelData.map)
                            }
                        }
                    }
                }
            }
        }
    }
    //tableview
    StackLayout{
        Layout.fillHeight: true
        Layout.fillWidth: true
        currentIndex: tabbar.currentIndex
        Repeater{
            model: TableManager.model
            delegate: table
        }
        Component{
            id: table
            //table rectangle
            Rectangle{
                id : tableractangle
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "#CAE4F2"
                clip: true
                //table view and all on this page
                TableView {
                    id: tableView
                    anchors.fill: parent
                    anchors.leftMargin: rowsHeader.implicitWidth
                    anchors.topMargin: columnsHeader.implicitHeight
                    property int currentColumn: -1
                    property int currentRow: -1
                    property int spotwidth: 80
                    property int spotheight: 32
                    
                    model: modelData.model
                    //scroll bars for tableview
                    ScrollBar.horizontal: ScrollBar{
                        hoverEnabled: true
                        policy: ScrollBar.AlwaysOn
                        contentItem: Rectangle {
                            implicitWidth: tableractangle.width/90
                            implicitHeight: tableractangle.height/90
                            radius: width
                            color: "#4A4C4E"
                        }
                    }
                    ScrollBar.vertical: ScrollBar{
                        hoverEnabled: true
                        policy: ScrollBar.AlwaysOn
                        //                                size: 30
                        contentItem: Rectangle {
                            implicitWidth: tableractangle.width/90
                            implicitHeight: tableractangle.height/90
                            radius: width
                            color: "#4A4C4E"
                        }
                    }
                    
                    //one spot in table
                    // @disable-check M16
                    delegate: Rectangle {
                        id : tablespot
                        implicitWidth: tableView.spotwidth
                        implicitHeight: tableView.spotheight
                        color: tableView.currentRow === row &&
                               tableView.currentColumn === column ? "green" : "transparent"
                        border.color : "black"
                        border.width : 1
                        Text {
                            text: display
                            font.pixelSize: 13
                            anchors.centerIn: parent
                        }
                        
                        MouseArea{
                            anchors.fill: parent
                            
                            onClicked: {
                                if (tableView.currentColumn === column && tableView.currentRow === row)
                                {
                                    tableView.currentColumn = -1
                                    tableView.currentRow = -1
                                }
                                else
                                {
                                    tableView.currentColumn = column
                                    tableView.currentRow = row
                                }
                            }
                        }
                    }
                    //selected line
                    Rectangle{
                        id: highlight
                        width: tableView.width
                        height: tableView.spotheight
                        y: (tableView.currentRow) * tableView.spotheight
                        color: "#49ACE0"
                        opacity: 0.7
                        visible: tableView.currentRow !== -1
                        //animation on y!!!!!!!
                        Behavior on y { NumberAnimation { duration: 200 ; easing.type: Easing.OutCubic } }
                    }
                    //row headers
                    Row {
                        id: columnsHeader
                        y: tableView.contentY - tableView.spotheight
                        z: 2
                        Repeater {
                            model: tableView.columns > 0 ? tableView.columns : 1
                            Rectangle{
                                id : lablebackground
                                width: tableView.spotwidth
                                height: tableView.spotheight
                                property bool sortedByIncrease: true
                                Rectangle {
                                    anchors.fill: parent;
                                    color: modelData  === tableView.currentColumn ? "#AA76DD" : "yellow"
                                    Text {
                                        anchors.fill: parent
                                        text: tableView.model.headerData(modelData ,Qt.Horizontal) + (lablebackground.sortedByIncrease ? "-": "^" )
                                        color: 'black'
                                        font.pixelSize: 12
                                        padding: 10
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }

                                
                                MouseArea
                                {
                                    anchors.fill: parent
                                    onClicked:
                                    {
                                        lablebackground.sortedByIncrease  = !lablebackground.sortedByIncrease
                                        tableView.model.sort(modelData , lablebackground.sortedByIncrease)
                                    }
                                }
                            }
                        }
                    }
                    //column headers
                    Column {
                        id: rowsHeader
                        x: tableView.contentX  - tableView.spotwidth
                        z: 2
                        Repeater {
                            model: tableView.rows > 0 ? tableView.rows : 1
                            Rectangle{
                                width: tableView.spotwidth
                                height: tableView.spotheight
                                Label {
                                    anchors.fill: parent
                                    text:tableView.model.headerData(modelData ,Qt.Vertical)
                                    color: "black"
                                    font.pixelSize: 15
                                    padding: 10
                                    verticalAlignment: Text.AlignVCenter
                                    // @disable-check M16
                                    background: Rectangle { color: tableView.currentRow === index ? "#AA76DD" : "#ffa07a" }
                                }
                                MouseArea
                                {
                                    anchors.fill: parent
                                    onClicked:
                                    {
                                        if (tableView.currentRow === index)
                                        {
                                            tableView.currentRow = -1
                                        }
                                        else
                                        {
                                            tableView.currentRow = index
                                        }
                                    }
                                }
                            }
                        }
                    }
                    ScrollIndicator.horizontal: ScrollIndicator { }
                    ScrollIndicator.vertical: ScrollIndicator { }
                }
                
            }
        }
    }
}
