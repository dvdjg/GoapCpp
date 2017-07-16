TARGET = z
QT       -= core gui

TEMPLATE = lib
CONFIG += staticlib
CONFIG += exceptions
CONFIG -= rtti

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

DEFINES += HAVE_MEMCPY HAVE_STDARG_H

win32 {
DEFINES += _WIN32_WINNT=0x0501
DEFINES += _SCL_SECURE_NO_DEPRECATE _CRT_SECURE_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE _AFX_SECURE_NO_WARNINGS _ATL_SECURE_NO_WARNINGS
# _MBCS
#DEFINES -= UNICODE _UNICODE
} else {
DEFINES += HAVE_UNISTD_H
}

HEADERS += \
    zlib.h \
    zutil.h \
    zlib.h \
    zip.h \
    zconf.in.h \
    zconf.h \
    unzip.h \
    trees.h \
    ioapi.h \
    inftrees.h \
    inflate.h \
    inffixed.h \
    inffast.h \
    gzguts.h \
    deflate.h \
    crypt.h \
    crc32.h \
    mztools.h

SOURCES += \
    zutil.c \
    zip.c \
    unzip.c \
    uncompr.c \
    trees.c \
    minigzip.c \
    ioapi.c \
    inftrees.c \
    inflate.c \
    inffast.c \
    infback.c \
    gzwrite.c \
    gzread.c \
    gzlib.c \
    gzclose.c \
    deflate.c \
    crc32.c \
    compress.c \
    adler32.c \
    mztools.c

unix {
    target.path = /usr/lib
    INSTALLS += target
}
