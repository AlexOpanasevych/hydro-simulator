QT += quick core

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        pluginmanager.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    pluginmanager.h

INCLUDEPATH += $$PWD/../src/

CONFIG(release, debug|release): {
    DEFINES += BUILD=\"\\\"RELEASE\\\"\"
    LIBS += -L$$PWD/../bin/libs -lklibcorelite
}else{
    DEFINES += BUILD=\"\\\"DEBUG\\\"\"
    win32: LIBS += -L$$OUT_PWD/../src/libs/klibcorelite/debug -lklibcorelite
    else:unix: LIBS += -L$$OUT_PWD/../src/libs/klibcorelite -lklibcorelite
}

CONFIG(deploy, release|deploy):{
    DEFINES += BUILD=\"\\\"DEPLOY\\\"\"
}


INCLUDEPATH += $$PWD/../src/libs/klibcorelite
DEPENDPATH += $$PWD/../src/libs/klibcorelite