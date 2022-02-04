import QtQuick 2.0
import CQML 1.0

ImageButton{
    iconCode: currentChecked ? "qrc:/resources/icons/torpedoBold.svg" : "qrc:/resources/icons/torpedoRegular.svg" ;
    textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
    buttonBackground: "#30384f"
    description: "Torpedos"
    onCurrentCheckedChanged: if (currentChecked){
                                 if (sceneArea.scene.piService.currentChoosed)
                                     sceneArea.scene.piService.currentChoosed.pressed()
                                 sceneArea.scene.piService.show("Torpedo")
                             }
    elevation: 0
}
