import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.0
import Trajectory 1.0
import PluginsBase 1.0
import CQML 1.0 as C

Item{
    id: root
    //for sceneloader only
    property bool moving: false

    ExtraInfoPanel {
        scale: itemsLoader.item.scale
        visible: sceneArea.extraInfo
        x: itemsLoader.item.x - 5
        y: itemsLoader.item.y - 5
        width:  itemsLoader.item.width + 10
        height: itemsLoader.item.height + 10
        z: root.z
    }

    Loader{
        id: itemsLoader
        z: 1
        sourceComponent: sceneArea.scene.getComponent(modelData, "item")
        onLoaded: { sceneArea.domenMap.insert(modelData, itemsLoader.item) }

        Connections{
            target: itemsLoader.item
            function onPressed()
            {
                modelData.pressed()
                if (sceneArea.workMode === "REMOVE"){
                    sceneArea.currentItem = sceneArea.scene
                    sceneArea.domenMap.remove(modelData)
                    sceneArea.domenMap.remove(modelData)
                    sceneArea.scene.removeItem(modelData)
                }
                else if (sceneArea.workMode === "ADD")
                {
                    sceneArea.scene.piService.currentChoosed.pressed()
                    addItem.close()
                    sceneArea.currentItem = modelData
                    menu.open("Edit")
                }
                else if (sceneArea.workMode === "DEFAULT" || sceneArea.workMode === "MOVE")
                {
                    menu.open("Edit")
                }
                sceneArea.currentItem = modelData

                if (trajectoryLoader.item && trajectoryLoader.item.sceneItem && sceneArea.scene.scenePlayer.mode === 2) trajectoryLoader.item.sceneItem.trajectory.selectTrajectory(0)


                sceneArea.scene.activateFilter(modelData.type)
            }

            function onReleased(){
                if (sceneArea.scene.scenePlayer.mode === 2 /*KScenePlayer.STOP*/ && sceneArea.workMode === "MOVE") // KSceneArea.MOVE
                {
                    if (modelData.trajectory)
                        modelData.trajectory.move(Qt.point(modelData.x, -modelData.y))
                }
                if (modelData.trajectory) modelData.trajectory.opacity = 1
            }

            function onXChanged(x){
                if (modelData.trajectory && sceneArea.scene.scenePlayer.mode === 2) modelData.trajectory.opacity = 0
            }

            function onYChanged(y){
                if (modelData.trajectory && sceneArea.scene.scenePlayer.mode === 2) modelData.trajectory.opacity = 0
            }
        }

        Binding{
            target: itemsLoader.item
            property: "scale"
            value: (1/scaleTransform.xScale)
        }

        Binding{
            target: modelData
            property: "interactive"
            value: (sceneArea.workMode === "MOVE") //KSceneArea.MOVE
        }

    }
    //trajectories
    Loader{
        id: trajectoryLoader
        active: sceneItem.trajectory !== null ? true : false
        property SceneItem sceneItem: modelData
        sourceComponent: trajectory
    }

    //trace
    TraceItem{
        id: traceItem
        property SceneItem sceneItem: modelData
        bufferLength: !sceneItem ? 0 : sceneItem.bufferLength

        Connections{
            target: modelData.trace
            function onCoordsUpdated(coords)
            {
                var c = Qt.point(coords.x, coords.y)
                traceItem.updateCoords(c)
            }

        }
    }
    //segments points
    Repeater{
        model: modelData.trace.segments
        delegate: Rectangle{
            width: 10
            height: 10
            radius: 5
            x: modelData.emittingPoint.x - width / 2
            y: modelData.emittingPoint.y - height / 2
            color: "green"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    modelData.press()
                }
            }
        }
    }

    Repeater{
        model: modelData.trace.segments
        delegate: SegmentItem{
            points: modelData.points
            color: modelData.checked ? "green" : "black"
            lineWidth: modelData.checked ? 3 : 1
        }
    }

    Component{
        id: trajectory
        TrajectoryItem{
            trajectory: sceneItem.trajectory

            opacity: sceneItem.trajectory.opacity

            Behavior on opacity { NumberAnimation { duration: 50 } }

            Connections{
                target: sceneArea.scene.scenePlayer
                function onStartSignal()
                {
                    //console.log("startSignal")
                    sceneItem.trajectory.selectTrajectory(-1)
                }
            }

            Connections{
                target: sceneArea
                function onCurrentItemChanged()
                {
                    //console.log("currentChanged")
                    sceneItem.trajectory.selectTrajectory(-1)
                }
            }
        }
    }
}
