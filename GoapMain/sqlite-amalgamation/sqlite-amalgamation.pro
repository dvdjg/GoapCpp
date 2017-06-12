#-------------------------------------------------
#
# Project created by QtCreator 2017-06-05T22:20:14
#
#-------------------------------------------------

QT       -= core gui

TARGET = sqlite-amalgamation
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14


!include(../common.pri) {
    error(Could not find the common.pri file!)
}

win32-msvc*:INCLUDEPATH += D:/OneDrive/Programa/GoapCpp/GoapMain/3rdparty/botan/build_msvc
#win32-msvc*:LIBS += -L"D:/OneDrive/Programa/GoapCpp/GoapMain/3rdparty/botan/build_msvc" -lbotan

SOURCES += \
    sqlite3.c \
    codec.cpp

HEADERS += \
    sqlite3.h \
    sqlite3ext.h \
    codec_c_interface.h \
    codec.h
unix {
    INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_gcc
    target.path = /usr/lib
    INSTALLS += target
}
