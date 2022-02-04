QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testregexmatch.cpp

INCLUDEPATH += $$PWD/../../src/libs/PluginsBase
DEPENDPATH += $$PWD/../../src/libs/PluginsBase

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/libs/PluginsBase/release/ -lPluginsBase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/libs/PluginsBase/debug/ -lPluginsBase
else:unix: LIBS += -L$$OUT_PWD/../../src/libs/PluginsBase/ -lPluginsBase

INCLUDEPATH += $$PWD/../../src/

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/libs/core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/libs/core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../../src/libs/core/ -lcore

INCLUDEPATH += $$PWD/../../src/libs/core
DEPENDPATH += $$PWD/../../src/libs/core
