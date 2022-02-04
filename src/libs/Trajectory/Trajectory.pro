TEMPLATE = lib
TARGET = Trajectory
QT += qml
QT += quick
CONFIG += qt
CONFIG += c++17
CONFIG += shared dll

CONFIG(release, debug|release){
DESTDIR = $$PWD/../../../bin/libs
}

DEFINES += TRAJECTORY_LIBRARY
# Input
SOURCES += \
        qsgItems/segmentitem.cpp \
        qsgItems/traceitem.cpp \
        qsgItems/trajectoryitem.cpp \
        traceData/segmentdata.cpp \
        traceData/trace.cpp \
        traceData/trajectory.cpp

HEADERS += \
        qsgItems/segmentitem.h \
        qsgItems/traceitem.h \
        qsgItems/trajectoryitem.h \
        traceData/segmentdata.h \
        traceData/trace.h \
        traceData/trajectory.h \
        trajectory_global.h \
        trajectory_plugin.h

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
