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
    $$top_srcdir/lib/$${LIBPRE}rpc$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}dependencies$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}gmock-gtest$$SUFFIX$$LIBPOST

POSTLIBS = $$LIBS
LIBS += \
    -lrpc$$SUFFIX \
    -ldependencies$$SUFFIX \
    $$POSTLIBS
win32-g++:LIBS += -lMswsock
