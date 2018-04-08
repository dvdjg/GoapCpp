#-------------------------------------------------
#
# Project created by QtCreator 2017-06-02T09:02:28
#
#-------------------------------------------------

QT       -= core gui

TARGET = DistributedPersistence
TEMPLATE = lib
CONFIG += staticlib

!include(../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += distributedpersistence.cpp

HEADERS += distributedpersistence.h

INCLUDEPATH += $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLibrary $$top_srcdir/3rdparty/sqlite_modern_cpp $$top_srcdir/3rdparty/sqlite
DEPENDPATH += $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLibrary $$top_srcdir/3rdparty/sqlite_modern_cpp $$top_srcdir/3rdparty/sqlite

#PRE_TARGETDEPS += \
#    $$top_srcdir/lib/$${LIBPRE}GoapLibrary$$SUFFIX$$LIBPOST \
#    $$top_srcdir/lib/$${LIBPRE}sqlite$$SUFFIX$$LIBPOST
#
#LIBS += \
#    -lsqlite$$SUFFIX \
#    -lGoapLib$$SUFFIX

unix {
    target.path = /usr/lib
    INSTALLS += target
}
