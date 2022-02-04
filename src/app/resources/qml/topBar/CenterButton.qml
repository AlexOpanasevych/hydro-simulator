import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import KLib 1.0
import CQML 1.0
import "../topBar"

IconButton{
    id: centerButton
    property bool hideable: false
    buttonBackground: "#1f2936"
    textColor: currentChecked ? "#6ab2f2" : "silver"
    font.family: currentChecked ? IconHelper.iconsPressed.name : IconHelper.icons.name
    iconCode: IconHelper.icon["crosshair"]
    description: "center"
    descriptionSize: 10
    elevation: currentChecked ? 3 : 0
    internalShadow: currentChecked
    onReleased: {
        sceneArea.moveToCenter()
    }
}
