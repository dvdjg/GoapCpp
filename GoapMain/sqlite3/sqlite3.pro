TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

DEFINES +=

SOURCES += \
    shell.c

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../sqlite-amalgamation/release/ -lsqlite-amalgamation
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../sqlite-amalgamation/debug/ -lsqlite-amalgamation
else:unix: LIBS += -L$$OUT_PWD/../sqlite-amalgamation/ -lsqlite-amalgamation

INCLUDEPATH += $$PWD/../sqlite-amalgamation
DEPENDPATH += $$PWD/../sqlite-amalgamation

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite-amalgamation/release/libsqlite-amalgamation.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite-amalgamation/debug/libsqlite-amalgamation.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite-amalgamation/release/sqlite-amalgamation.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite-amalgamation/debug/sqlite-amalgamation.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../sqlite-amalgamation/libsqlite-amalgamation.a

unix {
QMAKE_CXXFLAGS += -pthread
QMAKE_CFLAGS += -pthread -fno-strict-aliasing
LIBS += -pthread -ldl
}
