#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = Tools
QT += qml
QT += quick
CONFIG += qt
CONFIG += plugin
CONFIG += c++17
CONFIG += qmltypes
CONFIG += shared dll

QML_IMPORT_NAME = Tools
QML_IMPORT_VERSION = 1.0
DESTDIR = $$PWD/../../../bin/plugins/$$QML_IMPORT_NAME
uri = Vessel
QMLTYPES_FILENAME = $$DESTDIR/Tools.qmltypes
#Inputs
SOURCES += \
        cusommonitor.cpp \
        tools_plugin.cpp \
        myitem.cpp


HEADERS += \
        cusommonitor.h \
        tools_plugin.h \
        myitem.h


DISTFILES = qmldir      \
    qml/Test/TestBackgroundExtension.qml \
    qml/Test/TestSettingsButton.qml \
    qml/Test/TestSettingsWindow.qml
#====================SPECIAL_PROPERTIES_STARTS====================
CONFIG(release, debug|release){
DEFINES += RELEASE
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
QML_IMPORT_PATH += $$PWD/../../../bin/plugins/
#====================SPECIAL_PROPERTIES_ENDS====================

#=============COPIES_STARTS=============#
qmldir.files = qmldir

default_copy.files = \
     qmldir \
     depencies.json
default_copy.path = $$DESTDIR/

qml_Test_copy.files = \
     qml/Test/TestSettingsWindow.qml \
     qml/Test/TestSettingsButton.qml \
     qml/Test/TestBackgroundExtension.qml
qml_Test_copy.path = $$DESTDIR/qml/Test/

COPIES += default_copy qml_Test_copy
#=============COPIES_END=============#

