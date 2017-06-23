QT       -= core gui

TARGET = gmock-gtest
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14


!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

DEFINES += \
    ASIO_STANDALONE

unix:DEFINES += \

INCLUDEPATH += include # $$top_srcdir/3rdparty/rpclib/include

#unix:INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_gcc
#win32-msvc*:INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_msvc

#LIBS += -lbotan$$SUFFIX -llzma

SOURCES += \
    gmock-gtest-all.cc

HEADERS += \
    gmock/gmock.h \
    gtest/gtest.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
