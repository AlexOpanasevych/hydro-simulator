import QtQuick 2.0
import SceneItemsBase 1.0

BaseSceneItem {

    Rectangle{
        width: 20
        height: 20
        color: "yellow"

        property var sceneItem: loaderData
        property real prevX: 0
        property real prevY: 0
        transformOrigin: Item.Center
        antialiasing: true
        x: sceneItem.x
        y: -sceneItem.y
        rotation: sceneItem.rotation
    }
}
