#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = core
QT += core concurrent sql
CONFIG += c++17
CONFIG += shared dll

CONFIG(release, debug|release){
DESTDIR = $$PWD/../../../bin/libs
}
#Inputs
DEFINES += CORE_LIBRARY
INCLUDEPATH += $$PWD/
INCLUDEPATH += $$PWD/../..
HEADERS += \
    abstractengine.h \
    abstractpicontroller.h \
    basemovecontroller.h \
    basemsg.h \
    core_global.h \
    engineinterface.h \
    itemsfactory.h \
    logger.h \
    messageanswerdispatcher.h \
    messagedispatcher.h \
    physicalitemsampledata.h \
    sampledata.h \
    sampledatacontainer.h \
    scanable.h \
    spline.h \
    kcirclebuffer.h \
    circularbuffer.h \
    utility/VariantContainer.h \
    utility/unifiedcmdparser.h \
    waveable.h \
    wavesamplecontainer.h

SOURCES += \
    abstractengine.cpp \
    abstractpicontroller.cpp \
    basemovecontroller.cpp \
    basemsg.cpp \
    circularbuffer.cpp \
    engineinterface.cpp \
    itemsfactory.cpp \
    kcirclebuffer.cpp \
    logger.cpp \
    messageanswerdispatcher.cpp \
    messagedispatcher.cpp \
    physicalitemsampledata.cpp \
    sampledata.cpp \
    sampledatacontainer.cpp \
    scanable.cpp \
    utility/unifiedcmdparser.cpp \
    waveable.cpp \
    wavesamplecontainer.cpp
