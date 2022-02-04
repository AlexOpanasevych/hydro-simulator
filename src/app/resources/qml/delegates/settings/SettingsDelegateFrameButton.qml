import QtQuick 2.15
import CQML 1.0

IconButton {
    iconCode: IconHelper.icon["cogs"]
    description: "Settings"
    textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
    buttonBackground: "#30384f"
    font.family: currentChecked ? IconHelper.iconsPressed.name : IconHelper.icons.name
    elevation: 0
}
