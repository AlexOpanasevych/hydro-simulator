import QtQuick 2.0

Item{
    id: root
    property variant boxesText: []
    property int mode: 0
    property double offset: 5

    /*      mode
     *  0 - km/h
     *  1 - m/s
     *  2 - knots
    */
    Repeater{
        id: repeater
        model: boxesText.length
        delegate: Item{
            CustomCheckBox{
                id: checkBox
                text: root.boxesText[index]
                y: index * (height + offset)
                mode: root.mode
                idx: index
                onClicked: { root.mode = index }
            }
        }
    }
}
