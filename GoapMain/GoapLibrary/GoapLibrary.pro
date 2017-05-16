#-------------------------------------------------
#
# Project created by QtCreator 2016-05-09T21:39:38
#
#-------------------------------------------------

QT       -= gui

TARGET = GoapLibrary
TEMPLATE = lib
CONFIG += staticlib c++11

CONFIG(debug, debug|release):SUFFIX=d
else:SUFFIX=

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

LIBDIR=$$LIBDIR/$$SPEC
DESTDIR=$$DESTDIR/$$SPEC

SOURCES += goaplibrary.cpp \
    statevalue.cpp \
    basicmath.cpp \
    memory.cpp \
    state.cpp

HEADERS += goaplibrary.h \
    istatevalue.h \
    imemory.h \
    imemoryvalue.h \
    statevalue.h \
    basicmath.h \
    memory.h \
    iintrusiveptr.h \
    ireadstate.h \
    state.h \
    shared.h \
    ptr.h \
    ptr_forward.h \
    objectpool.h \
    factory.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
