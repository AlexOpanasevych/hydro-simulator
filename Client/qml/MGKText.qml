import QtQuick 2.0

Text {
    property var modelData;

    x: modelData.x
    y: modelData.y
    height: modelData.height
    width: modelData.width
    text: modelData.caption
    font.pointSize: modelData !== undefined && modelData.pointSize !== undefined ? modelData.pointSize : 12

}
