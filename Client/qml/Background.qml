import QtQuick 2.0
import KLib 1.0

Item {
    id: root
    property var modelData;
    x: 0
    y: 0
    width: modelData.width
    height: modelData.height
    Loader {
        id : loader
        sourceComponent: modelData.additionalQml.sourceComponent === "Image" ? imageComponent : rectangleComponent

        onLoaded: {
            switch(sourceComponent){
            case imageComponent:
                loader.item.source = "file:/" + App.resPath + "/" + modelData.additionalQml.imageSource
                break
            case rectangleComponent:
                loader.item.color = modelData.color !== undefined ? modelData.color : loader.item.color
                break
            }

            console.log("Background created:")
            console.log("width:", width)
            console.log("height:", height)

        }
    }

    Component {
        id: rectangleComponent

        Rectangle {
                z : 0
            height: root.height
            width: root.width
        }
    }

    Component {
        id: imageComponent
        Image {
            z : 0
//            anchors.fill: parent
            height: root.height
            width: root.width
        }
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
