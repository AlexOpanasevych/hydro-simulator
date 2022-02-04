import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls 1.4 as Old
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.15

import "../controls" as C
import "../base"

import KLib 1.0

QtObject {
    property list<Component> tabButtons: [
        Component {
            C.IconButton
            {
                iconCode: IconHelper.icon["ship"]//"\uf21a"
                description: "Ships"
                textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
                buttonBackground: "#30384f"
                font.family: currentChecked ? IconHelper.iconsPressed.name : IconHelper.icons.name
                //onCurrentCheckedChanged: console.log(currentChecked, "ch")
                onCurrentCheckedChanged: { if (currentChecked) scene.activateFilter(KScene.SHIP) }
            }
        },
        Component {
            C.IconButton
            {
                iconCode: qsTr("\uF773")
                description: "Waves"
                textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
                buttonBackground: "#30384f"
                font.family: currentChecked ? IconHelper.iconsPressed.name : IconHelper.icons.name
            }
        },
        Component {
            C.ImageButton
            {
                iconCode: currentChecked ? "qrc:/resources/icons/torpedoBold.svg" : "qrc:/resources/icons/torpedoRegular.svg" ;
                textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
                buttonBackground: "#30384f"
                description: "Torpedos"
                onCurrentCheckedChanged: { if (currentChecked) scene.activateFilter(KScene.TORPEDO) }
            }
        }
        ,
        Component {
            C.ImageButton
            {
                iconCode: currentChecked ? "qrc:/resources/icons/submarineBold.svg" : "qrc:/resources/icons/submarineRegular.svg" ;
                textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
                buttonBackground: "#30384f"
                description: "Submarines"
                onCurrentCheckedChanged: { if (currentChecked) scene.activateFilter(KScene.SUBMARINE) }
            }
        }
        ,
        Component {
            C.IconButton
            {
                iconCode: IconHelper.icon["map_marker_edit"]
                description: "Edit target"
                textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
                buttonBackground: "#30384f"
                font.family: currentChecked ? IconHelper.iconsPressed.name : IconHelper.icons.name
            }
        }
    ]

    //property list<Component> tabWindows: []
}
