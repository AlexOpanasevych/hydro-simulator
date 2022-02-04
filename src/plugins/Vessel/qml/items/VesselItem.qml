import QtQuick 2.15

Item {
    id: shipItem
    property real prevX: 0
    property real prevY: 0
    transformOrigin: Item.Center
    antialiasing: true
    x: modelData.x - width / 1.5
    y: -(modelData.y + height / 1.5)
    property color color: "blue"
    width: 20
    height: 20
    rotation: /*modelData.rotation*/ 0

    signal pressed()
    signal released()

    Loader{
        anchors.fill: parent
        property string imageSource: sceneArea.scene.piService.params(modelData.objectName).imagePath
        sourceComponent: imageSource === "" ? nonImageComponent : imageComponent
    }

    Text{
        anchors.fill: parent
        visible: sceneArea.scene.devMode
        rotation: -modelData.rotation
        transformOrigin: Item.Center
        text: visible ? ("" + Qt.point(modelData.x, -modelData.y)) : ""
        verticalAlignment: Text.AlignVCenter
        antialiasing: true
    }

    onWidthChanged: modelData.width = width
    onHeightChanged: modelData.height = height

    MouseArea{
        anchors.fill: parent
        property double startX: 0
        property double startY: 0

        onPressed: {
            shipItem.pressed()
            startX = mouseX;
            startY = mouseY;
        }

        onReleased: shipItem.released()

        onMouseXChanged:  if (modelData.interactive) modelData.x = mapToItem(image, mouseX, mouseY).x
        onMouseYChanged:  if (modelData.interactive) modelData.y = -(mapToItem(image, mouseX, mouseY).y )
    }

    Component{
        id: imageComponent
        Image {
            source: "file:///" + imageSource
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit

//            sourceSize.width: shipItem.width
//            sourceSize.height: shipItem.height
        }
    }

    Image{
        z: 2
        enabled: modelData.dead
        visible: modelData.dead
        source: "qrc:/Vessel/dead.png"
        width: 50
        height: 50
//        speed: 4
//        x: modelData.x - height / 2
//        y: -(modelData.y - width / 2)
        anchors.centerIn: parent
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
                color: shipItem.color
                antialiasing: true
            }

            Rectangle{
                id: nose
                anchors.top: parent.top
                anchors.topMargin: width / 2 + 1
                anchors.horizontalCenter: parent.horizontalCenter
                width: shipItem.width / 1.2
                height: shipItem.width / 1.2
                transformOrigin: Item.Center
                radius: 3
                rotation: 45
                color: shipItem.color
                antialiasing: true
            }
        }
    }

    Component.onCompleted: { modelData.width = width; modelData.height = height; }
}

