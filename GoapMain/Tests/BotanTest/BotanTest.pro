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

unix:INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_gcc
win32-msvc*:INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_msvc

INCLUDEPATH += $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/3rdparty/sqlite_modern_cpp $$top_srcdir/3rdparty/sqlite
DEPENDPATH += $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/3rdparty/sqlite_modern_cpp $$top_srcdir/3rdparty/sqlite

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/release/libGoapLib.a $$OUT_PWD/../../sqlite/release/libsqlite.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/debug/libGoapLib.a $$OUT_PWD/../../sqlite/debug/libsqlite.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/release/GoapLib.lib $$OUT_PWD/../../sqlite/release/sqlite.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/debug/GoapLib.lib $$OUT_PWD/../../sqlite/debug/sqlite.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/libGoapLib.a $$OUT_PWD/../../sqlite/libsqlite.a

PRE_TARGETDEPS += \
    $$top_srcdir/lib/$${LIBPRE}botan$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}GoapLib$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}z$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}sqlite$$SUFFIX$$LIBPOST

LIBS += \
    -lbotan$$SUFFIX \
    -lsqlite$$SUFFIX \
    -lGoapLib$$SUFFIX \
    -lz$$SUFFIX

unix {
QMAKE_CXXFLAGS += -pthread -fno-strict-aliasing
QMAKE_CFLAGS += -pthread -fno-strict-aliasing
QMAKE_LFLAGS += -pthread -fopenmp
LIBS += -lgtest -lgmock
LIBS += -lbz2 -llzma -ldl
LIBS +=  -lboost_system -lboost_filesystem
} else:win32-msvc* {
LIBS += \
    -lgtest$$SUFFIX \
    -lgmock$$SUFFIX \
    -lUser32 -lAdvapi32
}


