QT       -= gui core
TEMPLATE = app
CONFIG += console static
CONFIG -= qt app_bundle


!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

INCLUDEPATH += $$top_srcdir/3rdparty/gmock-gtest  $$top_srcdir/3rdparty/gloox/src
DEPENDPATH += $$INCLUDEPATH

PRE_TARGETDEPS += \
    $$top_srcdir/lib/$${LIBPRE}gloox$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}z$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}lzma$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}gmock-gtest$$SUFFIX$$LIBPOST

POSTLIBS = $$LIBS
LIBS += \
    -lgloox$$SUFFIX \
    -llzma$$SUFFIX \
    -lz$$SUFFIX \
    -lgmock-gtest$$SUFFIX \
    $$POSTLIBS

*win32*:LIBS += -lSecur32 -lCrypt32

unix {
    LIBS += -ldl
} else:win32-msvc* {
}
