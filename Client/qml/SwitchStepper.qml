import QtQuick 2.0
import KLib 1.0

QtObject {
    id: root
    property var angles

    property int outputAngle : angles[0]
    property int outputValue
    property int inputAngle

    onInputAngleChanged: {
        chooseAngle();
    }

    function chooseAngle() {
        for(let i = 0; i < angles.length; i++) {
            if(KMath.degreesDistance(angles[i], inputAngle) < 4) {
                outputAngle = angles[i]
                outputValue = i + 1
            }
        }
    }


}
