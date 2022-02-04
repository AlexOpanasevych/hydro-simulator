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
DESTDIR = $$PWD/../../bin/plugins/$$QML_IMPORT_NAME
uri = CQML
QMLTYPES_FILENAME = $$DESTDIR/CQML.qmltypes

#Inputs
SOURCES+=\
        cqml_plugin.cpp \
        testitem.cpp


HEADERS+=\
        cqml_plugin.h \
        testitem.h


DISTFILES=\ 
        base/AwesomeMap.js \ 
        base/BaseBackground.qml \ 
        base/Icon.qml \ 
        base/IconHelper.qml \ 
        base/RippleMask.qml \ 
        base/WavedMouseArea.qml \ 
        controls/Button.qml \ 
        controls/ComboButton.qml \ 
        controls/IconButton.qml \ 
        controls/ImageButton.qml \ 
        controls/PlayerSlider.qml \ 
        controls/Slider.qml \ 
        controls/Slider2.qml \ 
        controls/TabButton.qml \ 
        extras/MenuItems.qml \ 
        extras/StackMenu.qml \ 
        qmldir 


qmldir.files = qmldir

#=============QMLDIR_COPY=============#
!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$DESTDIR/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

#=============COPIES_STARTS=============#
default_copy.files = \ 
     qmldir
default_copy.path = $$DESTDIR/

base_copy.files = \ 
     base/WavedMouseArea.qml \ 
     base/RippleMask.qml \ 
     base/IconHelper.qml \ 
     base/Icon.qml \ 
     base/BaseBackground.qml \ 
     base/AwesomeMap.js
base_copy.path = $$DESTDIR/base/

controls_copy.files = \ 
     controls/TabButton.qml \ 
     controls/Slider2.qml \ 
     controls/Slider.qml \ 
     controls/PlayerSlider.qml \ 
     controls/ImageButton.qml \ 
     controls/IconButton.qml \ 
     controls/ComboButton.qml \ 
     controls/Button.qml
controls_copy.path = $$DESTDIR/controls/

extras_copy.files = \ 
     extras/StackMenu.qml \ 
     extras/MenuItems.qml
extras_copy.path = $$DESTDIR/extras/

COPIES += default_copy base_copy controls_copy extras_copy
#=============COPIES_END=============#
