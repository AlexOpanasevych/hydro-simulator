pragma Singleton
import QtQuick 2.15
import "AwesomeMap.js" as Map

QtObject {
    property FontLoader icons: FontLoader {
        name: "Font Awesome 5 Pro Regular"
        source: "AwesomeRegular.ttf"
    }

    property FontLoader iconsPressed: FontLoader {
        name: "Font Awesome 5 Pro Solid"
        source: "AwesomeSolid.ttf"
    }

    readonly property var icon: Map.map
}
