import QtQuick 2.15
import CQML 1.0

IconButton{
    objectName: "Station"
    iconCode: "\uF8B1"
    description: "Station"
    textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
    buttonBackground: "#30384f"
    font.family: currentChecked ? IconHelper.iconsPressed.name : IconHelper.icons.name
    //enabled: sceneArea.scene.scenePlayer.mode === 2 // KScenePlayer.stop
    elevation: 0
}
