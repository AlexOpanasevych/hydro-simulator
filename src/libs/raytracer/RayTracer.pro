#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = RayTracer
QT       += core gui quickwidgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++14
CONFIG += c++17
CONFIG += shared dll

QML_IMPORT_NAME = RayTracer
CONFIG(release, debug|release){
DESTDIR = $$PWD/../../../build/$$QML_IMPORT_NAME
}
#Inputs
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
        main.cpp \
    poly34.cpp \
    qcustomplot/qcustomplot.cpp \
    simulation.cpp \
    simulationgraphic.cpp \
    spline.cpp \
        widget.cpp \
    raycalculator.cpp


HEADERS += \
    poly34.h \
    propertyhelper.h \
    qcustomplot/qcustomplot.h \
    simulation.h \
    simulationgraphic.h \
    spline.h \
        widget.h \
    raycalculator.h


DISTFILES += \  
    ControlPanel.qml  



CONFIG(release, debug|release){
#=============COPIES_STARTS=============#
qmldir.files = qmldir

default_copy.files = \ 
     ControlPanel.qml \ 
     raycalculator.h \ 
     widget.h \ 
     spline.h \ 
     simulationgraphic.h \ 
     simulation.h \ 
     propertyhelper.h \ 
     poly34.h
default_copy.path = $$DESTDIR/

qcustomplot_copy.files = \ 
     qcustomplot/qcustomplot.h
qcustomplot_copy.path = $$DESTDIR/qcustomplot/

COPIES += default_copy qcustomplot_copy
#=============COPIES_END=============#
}
