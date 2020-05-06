#-------------------------------------------------
#
# Project created by QtCreator 2017-06-02T09:20:34
#
#-------------------------------------------------

QT       -= core gui

TARGET = Interfaces
TEMPLATE = subdirs # leave SUBDIRS variable unassigned

INCLUDEPATH=../GoapLibrary ../Factory

HEADERS += \
    common/iroot.h \
    common/irefcounter.h \
    goap/iplanner.h \
    goap/ipath.h \
    goap/iplanningaction.h \
    goap/iplanningstatemeter.h \
    goap/iplanningstatecomparer.h \
    goap/iclonable.h \
    goap/iprioritized.h \
    goap/istate.h \
    goap/istringvalue.h \
    goap/imemory.h \
    goap/imemoryvalue.h \
    goap/istatevalue.h \
    goap/ibasicsink.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
