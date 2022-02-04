//! [1] => first handle
//! [2] => secondHandle
//! [3] => progressbar

import QtQuick 2.15

/*!
  *  \qmltype CircularRangeSlider
  */
Item {
    id: root

    width: radius * 2
    height: radius * 2
    /*!
      * \qmlproperty int radius
      * Radius of circle
      */
    property int radius: 150
    /*!
      * \qmlproperty real firstValue
      * Display angle of first handle (same as "from")
      */
    readonly property alias firstValue: firstDragObject.angle
    /*!
      * \qmlproperty real secondValue
      * Display angle of second handle (same as "to")
      */
    readonly property alias secondValue: secondDragObject.angle
    /*!
      * \qmlproperty color progressColor
      * Main color of CircularRangeSlider
      */
    property color progressColor: "#29b6f6"
    /*!
      * \qmlproperty color progressColor
      * Second color of CircularRangeSlider
      */
    property color progressBackgroundColor: "#e0e0e0"
    /*!
      * \qmlproperty bool readonly
      * Disable all gui interractive things
      */
    property bool readonly: true
    property real firstStartValue
    property real secondStartValue
    /*!
     * \qmlproperty Component firstHandle
     *  Contains component's visual implementation
     */
    property Component firstHandle: Rectangle {
        width: 30
        height: 30
        radius: 15
        color: progressColor
    }

    property Component secondHandle: Rectangle {
        width: 30
        height: 30
        radius: 15
        color: progressColor
    }

    signal pressed()

    Component.onCompleted: {
        setFirstValue(firstStartValue ? firstStartValue : 0)
        setSecondValue(secondStartValue ? secondStartValue : 60)
    }

    Rectangle {
        id: circle
        anchors.centerIn: parent
        width: 2 * radius
        height: 2 * radius
        radius: root.radius

        color: 'transparent'

        Canvas {
            id: radialCanvas
            anchors.fill: parent

            Connections{
                target: root
                function onFirstValueChanged(){
                    radialCanvas.requestPaint()
                }

                function onSecondValueChanged(){
                    radialCanvas.requestPaint()
                }

                function onProgressColorChanged(){
                    radialCanvas.requestPaint()
                }

            }

            onPaint: {
                var ctx = getContext("2d");
                ctx.reset();

                var centreX = width / 2;
                var centreY = height / 2;
                var radius=width/2;

                // draw a radial gradient color
                var gradient=ctx.createRadialGradient(centreX,centreY,0,centreX,centreY,radius);
                gradient.addColorStop(0, root.progressColor);
                gradient.addColorStop(1, 'transparent');
                ctx.fillStyle=gradient;
                ctx.lineTo(firstDragObject.x, firstDragObject.y)
                ctx.arc(centreX, centreY, width /2 - 5, (root.firstValue - 90) * Math.PI / 180, (root.secondValue - 90) * Math.PI / 180, false);
                ctx.lineTo(root.radius, root.radius)
                ctx.fill();
            }
        }

        Rectangle{
            width: 2
            height: root.radius - 3
            x: root.radius
            y: 3
            transformOrigin: Item.Bottom
            rotation: root.firstValue
            color: root.progressColor
            antialiasing: true
        }

        Rectangle{
            width: 2
            height: root.radius - 3
            x: root.radius
            y: 3
            transformOrigin: Item.Bottom
            rotation: root.secondValue
            color: root.progressColor
            antialiasing: true
        }


        //! [1]
        Item {
            id: firstDragObject
            readonly property real dragRadius: Math.sqrt(Math.pow(x - root.radius, 2) + Math.pow(y - root.radius, 2))
            property real angle: 0
            onXChanged: angle = circle.updateValue(x, y)
            onYChanged: angle = circle.updateValue(x, y)
        }
        //! [1]

        //! [2]
        Item {
            id: secondDragObject
            readonly property real dragRadius: Math.sqrt(Math.pow(x - root.radius, 2) + Math.pow(y - root.radius, 2))
            property real angle: 0
            onXChanged: angle = circle.updateValue(x, y)
            onYChanged: angle = circle.updateValue(x, y)
        }
        //! [2]

        function bound(val) { return Math.max(0, Math.min(360, val)); }

        function updateValue(mouseX, mouseY){
            var xx = -(mouseX - width * 0.5);
            var yy = -(height * 0.5 - mouseY);
            var angle = (xx || yy) ? Math.atan2(yy, xx) : 0;

            if (angle < Math.PI/ -2)
                angle = angle + Math.PI * 2;

            var value;
            value = (360 * (Math.PI * 3 / 2 - angle) / (2 * Math.PI));
            return bound(value)
        }
    }

    function setFirstValue(angle){
        angle += 90
        firstDragObject.x = root.radius - root.radius * Math.cos(angle * Math.PI / 180)
        firstDragObject.y = root.radius - root.radius * Math.sin(angle * Math.PI / 180)
    }

    function setSecondValue(angle){
        angle += 90
        secondDragObject.x = root.radius - root.radius * Math.cos(angle * Math.PI / 180)
        secondDragObject.y = root.radius - root.radius * Math.sin(angle * Math.PI / 180)
    }
}
