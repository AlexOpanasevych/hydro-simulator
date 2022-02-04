import QtQuick 2.15
import PhysicalItems 1.0

Item{
    //items
    Loader{
        id: itemsLoader
        z: 1
        sourceComponent: sceneArea.scene.getComponent(modelData, "item")
        onLoaded: { sceneArea.domenMap.insert(modelData, itemsLoader.item) }

        Connections{
            target: itemsLoader.item
            function onPressed()
            {
                if (sceneArea.workMode === 2) //KSceneArea.REMOVE
                {
                    sceneArea.currentItem = sceneArea.scene
                    sceneArea.domenMap.remove(modelData)
                    sceneArea.domenMap.remove(modelData)
                    sceneArea.scene.removeItem(modelData)

                }
                else if (sceneArea.workMode === 3) //KSceneArea.ADD
                {
                    sceneArea.scene.piService.currentChoosed.pressed()
                    addItem.close()
                    //sceneArea.workMode = 0 //KSceneArea.DEFAULT
                    sceneArea.currentItem = modelData
                }
                else
                {
                    sceneArea.currentItem = modelData
                }

                menu.open(2)

                sceneArea.scene.activateFilter(modelData.type)
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
            value: (sceneArea.workMode === 1) //KSceneArea.MOVE
        }

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
}
