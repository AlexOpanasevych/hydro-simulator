import QtQuick 2.15
import QtQuick.Controls 2.15
import CQML 1.0 as C

C.IconButton{
    id: centerButton
    property bool hideable: false
    buttonBackground: "#1f2936"
    textColor: currentChecked ? "#6ab2f2" : "silver"
    font.family: currentChecked ? C.IconHelper.iconsPressed.name : C.IconHelper.icons.name
    iconCode: "\uF05A"
    description: "extra info"
    descriptionSize: 10
    elevation: currentChecked ? 3 : 0
    internalShadow: currentChecked
    currentChecked: sceneArea.extraInfo
    onReleased: {
        if (!currentChecked)
        {
            sceneArea.extraInfo = true
        }
        else
        {
            sceneArea.extraInfo = false
        }
    }
}
