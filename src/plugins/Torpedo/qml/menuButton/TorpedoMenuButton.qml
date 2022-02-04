import QtQuick 2.15
import CQML 1.0

ImageButton{
    objectName: "Torpedo"
    iconCode: currentChecked ? "qrc:/resources/icons/torpedoBold.svg" : "qrc:/resources/icons/torpedoRegular.svg" ;
    textColor: currentChecked ? "#6ab3f3" : "#ADBDCB"
    buttonBackground: "#30384f"
    description: "Torpedos"
    onCurrentCheckedChanged: { if (currentChecked) scene.activateFilter("Torpedo") }
    elevation: 0
}
