TARGET = tst_factorytesttest

TEMPLATE = app
CONFIG += console static
CONFIG -= app_bundle
CONFIG -= qt

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += \
    tst_factory_all.cpp \
    tst_factory_nocount.cpp \
    main.cpp \
    tst_signals.cpp

INCLUDEPATH += $$top_srcdir/3rdparty/gmock-gtest $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/3rdparty/sqlite_modern_cpp
DEPENDPATH += $$INCLUDEPATH

PRE_TARGETDEPS += \
    $$top_srcdir/lib/$${LIBPRE}GoapLib$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}gmock-gtest$$SUFFIX$$LIBPOST

LIBS += \
    -lGoapLib$$SUFFIX

unix {
QMAKE_CXXFLAGS += -pthread -fno-strict-aliasing -fopenmp
QMAKE_CFLAGS += -pthread -fno-strict-aliasing -fopenmp
QMAKE_LFLAGS += -pthread -fopenmp
} else:win32-msvc* {
QMAKE_CXXFLAGS += -openmp
QMAKE_CFLAGS += -openmp
}

LIBS += \
    -lgmock-gtest$$SUFFIX

# win32-msvc*:LIBS += -lbotan$$SUFFIX
