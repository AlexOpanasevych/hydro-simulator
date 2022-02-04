import QtQuick 2.15

Item {
    id: root

    property point p1: Qt.point(0,0)
    property point p2: Qt.point(0,0)

    Text {
        x: 200
        y: 200
        id: name
        text: "Rotation: " + rotationAngle(p1,p2) + " Distance: " + distance(p1,p2) + " Middle: " + interpolate(p1,p2, (p1.x - p2.x) / 2 )
    }

    TraceTriangle{
        id: triangle
        width: 40
        height: 40
        rotation: rotationAngle(p1, p2) - 90
        x: parent.width - 60
        y: mapFromItem(image, rightBottom.scenePoint.x, interpolate(p2, p1, rightBottom.scenePoint.x)).y
    }

    function rotationAngle(point1, point2)
    {
        var angle = Math.atan((point1.y - point2.y) / (point1.x - point2.x)) * (180 / Math.PI) + 90
        if (point1.x < point2.x && point1.y > point2.y) {}
        else if (point2.x <= point1.x  || point2.y <= point1.y) angle += 180
        return angle.toFixed(2)
    }

    function distance(point1, point2)
    {
        return Math.sqrt(Math.pow(point1.x - point2.x, 2) + Math.pow(point1.y - point2.y, 2))
    }

    function interpolate(point1, point2, x, extrapolate = false)
    {
        //console.log("interpolate: ", point1, point2, x)

        let xL = point1.x, yL = point1.y, xR = point2.x, yR = point2.y;
        if ( !extrapolate )
        {
            if ( x < xL ) yR = yL;
            if ( x > xR ) yL = yR;
        }

        let dydx = ( yR - yL ) / ( xR - xL );
        var res = yL + dydx * ( x - xL );
        //console.log(res)
        return res
    }
}
