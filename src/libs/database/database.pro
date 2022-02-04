#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = database
QT += sql
CONFIG += c++17
CONFIG += shared dll

CONFIG(release, debug|release){

DESTDIR = $$PWD/../../../bin/libs
}
#Inputs
DEFINES += DATABASE_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    loginservice.cpp \
    mysqlprovider.cpp \
    sqliteprovider.cpp \
    ssmsprovider.cpp


HEADERS += \
    database_global.h \
    idatabaseprovider.h \
    loginservice.h \
    mysqlprovider.h \
    sqliteprovider.h \
    ssmsprovider.h

