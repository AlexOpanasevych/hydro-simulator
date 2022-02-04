import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQml 2.15
//! [fix for easy deploy]
import QtQuick.Dialogs 1.2
import Qt.labs.platform 1.1
import Qt.labs.settings 1.1
//! [fix for easy deploy]
import "scene"
import KLib 1.0
import CQML 1.0 as C

Window {
    id: window
    visible: true
    width: 1920
    height: 1080
    property var componentsList: []

    Connections{
        target: window
        function onClosing(event){
            PluginsManager.save()
            event.accepted = true
        }
    }

    LoggingCategory {
        id: category
        name: "sasha"
    }

    function log(msg){
        console.log(category, msg)
    }

    KSceneViewer{
        anchors.fill: parent
        sceneID: "first"
    }


}
