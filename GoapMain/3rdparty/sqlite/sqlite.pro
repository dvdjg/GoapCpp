QT       -= core gui

TARGET = sqlite
TEMPLATE = lib
CONFIG += staticlib

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

DEFINES += HAVE_STDINT_H HAVE_INTTYPES_H HAVE_UTIME HAVE_GETHOSTUUID

win32-*:INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_msvc
else:INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_gcc

LIBS += -lbotan$$SUFFIX -llzma

SOURCES += \
    sqlite3.c \
    codec.cpp

HEADERS += \
    sqlite3.h \
    sqlite3ext.h \
    codec.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
