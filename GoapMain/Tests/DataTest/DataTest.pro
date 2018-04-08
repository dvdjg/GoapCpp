TARGET = tst_data

QT       -= gui core
TEMPLATE = app
CONFIG += console static
CONFIG -= qt app_bundle


!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += \
    tst_data.cpp \
    main.cpp \
    tst_sqlite_encrypted.cpp \
    tst_sqlite_modern.cpp

INCLUDEPATH += $$top_srcdir/3rdparty/gmock-gtest $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLibrary $$top_srcdir/3rdparty/sqlite_modern_cpp $$top_srcdir/3rdparty/sqlite
DEPENDPATH += $$INCLUDEPATH

PRE_TARGETDEPS += \
    $$top_srcdir/lib/$${LIBPRE}botan$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}GoapLibrary$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}z$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}lzma$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}sqlite$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}gmock-gtest$$SUFFIX$$LIBPOST

POSTLIBS = $$LIBS
LIBS += \
    -lsqlite$$SUFFIX \
    -lbotan$$SUFFIX \
    -llzma$$SUFFIX \
    -lz$$SUFFIX \
    -lgmock-gtest$$SUFFIX \
    $$POSTLIBS

*win32*:LIBS += -lSecur32 -lCrypt32

unix {
    LIBS += -lbz2 -ldl
} else:win32-msvc* {
}

