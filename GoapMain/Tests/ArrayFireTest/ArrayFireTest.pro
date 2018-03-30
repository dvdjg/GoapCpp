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

INCLUDEPATH += $$top_srcdir/3rdparty/gmock-gtest $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/3rdparty/sqlite_modern_cpp $$top_srcdir/3rdparty/sqlite
DEPENDPATH += $$INCLUDEPATH

PRE_TARGETDEPS += \
    $$top_srcdir/lib/$${LIBPRE}z$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}lzma$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}gmock-gtest$$SUFFIX$$LIBPOST

win32-* {
    INCLUDEPATH += \
        "F:/Program Files/ArrayFire/v3/include"
    LIBS += \
        -L"F:/Program Files/ArrayFire/v3/lib" \
        -laf
}
win32-g++:LIBS += "F:/Program Files/ArrayFire/v3/lib/af.lib"
LIBS += \
    -lGoapLib$$SUFFIX \
    -lz$$SUFFIX \
    -llzma$$SUFFIX

#    -lsqlite$$SUFFIX \
#    -lbotan$$SUFFIX \

unix {
    LIBS += -lbz2 -ldl
}

LIBS += \
    -lgmock-gtest$$SUFFIX
