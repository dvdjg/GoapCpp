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
    path.cpp \
    planner.cpp \
    statevalue.cpp \
    basicmath.cpp \
    memory.cpp \
    state.cpp \
    time_utils.cpp \
    basicostream.cpp \
    basicsinkcollection.cpp \
    numericstatecomparer.cpp \
    exactstatecomparer.cpp \
    goaplibinscribe.cpp

HEADERS += goaplibrary.h \
    cosinedistance.h \
    refcounter.h \
    statevalue.h \
    basicmath.h \
    state.h \
    goapmemory.h \
    time_utils.h \
    basicostreamsink.h \
    basicostream.h \
    basicsinkcollection.h \
    planningstatemeter.h \
    functionstatemeter.h \
    numericstatecomparer.h \
    exactstatecomparer.h \
    comparerstatemeter.h \
    goaplibinscribe.h \
    scopetime.h \
    newptr.h \
    actioninfo.h \
    path.h \
    planner.h \
    planningaction.h \
    prioritizedqueue.h \
    scopetimeostream.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
