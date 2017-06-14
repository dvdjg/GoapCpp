TEMPLATE = app
CONFIG += console
CONFIG += static
CONFIG -= app_bundle
CONFIG -= qt

!include(../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += \
    shell.c


INCLUDEPATH += $$top_srcdir/sqlite-amalgamation
DEPENDPATH += $$top_srcdir/sqlite-amalgamation

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite-amalgamation/release/libsqlite-amalgamation.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite-amalgamation/debug/libsqlite-amalgamation.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite-amalgamation/release/sqlite-amalgamation.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite-amalgamation/debug/sqlite-amalgamation.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../sqlite-amalgamation/libsqlite-amalgamation.a

LIBS += -lsqlite-amalgamation$$SUFFIX \
    -lbotan$$SUFFIX

unix {
QMAKE_CXXFLAGS += -pthread -fno-strict-aliasing -fopenmp
QMAKE_CFLAGS += -pthread -fno-strict-aliasing -fopenmp
QMAKE_LFLAGS += -pthread -fopenmp
LIBS +=  -lboost_system -lboost_filesystem
LIBS += -lbz2 -llzma -lz -ldl
} else:win32-msvc* {
LIBS += -lUser32 -lAdvapi32
QMAKE_CXXFLAGS += -openmp
QMAKE_CFLAGS += -openmp
QMAKE_LFLAGS += -openmp
}

