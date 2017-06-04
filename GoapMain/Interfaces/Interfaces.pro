#-------------------------------------------------
#
# Project created by QtCreator 2017-06-02T09:20:34
#
#-------------------------------------------------

QT       -= core gui

TARGET = Interfaces
TEMPLATE = subdirs # leave SUBDIRS variable unassigned

HEADERS += \
    iroot.h \
    irefcounter.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
