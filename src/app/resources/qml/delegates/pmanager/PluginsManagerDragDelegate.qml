import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQml.Models 2.1
import QtQuick.Controls 2.15
import CQML 1.0 as C

MouseArea {
    id: dragArea
    property bool held: false
    anchors {
        right: parent ? parent.right : undefined;
        left: parent ? parent.left : undefined
    }
    height: content.height
    drag.target: held ? content : undefined
    drag.axis: Drag.YAxis
    onPressed: {
          dragArea.held = true
    }

    onReleased: held = false

    DropArea {
        anchors { fill: parent; margins: 10 }

        onEntered: {
            visualModel.items.move(
                        drag.source.DelegateModel.itemsIndex,
                        dragArea.DelegateModel.itemsIndex)
        }
    }

    Binding{
        target: modelData
        property: "priority"
        value: dragArea.DelegateModel.itemsIndex + 1
    }

    C.BaseBackground {
        id: content
        clip: true
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }
        height: 40
        width: dragArea.width;
        backgroundColor: dragArea.held ? Qt.darker("#1f2936", 1.3) : "#1f2936"
        elevation: dragArea.held ? 5 : 0
        radius: 2
        Drag.active: dragArea.held
        Drag.source: dragArea
        Drag.hotSpot.x: width / 2
        Drag.hotSpot.y: height / 2
        states:[
            State {
                when: dragArea.held
                ParentChange { target: content; parent: rootWindow }
                AnchorChanges {
                    target: content
                    anchors { horizontalCenter: undefined; verticalCenter: undefined }
                }
            }
        ]
        RowLayout{
            anchors.fill: parent
            Text{
                text: modelData.priority
                verticalAlignment: Text.AlignVCenter
                Layout.preferredWidth: 20
                Layout.fillHeight: true
                color: "#6ab3f3"
            }
            Text{
                text: modelData.name
                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "#6ab3f3"
            }
            C.CheckBox{
                textColor:  "#6ab3f3"
                indicatorMarkColor:  "#6ab3f3"
                indicatorRectColor: dragArea.held ? Qt.darker("#1f2936", 1.3) : "#1f2936"
                indicatorBorderColor: "#6ab3f3"
                text: ""
                Layout.preferredWidth: height
                Layout.fillHeight: true
                onCheckedChanged: modelData.enabled = checked
                Component.onCompleted: checked = modelData.enabled
            }
        }
    }
}
