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
    $$top_srcdir/lib/$${LIBPRE}gmock-gtest$$SUFFIX$$LIBPOST

LIBS += \
    -lgloox$$SUFFIX \
    -lz$$SUFFIX \
    -lgmock-gtest$$SUFFIX


unix {
    QMAKE_CXXFLAGS += -pthread -fno-strict-aliasing
    QMAKE_CFLAGS += -pthread -fno-strict-aliasing
    QMAKE_LFLAGS += -pthread -fopenmp
    LIBS += -ldl
    LIBS += -lboost_system -lboost_filesystem
} else:win32-msvc* {
    LIBS += \
        -lUser32 -lAdvapi32
}
