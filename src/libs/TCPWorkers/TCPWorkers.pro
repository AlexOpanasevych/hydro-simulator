#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = TCPWorkers
QT -= gui
QT += network
QT += concurrent
CONFIG += shared dll
CONFIG += c++17

QML_IMPORT_NAME = TCPWorkers
CONFIG(release, debug|release){
DESTDIR = $$PWD/../../../bin/libs
}
#Inputs
DEFINES += TCPWORKERS_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/
INCLUDEPATH += $$PWD/../..

SOURCES += \
    streamconnector.cpp \
    streamconnectorclientside.cpp \
    streamconnectorv2.cpp \
    tcpclientworker.cpp \
    tcpserverworker.cpp

HEADERS += \
    Protocol.h \
    TCPWorkers_global.h \
    streamconnector.h \
    streamconnectorclientside.h \
    streamconnectorv2.h \
    tcpclientworker.h \
    tcpserverworker.h


CONFIG(release, debug|release) {
DEFINES += RELEASE
    LIBS += -L$$PWD/../../../bin/libs/ -lpluginsbase
}
else {
win32: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/debug -lpluginsbase
else:unix: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/ -lpluginsbase
}

INCLUDEPATH += $$PWD/../../libs/PluginsBase
DEPENDPATH += $$PWD/../../libs/PluginsBase
