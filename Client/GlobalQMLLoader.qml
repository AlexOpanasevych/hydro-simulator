import QtQuick 2.15
import QtQuick.Controls 2.0
import KLib 1.0

Item {
    id: root
    property int prevWidth : 1920
    property int prevHeight: 1080
    property real scaleWidth : 1.0;
    property real scaleHeight : 1.0;
    property alias globalRep: globalRep


    ComboBox {
        model: 7


    }


    FlexibleProxyModel {
        id: filter1
        source: App.client.itemsModel
        filter: Filter {
            propertyName: "clientName"
            value: "platina1"
        }
    }

    FlexibleProxyModel {
        id: filter2
        source: App.client.itemsModel
        filter: Filter {
            propertyName: "clientName"
            value: "platina2"
        }
    }
    FlexibleProxyModel {
        id: filter3
        source: App.client.itemsModel
        filter: Filter {
            propertyName: "clientName"
            value: "platina3"
        }
    }

    FlexibleProxyModel {
        id: filter4
        source: App.client.itemsModel
        filter: Filter {
            propertyName: "clientName"
            value: "platina4"
        }
    }

    FlexibleProxyModel {
        id: filter5
        source: App.client.itemsModel
        filter: Filter {
            propertyName: "clientName"
            value: "platina5"
        }
    }

    FlexibleProxyModel {
        id: filter6
        source: App.client.itemsModel
        filter: Filter {
            propertyName: "clientName"
            value: "bronze1"
        }
    }

    FlexibleProxyModel {
        id: filter7
        source: App.client.itemsModel
        filter: Filter {
            propertyName: "clientName"
            value: "bronze2"
        }
    }

    Repeater {
        id: globalRep
        model: 7
        delegate: QMLLoader {

            qmlRep.model: {
                switch(index) {
                case 0:
                    return filter4
                case 1:
                    return filter1
                case 2:
                    return filter2
                case 3:
                    return filter3
                case 4:
                    return filter5
                case 5:
                    return filter6
                case 6:
                    return filter7
                }
            }

            qmlRep.onItemAdded: {
                item.transformOrigin = Item.TopLeft
                item.scale = 0.25
                item.x /= 4
                item.y /= 2
            }

            backgroundLoader.onLoaded: {
                backgroundLoader.item.transformOrigin = Item.TopLeft
                backgroundLoader.item.scale = 0.25
            }
        }

        onItemAdded: {
            parent.onItemAdded(index, item)
        }

    }

    function onItemAdded(index, item) {
        switch(index) {
        case 0:
            item.x = 0
            item.y = 0
            console.log("platina4")
            break
        case 1:
            item.x = 0
            item.y = root.prevHeight / 4
            console.log("platina1")
            break

        case 2:
            item.x = root.prevWidth / 4
            item.y = root.prevHeight / 4
            console.log("platina2")
            break
        case 3:
            item.x = root.prevWidth / 2
            item.y = root.prevHeight / 4
            console.log("platina3")
            break
        case 4:
            item.x = root.prevWidth / 2
            item.y = 0
            console.log("platina5")
            break
        case 5:
            item.x = root.prevWidth * 0.75
            item.y = 0
            console.log("bronze1")
            break
        case 6:
            item.x = root.prevWidth * 0.75
            item.y = root.prevHeight / 4
            console.log("bronze2")
            break
        default:
            item.x = -999999
            item.y = -999999
            console.log("client not exist")
        }
    }

    function onDigitPressed(index) {
        for(var i = 0; i < globalQmlLoader.globalRep.count; i++) {
            if(i !== index) {
                globalQmlLoader.globalRep.itemAt(i).enabled = false
                globalQmlLoader.globalRep.itemAt(i).visible = false
            }else {
                globalQmlLoader.globalRep.itemAt(i).scale = 4
                globalQmlLoader.globalRep.itemAt(i).enabled = true
                globalQmlLoader.globalRep.itemAt(i).visible = true
                globalQmlLoader.globalRep.itemAt(i).x=0
                globalQmlLoader.globalRep.itemAt(i).y=0
            }
        }
    }
}
