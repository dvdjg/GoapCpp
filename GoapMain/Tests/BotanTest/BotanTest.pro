TARGET = tst_botan

QT       -= gui core
TEMPLATE = app
CONFIG += console static
CONFIG -= qt app_bundle


!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += \
    main.cpp \
    test_compression.cpp \
    tests.cpp

HEADERS += \
    tests.h

win32-*:INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_msvc
else:INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_gcc

INCLUDEPATH += $$top_srcdir/3rdparty/gmock-gtest $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLibrary $$top_srcdir/3rdparty/sqlite_modern_cpp $$top_srcdir/3rdparty/sqlite
DEPENDPATH += $$INCLUDEPATH

PRE_TARGETDEPS += \
    $$top_srcdir/lib/$${LIBPRE}botan$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}GoapLibrary$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}z$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}lzma$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}sqlite$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}gmock-gtest$$SUFFIX$$LIBPOST

POST_LIBS = $$LIBS

LIBS = \
    -lbotan$$SUFFIX \
    -lsqlite$$SUFFIX \
    -lGoapLib$$SUFFIX \
    -lz$$SUFFIX \
    -llzma$$SUFFIX

LIBS += $$POST_LIBS

unix {
LIBS += -lbz2 -ldl
} else:win32-msvc* {
}

LIBS += \
    -lgmock-gtest$$SUFFIX

