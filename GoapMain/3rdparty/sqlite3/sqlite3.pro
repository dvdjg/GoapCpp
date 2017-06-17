TARGET = sqlite3
TEMPLATE = app
CONFIG += console
CONFIG += static
CONFIG -= app_bundle
CONFIG -= qt

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += \
    shell.c


INCLUDEPATH += $$top_srcdir/3rdparty/sqlite
DEPENDPATH += $$top_srcdir/3rdparty/sqlite

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite/release/libsqlite.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite/debug/libsqlite.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite/release/sqlite.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite/debug/sqlite.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../sqlite/libsqlite.a

LIBS += -lsqlite$$SUFFIX \
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

