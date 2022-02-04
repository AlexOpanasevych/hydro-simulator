#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = CQML
QT += qml
QT += quick
CONFIG += qt
CONFIG += plugin
CONFIG += c++17
CONFIG += qmltypes

QML_IMPORT_NAME = CQML
QML_IMPORT_MAJOR_VERSION = 1.0
DESTDIR = $$PWD/../../../bin/plugins/$$QML_IMPORT_NAME
message($$DESTDIR)
uri = CQML
QMLTYPES_FILENAME = $$DESTDIR/CQML.qmltypes

#Inputs
SOURCES += \
        cqml_plugin.cpp

HEADERS += \
        cqml_plugin.h

DISTFILES += \
        controls/CheckBox.qml \
        controls/ComboBox.qml \
        window/SystemDialog.qml \
        window/SystemMessage.qml \
        controls/Slider.qml \
        base/Clipboard.qml \ 
        controls/TextField.qml \ 
        extras/FolderDialog.qml \ 
        base/base.qrc \ 
        shaders/genie.vert \ 
        shaders/shaders.qrc \ 
        extras/FileDialog.qml \ 
        window/TabBar.qml \ 
        base/AwesomeMap.js \ 
        base/AwesomeRegular.ttf \ 
        base/AwesomeSolid.ttf \ 
        base/BaseBackground.qml \ 
        base/Icon.qml \ 
        base/IconHelper.qml \ 
        base/RippleMask.qml \ 
        base/WavedMouseArea.qml \ 
        controls/Tab.qml\ 
        controls/Button.qml \ 
        controls/ComboButton.qml \ 
        controls/IconButton.qml \ 
        controls/ImageButton.qml \ 
        controls/PlayerSlider.qml \ 
        controls/TabButton.qml \ 
        extras/MenuItems.qml \ 
        extras/StackMenu.qml 


RESOURCES += \
    base/base.qrc \
    shaders/shaders.qrc


qmldir.files = qmldir

#=============QMLDIR_COPY=============#
!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$DESTDIR/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)" "$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

#=============COPIES_STARTS=============#
base_copy.files = \ 
     base/WavedMouseArea.qml \ 
     base/RippleMask.qml \ 
     base/IconHelper.qml \ 
     base/Icon.qml \ 
     base/BaseBackground.qml \ 
     base/AwesomeSolid.ttf \ 
     base/AwesomeRegular.ttf \ 
     base/AwesomeMap.js \ 
     base/base.qrc \ 
     base/Clipboard.qml
base_copy.path = $$DESTDIR/base/

controls_copy.files = \ 
     controls/TabButton.qml \ 
     controls/CheckBox.qml \
     controls/ComboBox.qml \
     controls/PlayerSlider.qml \ 
     controls/ImageButton.qml \ 
     controls/IconButton.qml \ 
     controls/ComboButton.qml \ 
     controls/Button.qml \ 
     controls/Tab.qml \ 
     controls/TextField.qml \ 
     controls/Slider.qml
controls_copy.path = $$DESTDIR/controls/

extras_copy.files = \ 
     extras/StackMenu.qml \ 
     extras/MenuItems.qml \ 
     extras/FileDialog.qml \ 
     extras/FolderDialog.qml
extras_copy.path = $$DESTDIR/extras/

shaders_copy.files = \ 
     shaders/shaders.qrc \ 
     shaders/genie.vert
shaders_copy.path = $$DESTDIR/shaders/

window_copy.files = \ 
     window/SystemDialog.qml \
     window/SystemMessage.qml \
     window/TabBar.qml
window_copy.path = $$DESTDIR/window/

COPIES += base_copy controls_copy extras_copy shaders_copy window_copy
#=============COPIES_END=============#
