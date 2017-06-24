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
DEPENDPATH += $$INCLUDEPATH


LIBS += \
    -lsqlite$$SUFFIX \
    -lbotan$$SUFFIX \
    -lz

unix {
QMAKE_CXXFLAGS += -pthread -fno-strict-aliasing -fopenmp
QMAKE_CFLAGS += -pthread -fno-strict-aliasing -fopenmp
QMAKE_LFLAGS += -pthread -fopenmp
LIBS +=  -lboost_system -lboost_filesystem
LIBS += -lbz2 -llzma -ldl
} else:win32-msvc* {
LIBS += -lUser32 -lAdvapi32
QMAKE_CXXFLAGS += -openmp
QMAKE_CFLAGS += -openmp
}

