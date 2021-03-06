#-------------------------------------------------
#
# Project created by QtCreator 2017-05-15T21:48:26
#
#-------------------------------------------------

#QT       += testlib

QT       -= gui core

TARGET = tst_arrayfire
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += \
    tst_arrayfire.cpp \
    main.cpp

INCLUDEPATH += $$top_srcdir/3rdparty/gmock-gtest $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLibrary $$top_srcdir/3rdparty/sqlite_modern_cpp $$top_srcdir/3rdparty/sqlite
DEPENDPATH += $$INCLUDEPATH

PRE_TARGETDEPS += \
    $$LIBDIR/$${LIBPRE}z$$SUFFIX$$LIBPOST \
    $$LIBDIR/$${LIBPRE}lzma$$SUFFIX$$LIBPOST \
    $$LIBDIR/$${LIBPRE}gmock-gtest$$SUFFIX$$LIBPOST

POST_LIBS = $$LIBS
LIBS =
win32-* {
    INCLUDEPATH += \
        "D:/Program Files/ArrayFire/v3/include"
    LIBS += \
        -L"D:/Program Files/ArrayFire/v3/lib" \
        -laf
}
win32-g++:LIBS += "D:/Program Files/ArrayFire/v3/lib/af.lib"
LIBS += \
    -lGoapLibrary$$SUFFIX \
    -lz$$SUFFIX \
    -llzma$$SUFFIX

#    -lsqlite$$SUFFIX \
#    -lbotan$$SUFFIX \

unix {
    LIBS += -lbz2 -ldl
}

LIBS += \
    -lgmock-gtest$$SUFFIX

LIBS += $$POST_LIBS
