import QtQuick 2.0
import KLib 1.0

Item {
    id: root
    property var modelData;
    x: modelData.x;
    y: modelData.y

    width: image.sourceSize.width
    height: image.sourceSize.height
    property var value: modelData.value

    Image {
        visible: root.value > 0
        id: image
        source : "file:/" + App.resPath + "/" + modelData.additionalQml.imageSource
    }

    function updateHScale(scale) {
        y *= scale
        height *= scale
    }

    function updateWScale(scale) {
        x *= scale
        width *= scale
    }

}
