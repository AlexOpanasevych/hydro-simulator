import QtQuick 2.0
import KLib 1.0

Item {
    id: root

    x : modelData.x
    y : modelData.y
    height : modelData.height
    width : modelData.width

    property double minAngle: modelData.additionalQml.minAngle !== undefined ? modelData.additionalQml.minAngle : 90
    property double value: 0
    property string divImage: "file:/" + App.resPath + "/" + modelData.additionalQml.divImage
    property string handleImage: "file:/" + App.resPath + "/" + modelData.additionalQml.handleImage
    property string capImage: "file:/" + App.resPath + "/" + modelData.additionalQml.capImage

    property var modelData;

    Image {
        id: divisionImage
        source: divImage
        anchors.verticalCenter: cap.verticalCenter
        anchors.horizontalCenter: cap.horizontalCenter
    }

    Image {
        id: handle
        y: 30
        x: 70
        source: handleImage
        rotation: 2 * value * minAngle - minAngle
     }

     Image {
         id: cap
         anchors.fill: root
         source: capImage
     }

     Connections {
        target: !modelData ? null : modelData
        function onValueChanged(value){
            console.log("CirleIndicator: value changed", value.value)
            root.value = value.value
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
        console.log(minAngle, maxAngle)
     }
}
