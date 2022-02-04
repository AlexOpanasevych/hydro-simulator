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
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DEPRECATED_WARNINGS

QML_IMPORT_NAME = klibcorelite
CONFIG(release, debug|release){
DESTDIR = $$PWD/../../../bin/libs/$$QML_IMPORT_NAME
}
#Inputs
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += KLIBCORELITE_LIBRARY
SOURCES += \
    kadditional.cpp \
    kclassregistry.cpp \
    klibinfo.cpp \
    math/kmath.cpp \
    models/kflexiblemodel.cpp \
    utility/kabstractfactory.cpp \
    utility/kdelegate.cpp \
    utility/kdelegatechooser.cpp \
    utility/kobservablelist.cpp \
    utility/ksysinfo.cpp \
    utility/ktype.cpp


HEADERS += \
    kadditional.h \
    kclassregistry.h \
    klibcorelite_global.h \
    klibinfo.h \
    kmacro.h \
    math/kmath.h \
    models/kflexiblemodel.h \
    utility/kabstractfactory.h \
    utility/kdelegate.h \
    utility/kdelegatechooser.h \
    utility/kobservablelist.h \
    utility/ksysinfo.h \
    utility/ktype.h


INCLUDEPATH += ./
CONFIG(release, debug|release){
#=============COPIES_STARTS=============#
qmldir.files = qmldir

default_copy.files = \ 
     kmacro.h \ 
     klibinfo.h \ 
     klibcorelite_global.h \ 
     kclassregistry.h \ 
     kadditional.h
default_copy.path = $$DESTDIR/

math_copy.files = \ 
     math/kmath.h
math_copy.path = $$DESTDIR/math/

models_copy.files = \ 
     models/kflexiblemodel.h
models_copy.path = $$DESTDIR/models/

utility_copy.files = \ 
     utility/ktype.h \ 
     utility/ksysinfo.h \ 
     utility/kobservablelist.h \ 
     utility/kdelegatechooser.h \ 
     utility/kdelegate.h \ 
     utility/kabstractfactory.h
utility_copy.path = $$DESTDIR/utility/

COPIES += default_copy math_copy models_copy utility_copy
#=============COPIES_END=============#
}
