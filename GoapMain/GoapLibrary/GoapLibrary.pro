#-------------------------------------------------
#
# Project created by QtCreator 2016-05-09T21:39:38
#
#-------------------------------------------------

QT       -= core gui

TARGET = GoapLibrary
TEMPLATE = lib
CONFIG += staticlib c++14

CONFIG(debug, debug|release):SUFFIX=d
else:SUFFIX=

!include(../common.pri) {
    error(Could not find the common.pri file!)
}

TARGET=$$TARGET$$SUFFIX

*-g++:QMAKE_TARGET.arch = $$QMAKE_HOST.arch
*-g++-32:QMAKE_TARGET.arch = x86
*-g++-64:QMAKE_TARGET.arch = x86_64

SPEC=$$basename(QMAKESPEC)
SPEC=$$replace(SPEC, win32, windows)-$$QMAKE_TARGET.arch

LIBDIR=$$PWD/../lib
staticlib {
    DEFINES += STATIC
    DESTDIR=$$LIBDIR
} else {
    # Binaries and dynamic libs goes to bin
    DESTDIR=$$PWD/../bin
    static:DESTDIR=$$DESTDIR-static
}

INCLUDEPATH=../Interfaces ../Factory

LIBDIR=$$LIBDIR/$$SPEC
DESTDIR=$$DESTDIR/$$SPEC

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += goaplibrary.cpp \
    statevalue.cpp \
    basicmath.cpp \
    memory.cpp \
    state.cpp

HEADERS += goaplibrary.h \
    statevalue.h \
    basicmath.h \
    memory.h \
    state.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
