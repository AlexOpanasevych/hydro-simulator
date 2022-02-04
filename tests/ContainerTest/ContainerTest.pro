QT += testlib
QT -= gui
QT += concurrent
QT += sql
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
CONFIG += c++17

SOURCES +=  tst_testsdcontainer.cpp



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/libs/core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/libs/core/debug/ -lcore
else:unix:!macx: LIBS += -L$$OUT_PWD/../../src/libs/core/ -lcore

INCLUDEPATH += $$PWD/../../src/libs/core
DEPENDPATH += $$PWD/../../src/libs/core

