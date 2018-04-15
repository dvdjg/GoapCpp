TARGET = botan
QT       -= core gui

TEMPLATE = lib
CONFIG+= staticlib

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

INCLUDEPATH += $$top_srcdir/3rdparty/lzma/api
INCLUDEPATH += $$top_srcdir/3rdparty/sqlite
DEPENDPATH += $$INCLUDEPATH
DEFINES += BOTAN_TARGET_HAS_OPENMP

win32-msvc* {
DEFINES += BOTAN_TARGET_OS_IS_WINDOWS
QMAKE_CXXFLAGS += -bigobj
QMAKE_LFLAGS += -bigobj
DEFINES += BOTAN_TARGET_ARCH_IS_X86_64 BOTAN_BUILD_COMPILER_IS_MSVC _Pragma=__pragma
} else {
}
win32-g++:DEFINES += BOTAN_TARGET_OS_IS_MINGW
*-g++ {
    DEFINES += BOTAN_BUILD_COMPILER_IS_GCC
    *-g++-32:DEFINES += BOTAN_TARGET_ARCH_IS_X86_32
    *-g++-64:DEFINES += BOTAN_TARGET_ARCH_IS_X86_64
    QMAKE_CXXFLAGS+= -msse2 -Wa,-mbig-obj
    QMAKE_LFLAGS +=  -Wa,-mbig-obj
    CONFIG(debug, debug|release){
        QMAKE_CXXFLAGS += $$QMAKE_CFLAGS_OPTIMIZE_SIZE
    }
}
unix {
DEFINES += \
    BOTAN_TARGET_OS_TYPE_IS_UNIX \
    BOTAN_TARGET_OS_HAS_DLOPEN \
    BOTAN_TARGET_OS_IS_LINUX \
    BOTAN_TARGET_OS_TYPE_IS_UNIX \
    BOTAN_TARGET_OS_HAS_CLOCK_GETTIME
}

PRE_TARGETDEPS += \
    $$LIBDIR/$${LIBPRE}z$$SUFFIX$$LIBPOST \
    $$LIBDIR/$${LIBPRE}lzma$$SUFFIX$$LIBPOST


SOURCES += \
    botan_all.cpp \
    botan_all_aesni.cpp \
    botan_all_avx2.cpp \
    botan_all_rdrand.cpp \
    botan_all_ssse3.cpp

HEADERS += \
    botan_all.h \
    botan_all_internal.h \
    pkcs11.h \
    pkcs11f.h \
    pkcs11t.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
