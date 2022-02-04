import QtQuick 2.0
import KLib 1.0
Item {
    id: root
    property var modelData;

    property vector2d basis: Qt.vector2d(0, -1);

    property real imageRotation: modelData.additionalQml.imageRotation !== undefined ? modelData.additionalQml.imageRotation : 270
    property string onImage : "file:/" + App.resPath + "/" + modelData.additionalQml.onImage
    property string offImage: "file:/" + App.resPath + "/" + modelData.additionalQml.offImage
    property var processor

    property bool value: modelData.value

    x : modelData.x;
    y : modelData.y
    width : modelData.width
    height : modelData.height

    QtObject {
        id: internal
        property vector2d startPos: Qt.vector2d(0, 0)
    }

//    onStateChanged: {
//        if(root.state) {
//            image.source = onImage;
//        } else {
//            image.source = offImage;
//        }
//    }

    Image {
        id: image
        source: offImage
        rotation: root.imageRotation
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
    }

    MouseArea {
        id: mouseArea
        anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
        width: parent.width * Math.abs(basis.x === 0 ? 1 : 3)
        height: parent.height * Math.abs(basis.y === 0 ? 1 : 3)


        onPressed: {
            internal.startPos = Qt.vector2d(mouseX, mouseY);
        }

        onMouseYChanged: mousePosChanged();
        onMouseXChanged: mousePosChanged();
        function mousePosChanged() {
            var currentPos = Qt.vector2d(mouseX, mouseY);
            var vector1 = currentPos.minus(internal.startPos);

            if(vector1.length() > 5) {
                var dot = basis.dotProduct(vector1);
                if(dot > 0) {
                    modelData.requestValue = true
                    App.client.sCommunicator.requestValueChanged(modelData.requestValue, modelData)
                    image.source = onImage;
                } else {
                    modelData.requestValue = false
                    App.client.sCommunicator.requestValueChanged(modelData.requestValue, modelData)
                }
            }
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

    Component.onCompleted: {
        console.log("ONIMAGE: changed", onImage)
    }

    Connections {
        target: modelData
        function onValueChanged(value) {
            switch (value) {
            case true:
                image.source = onImage
                break
            case false:
                image.source = offImage
                break
            default:
                break
            }
        }
    }


}
