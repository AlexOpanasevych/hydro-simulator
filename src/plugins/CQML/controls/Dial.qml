import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls 1.4
import QtQuick.Controls.Private 1.0

/*!
    \qmltype Dial
    \inqmlmodule CQML
    \since 1.0
    \brief A circular dial is rotated to set value
*/

Item {
    id: control
    width: diameter
    height: diameter
    /*!
        \qmlproperty real diameter

        The diameter set height and width
    */
    property real diameter: 200
    /*!
        \qmlproperty readonly real Dial::value

        Display value, in the range of
        \c 0.0 to \c 359.0.

        The default value is \c{0.0}
    */
    readonly property alias value: internal.value
    /*!
        \qmlproperty readonly real Dial::fullTurn

        Display value, in the range of
        \c -inf to \c +inf.
        If rotating left? the value would be negative (example: -2).

        The default value is \c{0.0}
    */
    readonly property alias fullTurn: internal.fullTurn
    /*!
        \signal pressed

        Emits when the dial is Dial::pressed(real x, real y).

        The {x,y} is the point of click.
    */
    signal pressed(real mouseX, real mouseY)
    /*!
        \signal released

        Emits when the dial is Dial::released(real x, real y).

        The {x,y} is the point of click.
    */
    signal released(real mouseX, real mouseY)

    /*!
        \qmlproperty Dial::helm

        Property, that holds the Helm component
    */
    property Component helm: Rectangle{
        width: 50
        height: 50
        radius: 25
        border.width: 1
        border.color: "black"
        color: "#2D4B58"
    }
    /*!
        \qmlproperty Dial::backgroundColor

        Property holds the background color
    */
    property color backgroundColor: "#0B2835"

    Rectangle{
        id: background
        anchors.fill: parent
        radius: diameter / 2
        color: control.backgroundColor
        border.width: 2
        border.color: "black"
        clip: true
        Rectangle{
            id: helmLine
            width: helmLoader.item.width
            height: parent.height / 2
            x: parent.width / 2 - helmLine.width / 2
            color: "transparent"
            transformOrigin: Item.Bottom
            rotation: control.value
            Loader{
                id: helmLoader
                sourceComponent: helm
                Binding{
                    target: helmLoader.item
                    property: "y"
                    value: background.border.width
                }
            }
        }

        MouseArea{
            anchors.fill: parent
            onPressed: { control.pressed(mouseX, mouseY) }
            onReleased: { control.released(mouseX, mouseY) }
            onMouseXChanged: { internal.updateValue(mouseX, mouseY) }
            onMouseYChanged: { internal.updateValue(mouseX, mouseY) }
        }
    }
    /*!
      internal data
      */
    QtObject{
        id: internal
        /*!
          \property type name: value
        */
        property real value: 0
        property int fullTurn: 0
        property real prevValue: 0

        property RangeModel range: RangeModel {
            minimumValue: 0.0
            maximumValue: 360.0
            stepSize: 0
            value: 0
        }

        function bound(val) { return Math.max(range.minimumValue, Math.min(range.maximumValue, val)); }

        function updateValue(mouseX, mouseY){
            var xx = -(mouseX - width * 0.5);
            var yy = -(height * 0.5 - mouseY);
            var angle = (xx || yy) ? Math.atan2(yy, xx) : 0;

            if (angle < Math.PI/ -2)
                angle = angle + Math.PI * 2;

            var r = range.maximumValue - range.minimumValue;
            var value;
            value = (range.minimumValue + r * (Math.PI * 3 / 2 - angle) / (2 * Math.PI));
            internal.prevValue = internal.value
            internal.value = bound(value)
            if (internal.prevValue < 5 && internal.value - internal.prevValue > 350){
                internal.fullTurn--;
            }else if (internal.prevValue > 355 && internal.prevValue - internal.value > 350){
                internal.fullTurn++;
            }
        }
    }

    /*!
      one way to set value from outside
      */
    function setValue(value){
        internal.value = value
    }
}
