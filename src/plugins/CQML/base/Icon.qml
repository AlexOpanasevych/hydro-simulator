import QtQuick 2.15
//import InternalInterface 1.0
import CQML 1.0
Item {
    property alias text: t.testText
    signal wow(string text)
    TestItem{
        id: t
        Component.onCompleted: wow(t.testText)
    }
}
