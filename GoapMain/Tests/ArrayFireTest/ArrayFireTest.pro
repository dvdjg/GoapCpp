#-------------------------------------------------
#
# Project created by QtCreator 2017-05-15T21:48:26
#
#-------------------------------------------------

#QT       += testlib

QT       -= gui core

TARGET = tst_data
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++14

TEMPLATE = app

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += tst_arrayfire.cpp \
    main.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../GoapLib/release/ -lGoapLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../GoapLib/debug/ -lGoapLib
else:unix: LIBS += -L$$OUT_PWD/../../GoapLib/ -lGoapLib

INCLUDEPATH += $$top_srcdir/../Factory $$top_srcdir/../Interfaces $$top_srcdir/../GoapLib $$top_srcdir/../sqlite_modern_cpp $$top_srcdir/../sqlite-amalgamation
DEPENDPATH += $$top_srcdir/../Factory $$top_srcdir/../Interfaces $$top_srcdir/../GoapLib $$top_srcdir/../sqlite_modern_cpp $$top_srcdir/../sqlite-amalgamation

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/release/libGoapLib.a $$OUT_PWD/../../sqlite-amalgamation/release/libsqlite-amalgamation$$SUFFIX.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/debug/libGoapLib.a $$OUT_PWD/../../sqlite-amalgamation/debug/libsqlite-amalgamation$$SUFFIX.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/release/GoapLib.lib $$OUT_PWD/../../sqlite-amalgamation/release/sqlite-amalgamation$$SUFFIX.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/debug/GoapLib.lib $$OUT_PWD/../../sqlite-amalgamation/debug/sqlite-amalgamation$$SUFFIX.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/libGoapLib.a $$OUT_PWD/../../sqlite-amalgamation/libsqlite-amalgamation$$SUFFIX.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../sqlite-amalgamation/release/ -lsqlite-amalgamation$$SUFFIX
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../sqlite-amalgamation/debug/ -lsqlite-amalgamation$$SUFFIX
else:unix: LIBS += -L$$OUT_PWD/../../sqlite-amalgamation/ -lsqlite-amalgamation$$SUFFIX

LIBS += -lgtest -lgmock

unix {
QMAKE_CXXFLAGS += -pthread
QMAKE_CFLAGS += -pthread -fno-strict-aliasing
LIBS += -pthread -ldl
}

