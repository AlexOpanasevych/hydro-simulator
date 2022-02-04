import QtQuick 2.15

TextEdit {
    visible: false
    function copyClipboard() {
        paste()
        return text
    }
    function pasteClipboard(value){
        text = value
        selectAll()
        copy()
    }
}
