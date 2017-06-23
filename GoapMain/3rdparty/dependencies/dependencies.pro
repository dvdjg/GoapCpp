QT       -= core gui

TARGET = dependencies
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14


!include(../../common.pri) {
    error(Could not find the common.pri file!)
}


!include(dependencies.pri) {
    error(Could not find the dependencies.pri file!)
}

SOURCES += \ 
    src/format.cc \
    src/posix.cc

HEADERS += \ 
    include/format.h \
    include/posix.h \
    include/pimpl_ptr.hh \
    include/asio.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    dependencies.pri
