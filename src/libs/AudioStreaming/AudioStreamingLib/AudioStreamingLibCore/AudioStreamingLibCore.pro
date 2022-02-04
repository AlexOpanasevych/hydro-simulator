TEMPLATE =  lib
TARGET =    AudioStreamingLibCore
QT += core network multimedia
CONFIG += c++17
CONFIG +=   shared dll


INCLUDEPATH += $$PWD/3rdparty
INCLUDEPATH += $$PWD

CONFIG(release, debug|release){
DESTDIR = $$PWD/../../../../../bin/libs/
}

DEFINES += AUDIOSTREAMING_LIBRARY
INCLUDEPATH += $$PWD/
INCLUDEPATH += $$PWD/../..

SOURCES +=  $$PWD/audiostreaminglibcore.cpp \
            $$PWD/audiostreamingworker.cpp \
            $$PWD/discoverserver.cpp \
            $$PWD/discoverclient.cpp \
            $$PWD/abstractclient.cpp \
            $$PWD/abstractserver.cpp \
            $$PWD/client.cpp \
            $$PWD/server.cpp \
            $$PWD/encryptedclient.cpp \
            $$PWD/encryptedserver.cpp \
            $$PWD/webclient.cpp \
            $$PWD/sslclient.cpp \
            $$PWD/openssllib.cpp \
            $$PWD/tcpserver.cpp \
            $$PWD/audioinput.cpp \
            $$PWD/audiooutput.cpp \
            $$PWD/flowcontrol.cpp \
            $$PWD/levelmeter.cpp

HEADERS +=  $$PWD/audiostreaminglibcore.h \
            $$PWD/audiostreamingworker.h \
            $$PWD/common.h \
            $$PWD/audiocommon.h \
            $$PWD/threadcommon.h \
            $$PWD/discoverserver.h \
            $$PWD/discoverclient.h \
            $$PWD/abstractclient.h \
            $$PWD/abstractserver.h \
            $$PWD/client.h \
            $$PWD/server.h \
            $$PWD/encryptedserver.h \
            $$PWD/encryptedclient.h \
            $$PWD/webclient.h \
            $$PWD/sslclient.h \
            $$PWD/openssllib.h \
            $$PWD/tcpserver.h \
            $$PWD/audioinput.h \
            $$PWD/audiooutput.h \
            $$PWD/flowcontrol.h \
            $$PWD/levelmeter.h \
            audiostreaminglib_global.h

#Include r8brain
INCLUDEPATH +=  $$PWD/3rdparty/r8brain
SOURCES     +=  $$PWD/3rdparty/r8brain/r8bbase.cpp
SOURCES     +=  $$PWD/r8brain.cpp
HEADERS     +=  $$PWD/r8brain.h

WITH_OPUS {
DEFINES +=  NOMINMAX

SOURCES +=  $$PWD/opusencoderclass.cpp \
            $$PWD/opusdecoderclass.cpp

HEADERS +=  $$PWD/opusencoderclass.h \
            $$PWD/opusdecoderclass.h
}


DEBUG_LEVEL = 1 #2 maximum verbose, 1 less verbose, 0 no debug messages

OPUS_ENABLED = #TRUE
OPENSSL_ENABLED = #TRUE

UNKNOWN_CUSTOM_ACCELERATION_DEFINE = #TRUE
UNKNOWN_CUSTOM_ACCELERATION_FLAG = #TRUE
UNKNOWN_CUSTOM_ACCELERATION =

UNKNOWN_OPUS_INCLUDE =
UNKNOWN_OPUS_LIB =
UNKNOWN_OPENSSL_SSL_LIB =
UNKNOWN_OPENSSL_CRYPTO_LIB =

USE_CUSTOM_ACCELERATION = #TRUE
CUSTOM_ACCELERATION =

WIN_OPUS_INCLUDE =
ANDROID_OPUS_INCLUDE =
LINUX_OPUS_INCLUDE =
MACOS_OPUS_INCLUDE =

MINGW_32BITS_OPUS_LIB =

MINGW_32BITS_OPENSSL_SSL_LIB =
MINGW_32BITS_OPENSSL_CRYPTO_LIB =

MINGW_64BITS_OPUS_LIB =

MINGW_64BITS_OPENSSL_SSL_LIB =
MINGW_64BITS_OPENSSL_CRYPTO_LIB =

MSVC_32BITS_DEBUG_OPUS_LIB =
MSVC_32BITS_DEBUG_CELT_LIB =
MSVC_32BITS_DEBUG_SILK_COMMON_LIB =
MSVC_32BITS_DEBUG_SILK_FLOAT_LIB =

MSVC_32BITS_RELEASE_OPUS_LIB =
MSVC_32BITS_RELEASE_CELT_LIB =
MSVC_32BITS_RELEASE_SILK_COMMON_LIB =
MSVC_32BITS_RELEASE_SILK_FLOAT_LIB =

MSVC_32BITS_OPENSSL_SSL_LIB =
MSVC_32BITS_OPENSSL_CRYPTO_LIB =

MSVC_64BITS_DEBUG_OPUS_LIB =
MSVC_64BITS_DEBUG_CELT_LIB =
MSVC_64BITS_DEBUG_SILK_COMMON_LIB =
MSVC_64BITS_DEBUG_SILK_FLOAT_LIB =

MSVC_64BITS_RELEASE_OPUS_LIB =
MSVC_64BITS_RELEASE_CELT_LIB =
MSVC_64BITS_RELEASE_SILK_COMMON_LIB =
MSVC_64BITS_RELEASE_SILK_FLOAT_LIB =

MSVC_64BITS_OPENSSL_SSL_LIB =
MSVC_64BITS_OPENSSL_CRYPTO_LIB =

LINUX_32BITS_OPUS_LIB =

LINUX_32BITS_OPENSSL_SSL_LIB =
LINUX_32BITS_OPENSSL_CRYPTO_LIB =

LINUX_64BITS_OPUS_LIB =

LINUX_64BITS_OPENSSL_SSL_LIB =
LINUX_64BITS_OPENSSL_CRYPTO_LIB =

MACOS_OPUS_LIB =

MACOS_OPENSSL_SSL_LIB =
MACOS_OPENSSL_CRYPTO_LIB =

ANDROID_ARMV7A_OPUS_LIB =

ANDROID_ARMV7A_OPENSSL_SSL_LIB =
ANDROID_ARMV7A_OPENSSL_CRYPTO_LIB =

ANDROID_X86_OPUS_LIB =

ANDROID_X86_OPENSSL_SSL_LIB =
ANDROID_X86_OPENSSL_CRYPTO_LIB =



if (contains(OPUS_ENABLED, TRUE)) CONFIG += WITH_OPUS

if (contains(OPENSSL_ENABLED, TRUE)) CONFIG += WITH_OPENSSL

if (contains(DEBUG_LEVEL, 1)) DEFINES += IS_TO_DEBUG_VERBOSE_1
if (contains(DEBUG_LEVEL, 2)) DEFINES += IS_TO_DEBUG_VERBOSE_2

WITH_OPUS:DEFINES += OPUS
WITH_OPENSSL:DEFINES += OPENSSL

INCLUDEPATH += $$PWD/AudioStreamingLib

contains(QT_ARCH, x86_64):win32-g++ { #MINGW 64 bits
message("AudioStreamingLib is using settings for Windows MinGW 64 bits")

CONFIG += KNOWNDEVICE

DEFINES += EIGEN_MPL2_ONLY
DEFINES += EIGEN_DONT_VECTORIZE #Clear vector flags

!contains(USE_CUSTOM_ACCELERATION, TRUE){
DEFINES += EIGEN_VECTORIZE_SSE3
} else {
DEFINES += $$CUSTOM_ACCELERATION
}

WITH_OPUS:LIBS += $$MINGW_64BITS_OPUS_LIB

WITH_OPENSSL {
LIBS += $$MINGW_64BITS_OPENSSL_SSL_LIB
LIBS += $$MINGW_64BITS_OPENSSL_CRYPTO_LIB
LIBS += -lGdi32
}
} #MINGW 64 bits

contains(QT_ARCH, x86_64):unix:!macx:!android { #LINUX 64 bits
message("AudioStreamingLib is using settings for Linux 64 bits")

CONFIG += KNOWNDEVICE

DEFINES += EIGEN_MPL2_ONLY
DEFINES += EIGEN_DONT_VECTORIZE #Clear vector flags

!contains(USE_CUSTOM_ACCELERATION, TRUE){
DEFINES += EIGEN_VECTORIZE_SSE3
} else {
DEFINES += $$CUSTOM_ACCELERATION
}

WITH_OPUS:LIBS += $$LINUX_64BITS_OPUS_LIB
WITH_OPENSSL {
LIBS += $$LINUX_64BITS_OPENSSL_SSL_LIB
LIBS += $$LINUX_64BITS_OPENSSL_CRYPTO_LIB
LIBS += -ldl
}
} #LINUX 64 bits

#Include opus headers
WITH_OPUS {
KNOWNDEVICE:win32:INCLUDEPATH += $$WIN_OPUS_INCLUDE
KNOWNDEVICE:android:INCLUDEPATH += $$ANDROID_OPUS_INCLUDE
KNOWNDEVICE:unix:!macx:!android:INCLUDEPATH += $$LINUX_OPUS_INCLUDE
KNOWNDEVICE:macx:INCLUDEPATH += $$MACOS_OPUS_INCLUDE
!KNOWNDEVICE:INCLUDEPATH += $$UNKNOWN_OPUS_INCLUDE
}
