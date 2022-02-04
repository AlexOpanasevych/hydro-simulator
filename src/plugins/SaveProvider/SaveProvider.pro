#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = SaveProvider
QT += qml
QT += quick
CONFIG += qt
CONFIG += plugin
CONFIG += c++17
CONFIG += qmltypes
CONFIG += shared dll

QML_IMPORT_NAME = SaveProvider
QML_IMPORT_MAJOR_VERSION = 1.0
DESTDIR = $$PWD/../../../bin/plugins/$$QML_IMPORT_NAME
QMLTYPES_FILENAME = $$DESTDIR/SaveProvider.qmltypes
uri = SaveProvider

#Inputs
DEFINES += SAVEPROVIDER_LIBRARY
SOURCES += \
        savedata.cpp \
        savemanager.cpp \
        saveprovider_plugin.cpp


HEADERS += \
        savedata.h \
        savemanager.h \
        saveprovider_plugin.h


DISTFILES = \
    qml/DragDelegate.qml \
    qml/MoveComboButton.qml \
    qml/menuWindow/AdditionalWindow.qml \
    qmldir \  
    qml/SaveEditWindow.qml \  
    qml/menuButton/SaveMenuButton.qml \   
    qml/menuWindow/SaveMenuWindow.qml   




#=============QMLDIR_COPY=============#
!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$DESTDIR/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)" "$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

#====================SPECIAL_PROPERTIES_STARTS====================
CONFIG(release, debug|release){
INCLUDEPATH += $$PWD/../../
LIBS += -L$$PWD/../../../bin/libs -lpluginsbase
LIBS += -L$$PWD/../../../bin/libs -lklibcorelite
LIBS += -L$$PWD/../../../bin/libs -lcore
}else{
win32: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/debug -lpluginsbase
else:unix: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/ -lpluginsbase
win32: LIBS += -L$$OUT_PWD/../../libs/klibcorelite/debug/ -lklibcorelite
else:unix: LIBS += -L$$OUT_PWD/../../libs/klibcorelite/ -lklibcorelite
win32: LIBS += -L$$OUT_PWD/../../libs/core/debug -lcore
else:unix: LIBS += -L$$OUT_PWD/../../libs/core/ -lcore
}
INCLUDEPATH += $$PWD/../../libs/PluginsBase
DEPENDPATH += $$PWD/../../libs/PluginsBase
INCLUDEPATH += $$PWD/../../libs/klibcorelite
DEPENDPATH += $$PWD/../../libs/klibcorelite
INCLUDEPATH += $$PWD/../../libs/core
DEPENDPATH += $$PWD/../../libs/core
INCLUDEPATH += $$PWD/../../
QML_IMPORT_PATH += $$PWD/../../../bin/plugins/
#====================SPECIAL_PROPERTIES_ENDS====================

#=============COPIES_STARTS=============#
qmldir.files = qmldir

default_copy.files = \ 
     qmldir \
     depencies.json

default_copy.path = $$DESTDIR/

qml_copy.files = \ 
     qml/SaveEditWindow.qml \
     qml/DragDelegate.qml \
     qml/MoveComboButton.qml
qml_copy.path = $$DESTDIR/qml/

qml_menuButton_copy.files = \ 
     qml/menuButton/SaveMenuButton.qml
qml_menuButton_copy.path = $$DESTDIR/qml/menuButton/

qml_menuWindow_copy.files = \ 
     qml/menuWindow/AdditionalWindow.qml \
     qml/menuWindow/SaveMenuWindow.qml
qml_menuWindow_copy.path = $$DESTDIR/qml/menuWindow/

COPIES += default_copy qml_copy qml_menuButton_copy qml_menuWindow_copy
#=============COPIES_END=============#
