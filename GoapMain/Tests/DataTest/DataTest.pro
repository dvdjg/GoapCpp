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

INCLUDEPATH += $$top_srcdir/3rdparty/gmock-gtest $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/3rdparty/sqlite_modern_cpp $$top_srcdir/3rdparty/sqlite
DEPENDPATH += $$INCLUDEPATH

PRE_TARGETDEPS += \
    $$top_srcdir/lib/$${LIBPRE}botan$$SUFFIX$$LIBPOST \
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
LIBS +=  -lboost_system -lboost_filesystem
} else:win32-msvc* {
LIBS += \
    -lUser32 -lAdvapi32
}

LIBS += \
    -lgmock-gtest$$SUFFIX
