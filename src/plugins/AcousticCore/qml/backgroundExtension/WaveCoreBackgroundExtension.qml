import QtQuick 2.15
import QtGraphicalEffects 1.0
import AcousticCore 1.0
import QtQml 2.15

Item {
    Connections{
        target: window
        function onClosing(event){
            AcousticSimulationCore.threadStop()
            event.accepted = true
        }
    }

    WaveFieldItem{
        id: waveField
    }

    Repeater{
        model: AcousticSimulationCore.stationsCore.stationsModel
        delegate: Item{
            StationItemQSG{
                id: qsgStation
                startAngle: 0
                arcAngle: 10
                color: modelData.color
                xnCount: modelData.xnCount
                opacity: 0.5
                rotation: modelData.rotation + modelData.stationRotation
                x: modelData.x
                y: modelData.y

                Behavior on color { ColorAnimation { duration: 300 } }
            }

            Connections{
                target: AcousticSimulationCore.stationsCore
                function onCurrentItemChanged(item){
                    if (AcousticSimulationCore.stationsCore.currentItem === modelData){
                        timer.restart()
                    }else{
                        timer.stop()
                    }
                }
            }

            Timer{
                id: timer
                property int st: 0
                interval: 400
                running: false
                repeat: true
                onTriggered: {
                    st++;
                    if (st > 1) st = 0
                    switch(st){
                    case 0:
                        qsgStation.color = modelData.color
                        break
                    case 1:
                        qsgStation.color = "green"
                        break
                    }
                }

            }

            Binding{
                target: qsgStation
                property: "color"
                value: modelData.color
                when: !timer.running
                //@disable-check M16
                restoreMode : Binding.RestoreBinding
            }
        }
    }

    Connections{
        target: sceneRect
        function onSceneItemLoaded(sceneItem){
            AcousticSimulationCore.stationsCore.loadExtensionData(sceneItem)
        }
    }
}
