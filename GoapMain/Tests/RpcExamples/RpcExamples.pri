QT -= qt
CONFIG += console
CONFIG += static
CONFIG -= app_bundle

TEMPLATE = app

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

!include($$top_srcdir/3rdparty/rpclib/rpclib.pri) {
    error(Could not find the rpclib.pri file!)
}

!include($$top_srcdir/3rdparty/dependencies/dependencies.pri) {
    error(Could not find the dependencies.pri file!)
}


DEPENDPATH += $$INCLUDEPATH

PRE_TARGETDEPS += \
    $$LIBDIR/$${LIBPRE}rpc$$SUFFIX$$LIBPOST \
    $$LIBDIR/$${LIBPRE}dependencies$$SUFFIX$$LIBPOST \
    $$LIBDIR/$${LIBPRE}gmock-gtest$$SUFFIX$$LIBPOST

POSTLIBS = $$LIBS
LIBS += \
    -lrpc$$SUFFIX \
    -ldependencies$$SUFFIX \
    $$POSTLIBS
win32-g++:LIBS += -lMswsock
