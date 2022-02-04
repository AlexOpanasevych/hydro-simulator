import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQml.Models 2.12
import QtQuick.Controls.Styles 1.4

Item{
    Text{
        text: "Тип гідрології"
        font: comboBox.font
        color: "#333333"
        anchors.horizontalCenter: comboBox.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.bottom: comboBox.top
        anchors.bottomMargin: 5
    }
    ComboBox {
        id: comboBox
        y : 35
        x : 20
        currentIndex: 0
        model: [ "1", /*"2", "3", "4", */"5"/*, "6", "7"*/ ]

        width: 200
        onCurrentIndexChanged: simulation.season = (comboBox.currentIndex == 0 ? 0 : 5)
    }

    CheckBox {
        id: checkBox
        x: 20
        y: 80
        checked: true
        checkable: false
        property bool mode: simulation.reflected
        indicator: Rectangle {
            implicitWidth: 26
            implicitHeight: 26
            x: checkBox.leftPadding
            y: parent.height / 2 - height / 2
            radius: 6
            border.color: "#333333"
            color: "#eeeeee"
            Rectangle {
                width: 18
                height: 18
                x: 4
                y: 4
                radius: 6
                color: "red"
                border.color: "#333333"
                visible: !checkBox.mode
            }
        }

        Text {
           text: "Зона тiнi"
           font: checkBox.font
           color: "#333333"
           anchors.verticalCenter: parent.verticalCenter
           horizontalAlignment: Text.AlignHCenter
           verticalAlignment: Text.AlignVCenter
           anchors.left: parent.right
        }
    }

    CustomCheckBox{
        id: includeHydrology
        x: 20
        y: 110
        text: "Врахувати гідрологію"
        idx: 1
        mode: 1
        onClicked: {
            mode = -mode
        }
        onModeChanged: {
            if (mode === 1)
            {
                simulation.hydrologyState = true
            }
            else
            {
                simulation.hydrologyState = false
            }
        }
    }
}
