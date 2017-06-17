TARGET = half
QT       -= core gui

TEMPLATE = lib
CONFIG+= staticlib

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}


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
