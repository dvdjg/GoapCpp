QT += core
QT -= gui

TARGET = GoapApp
CONFIG += console
CONFIG += static
CONFIG -= app_bundle

TEMPLATE = app

!include(../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += main.cpp
