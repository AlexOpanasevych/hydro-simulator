import QtQuick 2.15

Item {
    id: root
    property string fillColor: "green"
    property string borderColor: "black"

    Rectangle{
        anchors.fill: parent
        color: "transparent"
        border.color: "black"
    }

    Canvas {
        id: canvas
        anchors.fill: parent

        property real radius: width > height ? height * 0.55 : width * 0.55
        readonly property real xCenter: width / 2
        readonly property real yCenter: height / 2
        readonly property real sqrt3: Math.sqrt(3)
        property var context
        smooth: true
        onVisibleChanged: if (available) context = getContext('2d')
        onAvailableChanged: if (available) context = getContext('2d')

        onPaint: {
            if (!context) return;
            context.reset();
            context.clearRect(0, 0, radius * 2, radius * 2);
            context.imageSmoothingEnabled = true;
            context.translate(xCenter - xCenter * 0.2, yCenter);
            context.strokeStyle = borderColor;
            context.fillStyle = fillColor;

            context.beginPath();
            context.moveTo(radius, 0);
            context.lineTo(-radius * 0.5, -radius * sqrt3 * 0.5);
            context.lineTo(-radius * 0.5,  radius * sqrt3 * 0.5);
            context.lineTo(radius, 0);
            context.closePath();

            context.fill();
            context.stroke();
        }
    }
}
