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
DEPENDPATH += $$INCLUDEPATH \
     $$top_srcdir/3rdparty/botan

PRE_TARGETDEPS += \
    $$LIBDIR/$${LIBPRE}botan$$SUFFIX$$LIBPOST \
    $$LIBDIR/$${LIBPRE}z$$SUFFIX$$LIBPOST \
    $$LIBDIR/$${LIBPRE}lzma$$SUFFIX$$LIBPOST \
    $$LIBDIR/$${LIBPRE}sqlite$$SUFFIX$$LIBPOST

LIBS += \
    -lsqlite$$SUFFIX \
    -lbotan$$SUFFIX \
    -lz$$SUFFIX \
    -llzma$$SUFFIX

win32-g++:LIBS += \
         -lboost_system-mgw73-mt-x64-1_71 -lboost_filesystem-mgw73-mt-x64-1_71 \
         -lUser32 -lAdvapi32 -lws2_32

app.depends = \
    sqlite$$SUFFIX \
    botan$$SUFFIX \
    z$$SUFFIX \
    lzma$$SUFFIX

unix {
    LIBS += -lbz2 -ldl
} else:*-g++ {
} else:win32-msvc* {
}

