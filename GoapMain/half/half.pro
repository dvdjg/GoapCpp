TARGET = half
QT       -= core gui

TEMPLATE = lib
CONFIG+= staticlib

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


SOURCES +=  half.cpp

HEADERS +=  half.h\
            halfExport.h \
            halfFunction.h \
            halfLimits.h \
            eLut.h \
            toFloat.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
