import QtQuick 2.15

Item {
    id: torpedoItem
    property real prevX: 0
    property real prevY: 0
    transformOrigin: Item.Center
    antialiasing: true
    x: modelData.x - width / 2
    y: -(modelData.y + height / 2)
    width: 10
    height: 30
    rotation: modelData.rotation
    property color color: "red"

    signal pressed()
    signal released()

    Text{
        anchors.fill: parent
        visible: sceneArea.scene.devMode
        rotation: -modelData.rotation
        transformOrigin: Item.Center
        text: visible ? ("" + Qt.point(modelData.x, -modelData.y)) : ""
    }

    Loader{
        anchors.fill: parent
        property string imageSource: sceneArea.scene.piService.params(modelData.objectName).imagePath
        sourceComponent: imageSource === "" ? nonImageComponent : imageComponent
    }

    onWidthChanged: modelData.width = width
    onHeightChanged: modelData.height = height

    MouseArea{
        anchors.fill: parent
        property double startX: 0
        property double startY: 0

        onPressed: {
            torpedoItem.pressed()
            startX = mouseX;
            startY = mouseY;
        }

        onReleased: torpedoItem.released()

        onMouseXChanged:  if (modelData.interactive) modelData.x = mapToItem(image, mouseX, mouseY).x - startX
        onMouseYChanged:  if (modelData.interactive) modelData.y = -(mapToItem(image, mouseX, mouseY).y - startY)
    }

    Component{
        id: imageComponent
        Image {
            source: imageSource
            anchors.fill: parent
            sourceSize.width: torpedoItem.width
            sourceSize.height: torpedoItem.height
        }
    }

    Component{
        id: nonImageComponent
        Item{
            anchors.fill: parent
            Rectangle{
                id: body
                width: parent.width
                height: parent.height - width
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                color: torpedoItem.color
                antialiasing: true
            }

            Rectangle{
                id: nose
                anchors.top: parent.top
                anchors.topMargin: width / 2 + 1
                anchors.horizontalCenter: parent.horizontalCenter
                width: torpedoItem.width / 1.2
                height: torpedoItem.width / 1.2
                transformOrigin: Item.Center
                radius: 3
                rotation: 45
                color: torpedoItem.color
                antialiasing: true
            }
        }
    }

    Component.onCompleted: { modelData.width = width; modelData.height = height; }
}


