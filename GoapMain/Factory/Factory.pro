#-------------------------------------------------
#
# Project created by QtCreator 2017-06-02T09:14:38
#
#-------------------------------------------------

QT       -= core gui

TARGET = Factory
#TEMPLATE = lib
#CONFIG += staticlib

!include(../common.pri) {
    error(Could not find the common.pri file!)
}

TEMPLATE = subdirs # leave SUBDIRS variable unassigned

HEADERS += \
    explicit_ptr.h \
    log_hook.h \
    factory.h \
    hasmember.h \
    instancedeleter.h \
    reuseobjectpool.h \
    utility_simple.h

