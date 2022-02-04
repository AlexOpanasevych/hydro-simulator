import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.12

Rectangle{
    id: root
    anchors.fill: parent
    color: "transparent"
    property variant angles: []
    property string textFont: '14px serif oblique'
    property bool isAngle: false
    property bool enableText: true
    property variant text: []
    property double imageRadius: root.width / 2.5


Canvas {
    anchors.fill: parent

    function angleToRadian(angle)
    {
        return (angle / 180 * Math.PI)
    }

    function transformY(radius, angle)
    {
        return radius * Math.sin(angle)
    }

    function transformX(radius, angle)
    {
        return radius * Math.cos(angle)
    }

    function findInVariant(variant, item)
    {
        for (var i = 0; i < variant.length; i++)
        {
            if (variant[i] === item)
            {
                return true
            }
        }
        return false
    }

    function drawTextAlongArc(context, centerX, centerY, radius)
    {
        context.save();
        context.translate(centerX, centerY);
        var count;
        var longLine = false;
        for (var i = 0; i < root.angles.length; i++)
        {
            context.save();
            context.rotate(angleToRadian(root.angles[i]));
            context.translate(0, -1 * radius);
            if (root.enableText)
            {

                var char1;
                if (root.isAngle && root.text.length > 0)
                {
                    if (findInVariant(root.text, root.angles[i]))
                    {
                        char1 = root.angles[i]
                        longLine = true
                        context.translate(0, radius / 3);
                        context.rotate(-angleToRadian(root.angles[i]));
                        context.fillText(char1, 0, 5);
                        context.rotate(angleToRadian(root.angles[i]));
                        context.translate(0, -1 * radius);
                    }
                    else if (root.angles[i] % 10 === 0)
                    {
                        longLine = true
                    }
                }
                else if (root.isAngle)
                {
                    char1 = root.angles[i]
                    context.fillText(char1, 0, 0);
                }
                else
                {
                    char1 = (i + 1)
                    context.fillText(char1, 0, 0);
                }

            }
            context.restore();
            var x1, x2, y1, y2;
            if (longLine)
            {
                x2 = transformY(radius - 23, angleToRadian(root.angles[i]))
                y2 = -transformX(radius - 23, angleToRadian(root.angles[i]))
                longLine = false
            }
            else
            {
                x2 = transformY(radius - 15, angleToRadian(root.angles[i]))
                y2 = -transformX(radius - 15, angleToRadian(root.angles[i]))
            }
            x1 = transformY(radius - 8, angleToRadian(root.angles[i]))
            y1 = -transformX(radius - 8, angleToRadian(root.angles[i]))



            context.beginPath();
            context.lineWidth = 2;
            context.moveTo(x1, y1);
            context.strokeStyle = "black"
            context.lineTo(x2, y2);
            context.stroke();


        }

        context.beginPath();
        context.fillStyle = "transparent"
        context.strokeStyle = "black"
        context.arc(0, 0, radius - 15, angleToRadian(270 + root.angles[root.angles.length - 1]),
                    angleToRadian(270 + root.angles[0]), true)
        context.fill();
        context.stroke();
        context.restore();
    }



  onPaint: {

      var ctx = getContext("2d");
      ctx.fillStyle = "transparent"

      ctx.font = root.textFont
      ctx.textAlign = "center";

      var centerX = width / 2;
      var centerY = height / 2;
      var radius  = root.imageRadius

      ctx.fillStyle= "#333333"
      drawTextAlongArc(ctx, centerX, centerY, radius);

  }
}

}
