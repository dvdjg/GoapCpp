TARGET = mylzma
QT       -= core gui

TEMPLATE = lib
CONFIG+= staticlib

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

win32-msvc* {
QMAKE_CXXFLAGS+= -openmp
QMAKE_LFLAGS +=  -openmp
} else {
QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
DEFINES += _7ZIP_ST
}

SOURCES += \
    LzFind.c \
    LzmaDec.c \
    LzmaEnc.c

HEADERS += \
    lzma.h \
    7zVersion.h \
    base.h \
    bcj.h \
    block.h \
    check.h \
    container.h \
    delta.h \
    filter.h \
    hardware.h \
    index_hash.h \
    index.h \
    LzFind.h \
    LzHash.h \
    lzma.h \
    LzmaDec.h \
    LzmaEnc.h \
    stream_flags.h \
    Types.h \
    version.h \
    vli.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
