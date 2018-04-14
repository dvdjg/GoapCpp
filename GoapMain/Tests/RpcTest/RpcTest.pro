TARGET = tst_rpc

QT       -= gui core
TEMPLATE = app
CONFIG += console static
CONFIG -= qt app_bundle

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

!include($$top_srcdir/3rdparty/rpclib/rpclib.pri) {
    error(Could not find the rpclib.pri file!)
}

!include($$top_srcdir/3rdparty/dependencies/dependencies.pri) {
    error(Could not find the dependencies.pri file!)
}

HEADERS += \
    testutils.h

SOURCES += \
    testmain.cc \
    rpc/response_test.cc \
    rpc/server_session_test.cc \
    rpc/server_test.cc \
    rpc/this_handler_test.cc \
    rpc/this_server_test.cc \
    rpc/this_session_test.cc
#    rpc/client_test.cc \
#    rpc/dispatcher_test.cc \


INCLUDEPATH += $$top_srcdir/3rdparty/gmock-gtest
DEPENDPATH += $$INCLUDEPATH

PRE_TARGETDEPS += \
    $$top_srcdir/lib/$${LIBPRE}rpc$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}dependencies$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}gmock-gtest$$SUFFIX$$LIBPOST

POSTLIBS = $$LIBS
LIBS += \
    -lrpc$$SUFFIX \
    -ldependencies$$SUFFIX \
    -lgmock-gtest$$SUFFIX \
    $$POSTLIBS
win32-g++:LIBS += -lMswsock


