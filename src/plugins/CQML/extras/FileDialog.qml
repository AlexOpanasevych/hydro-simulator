import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3 as Dialogs
import "../base"
import "../controls"
import "../shaders"

BaseBackground {
    id: root
    //clip: true
    implicitHeight: 20
    implicitWidth: 50

    property alias defaultSuffix: fileDialog.defaultSuffix
    property alias fileUrl: fileDialog.fileUrl
    property alias fileUrls: fileDialog.fileUrls
    property alias folder: fileDialog.folder
    property alias modality: fileDialog.modality
    property alias nameFilters: fileDialog.nameFilters
    property alias selectExisting: fileDialog.selectExisting
    property alias selectFolder: fileDialog.selectFolder
    property alias selectMultiple: fileDialog.selectMultiple
    property alias selectedNameFilter: fileDialog.selectedNameFilter
    property alias shortcuts: fileDialog.shortcuts
    property alias sidebarVisible: fileDialog.sidebarVisible
    property alias title: fileDialog.title

    signal accepted()
    signal rejected()
    signal pressed(point pos)
    signal released(point pos)
    signal copyPressed()
    function close() { fileDialog.close() }
    function open() { fileDialog.open() }
    function reject() { fileDialog.reject() }
    function accept() { fileDialog.accept() }
    function done(result) { fileDialog.done(result) }
    //property alias visible: fileDialog.visible

    property string path: ""
    property string pathPlaceholder: ""
    property real backgroundRadius: 7
    property string pathTitle: ""
    property real titleWidth: width * 0.2
    readonly property int pixelSize: height - 9
    property color backgroundColor: "#2c3847"
    property color textColor: "#6396cb"
    property color lineColor: "#6396cb"
    property color pathPlaceholderColor: "#6c7883"
    property color pathBackgroundColor: "transparent"
    property color copyColor: textColor
    property color copyBackgroundColor: backgroundColor
    elevation: 1

    //@disable-check M129
    Dialogs.FileDialog{
        id: fileDialog
        onAccepted: { root.accepted(); path = "" + fileDialog.fileUrls }
        onRejected: root.rejected()
        visible: false
    }

    Clipboard{ id: clipboard }

    Rectangle{
        anchors.fill: parent
        color: root.backgroundColor
        radius: backgroundRadius

        WavedMouseArea{
            id: wavedMouseArea
            anchors.fill: parent
            circular: root.radius > 0
            enabled: root.enabled

            //anchors.rightMargin: copyButton.width
            //hoverEnabled: true
            z: 0

            onPressed: {
                fileDialog.open()
                root.pressed(Qt.point(mouseX, mouseY))
            }

            onReleased: {
                root.released(Qt.point(mouseX, mouseY))
            }
        }

        RowLayout{
            id: rowLayout
            anchors.fill: parent
            spacing: 0
            z: 2

            Item{
                id: body
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true

                Label{
                    id: displayedText
                    anchors.fill: parent
                    text: path
                    font.pixelSize: root.pixelSize
                    color: textColor
                    elide: Text.ElideLeft
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    anchors.leftMargin: 3
                    anchors.rightMargin: 3
                    background: Rectangle{
                        anchors.fill: parent
                        color: root.pathBackgroundColor
                    }

                    //visible: false
                }

                Label{
                    id: placeholderText
                    text: pathPlaceholder
                    visible: displayedText.text === ""
                    anchors.fill: displayedText
                    font.pixelSize: root.pixelSize
                    color: root.pathPlaceholderColor
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }

                BaseBackground{
                    anchors.fill: parent
                    anchors.topMargin: parent.height - 3
                    anchors.leftMargin: backgroundRadius
                    radius: width / 2
                    backgroundColor: lineColor
                    z: 0
                    elevation: 0

                    tintColor: "transparent"
                }

                ShaderEffectSource {
                    id: sh
                    anchors.fill: displayedText
                    sourceItem: displayedText
                    visible: false
                }

                ShaderEffect {
                    id: copyEffect
                    anchors.fill: displayedText
                    //onHeightChanged: console.log(height)
                    //! [properties]
                    property variant source: sh
                    property real bend: 0
                    property real minimize: 0
                    property real side: 0.5
                    Behavior on bend {
                        NumberAnimation { from: 0; duration: 300; easing.type: Easing.InOutSine }
                    }
                    Behavior on minimize {
                        NumberAnimation { from: 0; duration: 300; easing.type: Easing.InOutSine }
                    }
                    //! [properties]
                    //! [vertex]
                    mesh: Qt.size(10, 10)
                    vertexShader: "
                                attribute highp vec4 qt_Vertex;
                                attribute highp vec2 qt_MultiTexCoord0;

                                uniform highp mat4 qt_Matrix;
                                uniform highp float bend;
                                uniform highp float minimize;
                                uniform highp float side;
                                uniform highp float width;
                                uniform highp float height;

                                varying highp vec2 qt_TexCoord0;

                                void main() {
                                    qt_TexCoord0 = qt_MultiTexCoord0;
                                    highp vec4 pos = qt_Vertex;
                                    pos.x = mix(qt_Vertex.x, width, minimize);
                                    highp float t = pos.x / width;
                                    t = (3. - 2. * t) * t * t;
                                    pos.y = mix(qt_Vertex.y, side * height, t * bend);
                                    gl_Position = qt_Matrix * pos;
                                }"
                }
            }

            IconButton{
                id: copyButton
                Layout.fillHeight: true
                Layout.preferredWidth: height
                iconCode: IconHelper.icon["copy"]
                font.family: IconHelper.icons.name
                buttonRadius: width / 2
                buttonBackground: root.copyBackgroundColor
                textColor: root.copyColor
                description: ""

                z: 4
                onPressed: {
                    root.copyPressed()
                    copyEffect.bend = 0
                    copyEffect.minimize = 0

                    copyEffect.bend = 1
                    copyEffect.minimize = 1

                    clipboard.pasteClipboard(path)
                }
            }
        }
    }
}
