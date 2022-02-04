#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = ClientParser
QT -= gui
CONFIG += shared dll
CONFIG += c++17

CONFIG(release, debug|release){
DESTDIR = $$PWD/../../../bin/libs
}
#Inputs
DEFINES += CLIENTPARSER_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    idataparser.cpp


HEADERS += \
    ClientParser_global.h \
    idataparser.h

