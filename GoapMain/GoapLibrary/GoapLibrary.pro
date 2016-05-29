#-------------------------------------------------
#
# Project created by QtCreator 2016-05-09T21:39:38
#
#-------------------------------------------------

QT       -= core gui

TARGET = GoapLibrary
TEMPLATE = lib
CONFIG += staticlib

SOURCES += goaplibrary.cpp

HEADERS += goaplibrary.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
