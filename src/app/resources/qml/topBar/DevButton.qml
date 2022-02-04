import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import KLib 1.0
import CQML 1.0
import "../topBar"

IconButton{
    id: devButton
    property bool hideable: false
    Layout.fillHeight: true
    Layout.maximumWidth: 45
    Layout.preferredWidth: 45
    buttonBackground: "#1f2936"
    textColor: currentChecked ? "#6ab2f2" : "silver"
    font.family: currentChecked ? IconHelper.iconsPressed.name : IconHelper.icons.name
    iconCode: IconHelper.icon["magic"]
    description: "devMode"
    elevation: currentChecked ? 3 : 0
    descriptionSize: 10
    internalShadow: currentChecked
    currentChecked: (sceneArea.scene.devMode)

    onReleased: {
        sceneArea.scene.devMode = !sceneArea.scene.devMode
    }
}
