import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import KLib 1.0
import CQML 1.0
import "../topBar"

IconButton{
    id: moveButton
    property bool hideable: true
    buttonBackground: "#1f2936"
    textColor: currentChecked ? "#6ab2f2" : "silver"
    font.family: currentChecked ? IconHelper.iconsPressed.name : IconHelper.icons.name
    iconCode: IconHelper.icon["move"]
    description: "move"
    descriptionSize: 10
    elevation: currentChecked ? 3 : 0
    currentChecked: (sceneArea.workMode === "MOVE")
    internalShadow: currentChecked
    onReleased: {
        if (!currentChecked)
        {
            sceneArea.workMode = "MOVE"
        }
        else
        {
            sceneArea.workMode = "DEFAULT"
        }
    }
}
