#-------------------------------------------------
#
# Project created by QtCreator 2017-05-15T21:41:51
#
#-------------------------------------------------

QT       -= core gui

TARGET = GoapLib
TEMPLATE = lib
CONFIG += staticlib

!include(../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += goaplib.cpp

HEADERS += goaplib.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


