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

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += distributedpersistence.cpp

HEADERS += distributedpersistence.h


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../GoapLib/release/ -lGoapLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../GoapLib/debug/ -lGoapLib
else:unix: LIBS += -L$$OUT_PWD/../GoapLib/ -lGoapLib

INCLUDEPATH += $$PWD/../Factory $$PWD/../Interfaces $$PWD/../GoapLib $$PWD/../sqlite_modern_cpp $$PWD/../sqlite-amalgamation
DEPENDPATH += $$PWD/../Factory $$PWD/../Interfaces $$PWD/../GoapLib $$PWD/../sqlite_modern_cpp $$PWD/../sqlite-amalgamation

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GoapLib/release/libGoapLib.a $$OUT_PWD/../sqlite-amalgamation/release/libsqlite-amalgamation.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GoapLib/debug/libGoapLib.a $$OUT_PWD/../sqlite-amalgamation/debug/libsqlite-amalgamation.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GoapLib/release/GoapLib.lib $$OUT_PWD/../sqlite-amalgamation/release/sqlite-amalgamation.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GoapLib/debug/GoapLib.lib $$OUT_PWD/../sqlite-amalgamation/debug/sqlite-amalgamation.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../GoapLib/libGoapLib.a $$OUT_PWD/../sqlite-amalgamation/libsqlite-amalgamation.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../sqlite-amalgamation/release/ -lsqlite-amalgamation
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../sqlite-amalgamation/debug/ -lsqlite-amalgamation
else:unix: LIBS += -L$$OUT_PWD/../sqlite-amalgamation/ -lsqlite-amalgamation

unix {
    target.path = /usr/lib
    INSTALLS += target
}
