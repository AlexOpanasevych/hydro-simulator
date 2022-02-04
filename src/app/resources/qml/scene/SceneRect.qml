import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls 1.4 as Old
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.15

import KLib 1.0

Rectangle{
    id: sceneRect
    property point mousePos: Qt.point(0,0)
    property real prevX: 0
    property real prevY: 0
    property real scale: 1
    property alias img: image
    signal sceneItemLoaded(var sceneItem)

    onWidthChanged: { updateCornersPoints() }
    onHeightChanged: { updateCornersPoints() }

    function updateCornersPoints() { var xpos = sceneMouseArea.xPos; sceneMouseArea.xPos = 0; sceneMouseArea.xPos = xpos; }
    
    MouseArea{
        id: sceneMouseArea
        anchors.fill: parent
        hoverEnabled: true
        property bool draging: false
        property real xPos: -1
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onWheel: {
            var delta = wheel.angleDelta.y / 120.0
            sceneRect.mousePos = mapToItem(image, mouseX, mouseY)
            var zoomFactor = 0.8
            if (delta > 0)
            {
                zoomFactor = 1.0/zoomFactor;
            }
            var z = scaleTransform.xScale * zoomFactor;
            if (z < 3 && z > 0.01)
                sceneRect.scale = z
            var newCoords = mapFromItem(image, sceneRect.mousePos)
            var dx = mouseX - newCoords.x
            var dy = mouseY - newCoords.y
            translate.x += dx ; translate.y += dy
            updateCornersPoints()
        }
        
        onClicked: {
            scene.mousePos = mapToItem(image, mouseX, mouseY)
        }

        onDoubleClicked: {
            currentItem = scene
            menu.close()
        }

        onMouseXChanged: {
            if (draging)
            {
                xPos = mouseX
            }
            else if (sceneMouseArea.height * 0.97 < mouseY)
            {
                if (!playerItem.isOpened())
                {
                    playerItem.open()
                }
            }
        }
        
        onPressed: {
            if(mouse.button & Qt.RightButton) {
                sceneArea.workMode = "DEFAULT"
                menu.close()
                addItem.close()
                return
            }
            draging = true
            scene.prevX = mouseX
            scene.prevY = mouseY
        }
        
        onReleased: {
            draging = false
            let tmp = mapToItem(image, mouseX, mouseY)
            sceneRect.mousePos.x = tmp.x
            sceneRect.mousePos.y = -tmp.y
            if (workMode === "ADD" && !sceneArea.scene.piService.editing)
            {
                var itemType = sceneArea.scene.piService.currentChoosed.objectName
                sceneArea.createItem(sceneRect.mousePos, itemType)
            }
            sceneArea.scenePressed(sceneRect.mousePos)
        }
        
        onMouseYChanged: {
            if (draging)
            {
                var dx = scene.prevX - mouseX
                var dy = scene.prevY - mouseY
                movement.x -= dx
                movement.y -= dy
                scene.prevX = mouseX
                scene.prevY = mouseY
            }
        }
    }
    
    Item{
        id: image
        x: sceneRect.width / 2
        y: sceneRect.height / 2
        property int test: 1
        
        transform: [
            Scale{
                id: scaleTransform
                origin.x: 0
                origin.y: 0
                xScale: sceneRect.scale
                yScale: sceneRect.scale
            },
            Translate{
                id: translate
            },
            Translate{
                id: movement
                property real duration: 500
                Behavior on x { id: behX; NumberAnimation { duration :  movement.duration - 2; } enabled : false }
                Behavior on y { id: behY; NumberAnimation { duration :  movement.duration - 2; } enabled : false }
                property Timer timer: Timer{
                    interval: movement.duration
                    repeat: false
                    running: false
                    onTriggered: {
                        behX.enabled = false
                        behY.enabled = false
                        sceneMouseArea.xPos += 1
                        sceneMouseArea.xPos -= 1
                    }
                }
                function startAnimate()
                {
                    behX.enabled = true
                    behY.enabled = true
                    timer.restart()
                }
            }
        ]
        
        //background lines + text
        KGridBackground{
            x: -scene.sceneWidth/ 2
            y: -scene.sceneHeight / 2
            sceneWidth: scene.sceneWidth
            sceneHeight: scene.sceneHeight
            
            width: scene.sceneWidth
            height: scene.sceneHeight
        }

        //center red
        Rectangle{
            id: red
            color: "red"
            anchors.centerIn: parent
            width: 10
            height: 10
            radius: 5
        }

        //background extension
        Repeater{
            model: sceneArea.scene.getComponents("backgroundExtension")
            delegate: Loader{
                id: l
                sourceComponent: modelData
            }
        }

        //frame item
        Rectangle{
            id: highlightComponent
            color: "transparent"
            z: mainLoader.count + 1
            property var connectedItem: currentItem === sceneArea.scene ? image : domenMap.get(currentItem)

            visible: connectedItem !== image
            x: connectedItem.x - 5
            y: connectedItem.y - 5
            width:  connectedItem.width + 10
            height: connectedItem.height + 10
            scale: connectedItem.scale
            rotation: connectedItem.rotation
            Shape {
                ShapePath {
                    fillColor: "transparent"
                    strokeColor: "red"
                    strokeWidth: 3
                    startX: 0
                    startY: highlightComponent.height * 0.2
                    PathLine { x: 0; y: 0 }
                    PathLine { x: highlightComponent.width * 0.2; y: 0 }
                    
                    PathMove { x: highlightComponent.width * 0.8; y: 0 }
                    PathLine { x: highlightComponent.width; y: 0 }
                    PathLine { x: highlightComponent.width; y: highlightComponent.height * 0.2 }
                    
                    PathMove { x: highlightComponent.width; y: highlightComponent.height * 0.8 }
                    PathLine { x: highlightComponent.width; y: highlightComponent.height }
                    PathLine { x: highlightComponent.width * 0.8; y: highlightComponent.height }
                    
                    PathMove { x: highlightComponent.width * 0.2; y: highlightComponent.height }
                    PathLine { x: 0; y: highlightComponent.height }
                    PathLine { x: 0; y: highlightComponent.height * 0.8 }
                }
            }
        }

        //main loader
        Repeater{
            id: mainLoader
            model: scene.sceneItems
            delegate: Item{
                z: sceneArea.currentItem === modelData ? mainLoader.count : index
                Loader{
                    id: sceneItemsLoader
                    sourceComponent: scene.getComponent(modelData, "sceneLoader")
                    onLoaded: {
                        log(sceneItemsLoader.item)
                        sceneItemLoaded(modelData)
                    }
                }
            }
        }
    }

    Rectangle{
        id: leftTop
        anchors.left: parent.left
        anchors.top: parent.top
        visible: scene.devMode
        color: "black"
        width: 10
        height: 10
        radius: 5
        Text {
            anchors.rightMargin: -6
            anchors.fill: parent
            text: "" + leftTop.scenePoint
        }
        Connections{
            target: sceneMouseArea
            function onXPosChanged(){
                var coords = image.mapFromItem(leftTop, x + leftTop.width / 2, y + leftTop.height / 2)
                coords.y = -coords.y
                leftTop.scenePoint = coords
            }
        }
        property point scenePoint: image.mapFromItem(leftTop, x + leftTop.width / 2, y + leftTop.height / 2)
    }
    
    Rectangle{
        id: rightTop
        anchors.right: parent.right
        anchors.top: parent.top
        visible: scene.devMode
        color: "black"
        width: 10
        height: 10
        radius: 5
        Text {
            anchors.leftMargin: -contentWidth
            anchors.fill: parent
            text: "" + rightTop.scenePoint
        }
        Connections{
            target: sceneMouseArea
            function onXPosChanged(){
                var coords = image.mapFromItem(rightTop, x + rightTop.width / 2, y + rightTop.height / 2)
                coords.y = -coords.y
                rightTop.scenePoint = coords
            }
        }
        property point scenePoint: image.mapFromItem(rightTop, x + rightTop.width / 2, y + rightTop.height / 2)
    }
    
    Rectangle{
        id: rightBottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        visible: scene.devMode
        color: "black"
        width: 10
        height: 10
        radius: 5
        Text {
            anchors.topMargin: -10
            anchors.leftMargin: -contentWidth
            anchors.fill: parent
            text: "" + rightBottom.scenePoint
        }
        Connections{
            target: sceneMouseArea
            function onXPosChanged(){
                var coords = image.mapFromItem(rightBottom, x + rightBottom.width / 2, y + rightBottom.height / 2)
                coords.y = -coords.y
                rightBottom.scenePoint = coords
            }
        }
        property point scenePoint: image.mapFromItem(rightBottom, x + rightBottom.width / 2, y + rightBottom.height / 2)
    }
    
    Rectangle{
        id: leftBottom
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        visible: scene.devMode
        color: "black"
        width: 10
        height: 10
        radius: 5
        Text {
            anchors.topMargin: -10
            anchors.fill: parent
            text: "" + leftBottom.scenePoint
        }
        Connections{
            target: sceneMouseArea
            function onXPosChanged(){
                var coords = image.mapFromItem(leftBottom, x + leftBottom.width / 2, y + leftBottom.height / 2)
                coords.y = -coords.y
                leftBottom.scenePoint = coords
            }
        }
        property point scenePoint: image.mapFromItem(leftBottom, x + leftBottom.width / 2, y + leftBottom.height / 2)
    }
    
    Rectangle{
        id: centerPoint
        anchors.centerIn: parent
        visible: scene.devMode
        color: "black"
        width: 10
        height: 10
        radius: 5
        Text {
            anchors.fill: parent
            text: "" + centerPoint.scenePoint
        }
        Connections{
            target: sceneMouseArea
            function onXPosChanged(){
                centerPoint.scenePoint = image.mapFromItem(centerPoint, x + centerPoint.width / 2, y + centerPoint.height / 2 )
            }
        }
        
        property point scenePoint: image.mapFromItem(centerPoint, x + centerPoint.width / 2, y + centerPoint.height / 2 )
    }

    Connections{
        target: sceneArea
        function onMoveToCenter(){
            let p = image.mapToItem(sceneRect, 0, 0)
            let xi = p.x, yi = p.y;
            let center = image.mapToItem(sceneRect, centerPoint.scenePoint.x, centerPoint.scenePoint.y )
            let xoffset = xi - center.x
            let yoffset = yi - center.y
            movement.startAnimate()
            movement.x -= xoffset
            movement.y -= yoffset
        }
    }

    function moveToItem(item)
    {
        let p = image.mapToItem(sceneRect, item.x, -item.y)
        let xi = p.x, yi = p.y;
        let tl = image.mapToItem(sceneRect, leftTop.scenePoint.x, leftTop.scenePoint.y)
        let br = image.mapToItem(sceneRect, rightBottom.scenePoint.x, rightBottom.scenePoint.y)
        if ( !(tl.x <= xi && br.x >= xi) || !(tl.y >= yi && br.y <= yi) )
        {
            let center = image.mapToItem(sceneRect, centerPoint.scenePoint.x, centerPoint.scenePoint.y )
            let xoffset = xi - center.x
            let yoffset = yi - center.y
            movement.startAnimate()
            movement.x -= xoffset
            movement.y -= yoffset
        }
    }
}
