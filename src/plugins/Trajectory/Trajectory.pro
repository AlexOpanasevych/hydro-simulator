TEMPLATE = lib
TARGET = Trajectory
QT += qml
QT += quick
CONFIG += qt
CONFIG += plugin
CONFIG += c++17
CONFIG += qmltypes
CONFIG += shared dll

QML_IMPORT_NAME = $$TARGET
QML_IMPORT_MAJOR_VERSION = 1.0
DESTDIR = $$PWD/../../../bin/plugins/$$QML_IMPORT_NAME
QMLTYPES_FILENAME = $$DESTDIR/Trajectory.qmltypes
uri = $$TARGET

DEFINES += TRAJECTORY_LIBRARY
# Input
SOURCES += \
        qsgItems/segmentitem.cpp \
        qsgItems/traceitem.cpp \
        qsgItems/trajectoryitem.cpp \
        traceData/segmentdata.cpp \
        traceData/trace.cpp \
        traceData/trajectory.cpp \
        trajectory_plugin.cpp

HEADERS += \
        qsgItems/segmentitem.h \
        qsgItems/traceitem.h \
        qsgItems/trajectoryitem.h \
        traceData/segmentdata.h \
        traceData/trace.h \
        traceData/trajectory.h \
        trajectory_global.h \
        trajectory_plugin.h

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
        copy_qmldir.target = $$DESTDIR/qmldir
        copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
        copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)" "$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"
        QMAKE_EXTRA_TARGETS += copy_qmldir
        PRE_TARGETDEPS += $$copy_qmldir.target
}

CONFIG(release, debug|release){
DEFINES += RELEASE
INCLUDEPATH += $$PWD/../../
LIBS += -L$$PWD/../../../bin/libs -lklibcorelite
LIBS += -L$$PWD/../../../bin/libs -lpluginsbase
}else{
INCLUDEPATH += $$PWD/../../

win32: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/debug -lpluginsbase
else:unix: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/ -lpluginsbase

win32: LIBS += -L$$OUT_PWD/../../libs/klibcorelite/debug/ -lklibcorelite
else:unix: LIBS += -L$$OUT_PWD/../../libs/klibcorelite/ -lklibcorelite
}

INCLUDEPATH += $$PWD/../../libs/klibcorelite
DEPENDPATH += $$PWD/../../libs/klibcorelite

INCLUDEPATH += $$PWD/../../libs/PluginsBase
DEPENDPATH += $$PWD/../../libs/PluginsBase
