TARGET = tst_factorytesttest

TEMPLATE = app
CONFIG += console static
CONFIG -= app_bundle
CONFIG -= qt

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += \
    tst_factorytesttest.cpp \
    main.cpp \
    tst_signals.cpp

INCLUDEPATH += $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/3rdparty/sqlite_modern_cpp
DEPENDPATH += $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/3rdparty/sqlite_modern_cpp

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/release/libGoapLib.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/debug/libGoapLib.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/release/GoapLib.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/debug/GoapLib.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/libGoapLib.a

LIBS += \
    -lGoapLib$$SUFFIX \
    -lgmock
unix {
QMAKE_CXXFLAGS += -pthread -fno-strict-aliasing -fopenmp
QMAKE_CFLAGS += -pthread -fno-strict-aliasing -fopenmp
QMAKE_LFLAGS += -pthread -fopenmp
LIBS += -lgtest
} else:win32-msvc* {
QMAKE_CXXFLAGS += -openmp
QMAKE_CFLAGS += -openmp
QMAKE_LFLAGS += -openmp
LIBS += -lgtest$$SUFFIX
}
# win32-msvc*:LIBS += -lbotan$$SUFFIX
