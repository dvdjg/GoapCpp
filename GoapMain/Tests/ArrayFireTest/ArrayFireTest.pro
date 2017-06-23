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
    $$top_srcdir/lib/$${LIBPRE}GoapLib$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}z$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}sqlite$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}gmock-gtest$$SUFFIX$$LIBPOST

LIBS += \
    -lGoapLib$$SUFFIX \
    -lsqlite$$SUFFIX \
    -lbotan$$SUFFIX \
    -lz$$SUFFIX

unix {
QMAKE_CXXFLAGS += -pthread -fno-strict-aliasing
QMAKE_CFLAGS += -pthread -fno-strict-aliasing
QMAKE_LFLAGS += -pthread -fopenmp
LIBS += -lbz2 -llzma -ldl
LIBS += -lboost_system -lboost_filesystem
}

LIBS += \
    -lgmock-gtest$$SUFFIX
