#-------------------------------------------------
#
# Project created by QtCreator 2017-06-05T22:20:14
#
#-------------------------------------------------

QT       -= core gui

TARGET = sqlite
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14


!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

unix:INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_gcc
win32-msvc*:INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_msvc

LIBS += -lbotan$$SUFFIX -llzma

SOURCES += \
    sqlite3.c \
    codec.cpp

HEADERS += \
    sqlite3.h \
    sqlite3ext.h \
    codec_c_interface.h \
    codec.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
