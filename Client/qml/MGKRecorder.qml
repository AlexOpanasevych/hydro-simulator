import QtQuick 2.0

import KLib 1.0

Item {
    property var modelData;

    x: modelData.x
    y: modelData.y
    width:modelData.width
    height: modelData.height

    CustomItem {
        anchors.fill: parent
        backgroundImageSource: App.resPath + "/" + modelData.additionalQml.backgroundImage
        linealImageSource: App.resPath + "/" + modelData.additionalQml.linealImage
        frameImageSource: App.resPath + "/" + modelData.additionalQml.frameImage

        Component.onCompleted: {
            console.log(x, y)
        }

    }

}
