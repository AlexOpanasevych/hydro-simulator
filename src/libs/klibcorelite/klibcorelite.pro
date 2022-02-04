#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = klibcorelite
QT -= gui
QT += core
QT += quick
QT += testlib
QT += widgets
QT += multimedia
CONFIG += shared dll
CONFIG += c++17

CONFIG(release, debug|release){
DESTDIR = $$PWD/../../../bin/libs
}
#Inputs
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += KLIBCORELITE_LIBRARY
SOURCES += \
    klibinfo.cpp \
    math/kmath.cpp \
    kadditional.cpp \
    kclassregistry.cpp \
    models/kflexiblemodel.cpp \
    models/modelmanager.cpp \
    utility/kcirclebuffer.cpp \
    utility/ktype.cpp \
    utility/ksysinfo.cpp \
    utility/kdelegate.cpp \
    utility/kobservablelist.cpp \
    utility/kabstractfactory.cpp \
    utility/kdelegatechooser.cpp \
    utility/kuniversalqueue.cpp \
    utility/kvoidhandle.cpp

HEADERS += \
    kadditional.h \
    kclassregistry.h \
    klibcorelite_global.h \
    klibinfo.h \
    kmacro.h \
    math/kmath.h \
    models/kflexiblemodel.h \
    models/modelmanager.h \
    utility/kabstractfactory.h \
    utility/kcirclebuffer.h \
    utility/kdelegate.h \
    utility/kdelegatechooser.h \
    utility/kobservablelist.h \
    utility/kslotregisterer.h \
    utility/ksysinfo.h \
    utility/ktype.h \
    utility/kuniversalqueue.h \
    utility/kvoidhandle.h \
    utility/kutilinfo.h


INCLUDEPATH += ./
