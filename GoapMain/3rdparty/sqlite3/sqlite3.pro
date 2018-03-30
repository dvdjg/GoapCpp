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
    $$top_srcdir/lib/$${LIBPRE}botan$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}z$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}lzma$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}sqlite$$SUFFIX$$LIBPOST

LIBS += \
    -lsqlite$$SUFFIX \
    -lbotan$$SUFFIX \
    -lz$$SUFFIX \
    -llzma$$SUFFIX

unix {
    LIBS += -lbz2 -ldl
} else:*-g++ {
} else:win32-msvc* {
}

