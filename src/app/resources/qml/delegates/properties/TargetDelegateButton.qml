import QtQuick 2.15
import CQML 1.0

IconButton{
    objectName: "Edit"
    iconCode: IconHelper.icon["map_marker_edit"]
    description: "Edit target"
    textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
    buttonBackground: "#30384f"
    font.family: currentChecked ? IconHelper.iconsPressed.name : IconHelper.icons.name
}
