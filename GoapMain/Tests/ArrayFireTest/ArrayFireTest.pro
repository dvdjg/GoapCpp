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

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../GoapLib/release/ -lGoapLib
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../GoapLib/debug/ -lGoapLib
#else:unix: LIBS += -L$$OUT_PWD/../../GoapLib/ -lGoapLib

INCLUDEPATH += $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/sqlite_modern_cpp $$top_srcdir/sqlite-amalgamation
DEPENDPATH += $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/sqlite_modern_cpp $$top_srcdir/sqlite-amalgamation

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/release/libGoapLib.a $$OUT_PWD/../../sqlite-amalgamation/release/libsqlite-amalgamation$$SUFFIX.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/debug/libGoapLib.a $$OUT_PWD/../../sqlite-amalgamation/debug/libsqlite-amalgamation$$SUFFIX.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/release/GoapLib.lib $$OUT_PWD/../../sqlite-amalgamation/release/sqlite-amalgamation$$SUFFIX.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/debug/GoapLib.lib $$OUT_PWD/../../sqlite-amalgamation/debug/sqlite-amalgamation$$SUFFIX.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/libGoapLib.a $$OUT_PWD/../../sqlite-amalgamation/libsqlite-amalgamation$$SUFFIX.a

LIBS += \
    -lsqlite-amalgamation$$SUFFIX \
    -lGoapLib$$SUFFIX \
    -lgmock

unix {
QMAKE_CXXFLAGS += -pthread -fno-strict-aliasing
QMAKE_CFLAGS += -pthread -fno-strict-aliasing
QMAKE_LFLAGS += -pthread -fopenmp
LIBS += -ldl \
    -L/home/david/Programa/GoapCpp/GoapMain/3rdparty/botan/build_gcc  -lbotan-2
LIBS += -lgtest
LIBS += -lbz2 -llzma -lz -ldl
LIBS +=  -lboost_system -lboost_filesystem
}

win32-msvc*:LIBS += -lgtest$$SUFFIX
