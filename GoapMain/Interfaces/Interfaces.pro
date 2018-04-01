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
    goap/imemory.h \
    goap/imemoryvalue.h \
    goap/ireadstate.h \
    goap/istatevalue.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
