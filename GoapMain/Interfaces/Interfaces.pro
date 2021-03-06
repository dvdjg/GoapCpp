#-------------------------------------------------
#
# Project created by QtCreator 2017-06-02T09:20:34
#
#-------------------------------------------------

QT       -= core gui

TARGET = Interfaces
TEMPLATE = subdirs # leave SUBDIRS variable unassigned
CONFIG += c++14

CONFIG(debug, debug|release){
    SUFFIX=d
    CONFDIR=debug
    DEFINES += GOAP_DEBUG
} else {
    SUFFIX=
    CONFDIR=release
}

DEFINES += HAS_BOOST_SMART_INTRUSIVE_PTR

INCLUDEPATH=../GoapLibrary ../Factory
INCLUDEPATH += F:/Programa/boost_1_71_0

HEADERS += \
    common/iroot.h \
    common/irefcounter.h \
    goap/iplanner.h \
    goap/ipath.h \
    goap/iplanningaction.h \
    goap/iplanningstatemeter.h \
    goap/ifunctionstatemeter.h \
    goap/iplanningstatecomparer.h \
    goap/iclonable.h \
    goap/ihashable.h \
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
