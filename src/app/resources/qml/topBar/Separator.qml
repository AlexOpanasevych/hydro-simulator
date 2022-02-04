import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import KLib 1.0
import CQML 1.0

Rectangle{
    property bool hideable: true
    objectName: "separator"
    Layout.fillHeight: true
    Layout.preferredWidth: 1
    border.width: 1
    border.color: "black"
    color: "transparent"
}
