import QtQuick 2.15
import CQML 1.0

IconButton{
    objectName: "Save"
    iconCode: IconHelper.icon["save"]
    description: "Saves"
    textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
    buttonBackground: "#30384f"
    font.family: currentChecked ? IconHelper.iconsPressed.name : IconHelper.icons.name
    //onCurrentCheckedChanged: { if (currentChecked) scene.activateFilter("Vessel") }
    enabled: sceneArea.scene.scenePlayer.mode === 2 // KScenePlayer.stop
    elevation: 0
}
