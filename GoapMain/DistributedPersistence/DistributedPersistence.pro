#-------------------------------------------------
#
# Project created by QtCreator 2017-06-02T09:02:28
#
#-------------------------------------------------

QT       -= core gui

TARGET = DistributedPersistence
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14

!include(../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += distributedpersistence.cpp

HEADERS += distributedpersistence.h

INCLUDEPATH += $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/3rdparty/sqlite_modern_cpp $$top_srcdir/3rdparty/sqlite
DEPENDPATH += $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/3rdparty/sqlite_modern_cpp $$top_srcdir/3rdparty/sqlite

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GoapLib/release/libGoapLib.a $$OUT_PWD/../sqlite/release/libsqlite$$SUFFIX.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GoapLib/debug/libGoapLib.a $$OUT_PWD/../sqlite/debug/libsqlite$$SUFFIX.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GoapLib/release/GoapLib.lib $$OUT_PWD/../sqlite/release/sqlite$$SUFFIX.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GoapLib/debug/GoapLib.lib $$OUT_PWD/../sqlite/debug/sqlite$$SUFFIX.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../GoapLib/libGoapLib.a $$OUT_PWD/../sqlite/libsqlite.a

LIBS += \
    -lsqlite$$SUFFIX \
    -lGoapLib$$SUFFIX

unix {
    target.path = /usr/lib
    INSTALLS += target
}
