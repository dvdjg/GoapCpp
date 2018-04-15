#-------------------------------------------------
#
# Project created by QtCreator 2017-06-02T09:14:38
#
#-------------------------------------------------

QT       -= core gui

TARGET = Factory
#TEMPLATE = lib
#CONFIG += staticlib

TEMPLATE = subdirs # leave SUBDIRS variable unassigned
INCLUDEPATH += \
    F:/Programa/boost_1_66_0

#SOURCES += nowTime.cpp

HEADERS += \
    explicit_ptr.h \
    log_hook.h \
    factory.h \
    hasmember.h \
    instancedeleter.h \
    reuseobjectpool.h

