#-------------------------------------------------
#
# Project created by QtCreator 2016-05-09T21:39:38
#
#-------------------------------------------------

QT       -= core gui

TARGET = GoapLibrary
TEMPLATE = lib
CONFIG += staticlib

!include(../common.pri) {
    error(Could not find the common.pri file!)
}

INCLUDEPATH += ../Interfaces ../Factory ../3rdparty/half

SOURCES += \
    goaplibrary.cpp \
    statevalue.cpp \
    basicmath.cpp \
    memory.cpp \
    state.cpp

HEADERS += goaplibrary.h \
    refcounter.h \
    statevalue.h \
    basicmath.h \
    state.h \
    goapmemory.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
