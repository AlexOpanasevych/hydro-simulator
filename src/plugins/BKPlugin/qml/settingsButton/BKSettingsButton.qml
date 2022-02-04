import QtQuick 2.0
import CQML 1.0 as C
import KLib 1.0

C.Button{
    id: createTrace
    text: "Open BK settings"
    buttonRadius: 3
    textColor:  "#e94040"
    buttonBackground:  "#6ab2f2"

    onPressed: {
        BK.openWindow()
    }

    Connections {
        target: BK
        function onSetEnginePause() {
            sceneArea.scene.scenePlayer.pause()
        }

        function onSetEngineStart() {
            sceneArea.scene.scenePlayer.start()
        }
    }

//    Connections {
//        target: sceneArea.scene.scenePlayer
//        function onModeChanged(mode) {
//            if(mode === KScenePlayer.STOP) {
//                BK.reset()
//            }
//        }
//    }
}
