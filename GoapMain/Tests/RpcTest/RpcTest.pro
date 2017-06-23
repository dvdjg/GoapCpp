TARGET = tst_data

QT       -= gui core
TEMPLATE = app
CONFIG += console static
CONFIG -= qt app_bundle

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

!include(../../3rdparty/rpclib/rpclib.pri) {
    error(Could not find the rpclib.pri file!)
}

!include(../../3rdparty/dependencies/dependencies.pri) {
    error(Could not find the dependencies.pri file!)
}

SOURCES += \
    testmain.cc \
    rpc/client_test.cc \
    rpc/dispatcher_test.cc \
    rpc/response_test.cc \
    rpc/server_session_test.cc \
    rpc/server_test.cc \
    rpc/this_handler_test.cc \
    rpc/this_server_test.cc \
    rpc/this_session_test.cc


INCLUDEPATH += $$top_srcdir/3rdparty/gmock-gtest # $$top_srcdir/3rdparty/dependencies/include $$top_srcdir/3rdparty/rpclib/include $$top_srcdir/3rdparty/rpclib/include/rpc
DEPENDPATH += $$INCLUDEPATH

PRE_TARGETDEPS += \
    $$top_srcdir/lib/$${LIBPRE}rpc$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}dependencies$$SUFFIX$$LIBPOST

LIBS += \
    -lrpc$$SUFFIX \
    -lgmock-gtest$$SUFFIX \
    -ldependencies$$SUFFIX

unix {
QMAKE_CXXFLAGS += -pthread -fno-strict-aliasing
QMAKE_CFLAGS += -pthread -fno-strict-aliasing
QMAKE_LFLAGS += -pthread -fopenmp
} else:win32-msvc* {
LIBS += \
    -lUser32 -lAdvapi32
}

HEADERS += \
    testutils.h
