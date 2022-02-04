import QtQuick 2.15
import CQML 1.0

IconButton{
    objectName: "Vessel"
    iconCode: IconHelper.icon["ship"]//"\uf21a"
    description: "Vessels"
    textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
    buttonBackground: "#30384f"
    font.family: currentChecked ? IconHelper.iconsPressed.name : IconHelper.icons.name
    onCurrentCheckedChanged: { if (currentChecked) scene.activateFilter("Vessel") }
    elevation: 0
}
