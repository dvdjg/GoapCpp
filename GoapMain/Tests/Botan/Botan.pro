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

INCLUDEPATH += $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/sqlite_modern_cpp $$top_srcdir/sqlite-amalgamation
DEPENDPATH += $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/sqlite_modern_cpp $$top_srcdir/sqlite-amalgamation

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/release/libGoapLib.a $$OUT_PWD/../../sqlite-amalgamation/release/libsqlite-amalgamation.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/debug/libGoapLib.a $$OUT_PWD/../../sqlite-amalgamation/debug/libsqlite-amalgamation.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/release/GoapLib.lib $$OUT_PWD/../../sqlite-amalgamation/release/sqlite-amalgamation.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/debug/GoapLib.lib $$OUT_PWD/../../sqlite-amalgamation/debug/sqlite-amalgamation.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/libGoapLib.a $$OUT_PWD/../../sqlite-amalgamation/libsqlite-amalgamation.a

unix {
QMAKE_CXXFLAGS += -pthread -fno-strict-aliasing
QMAKE_CFLAGS += -pthread -fno-strict-aliasing
QMAKE_LFLAGS += -pthread -fopenmp

LIBS += -ldl
LIBS += -L$$top_srcdir/3rdparty/botan/build_gcc -lbotan-2
LIBS += -lgtest
LIBS += -lbz2 -llzma -lz -ldl
LIBS +=  -lboost_system -lboost_filesystem
}

win32-msvc* {
LIBS += \
    -L$$top_srcdir/3rdparty/botan/build_msvc_d \
    -lgtest$$SUFFIX \
    -lbotan$$SUFFIX \
    -lUser32 -lAdvapi32
}

HEADERS += \
    tests.h

LIBS += \
    -lsqlite-amalgamation$$SUFFIX \
    -lGoapLib$$SUFFIX \
    -lgmock
