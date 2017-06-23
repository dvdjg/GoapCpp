QT       -= core gui

TARGET = rpc
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14


!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

!include(rpclib.pri) {
    error(Could not find the rpclib.pri file!)
}

#unix:INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_gcc
#win32-msvc*:INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_msvc

#LIBS += -lbotan$$SUFFIX -llzma

SOURCES += \ 
    lib/rpc/client.cc \
    lib/rpc/dispatcher.cc \
    lib/rpc/rpc_error.cc \
    lib/rpc/server.cc \
    lib/rpc/this_handler.cc \
    lib/rpc/this_server.cc \
    lib/rpc/this_session.cc \
    lib/rpc/detail/client_error.cc \
    lib/rpc/detail/response.cc \
    lib/rpc/detail/server_session.cc

HEADERS += \ 
    include/rpc/client.h \
    include/rpc/compatibility.h \
    include/rpc/config.h \
    include/rpc/dispatcher.h \
    include/rpc/rpc_error.h \
    include/rpc/server.h \
    include/rpc/this_handler.h \
    include/rpc/this_server.h \
    include/rpc/this_session.h \
    include/rpc/version.h \
    include/rpc/msgpack.hpp \
    include/rpc/msgpack/cpp_config.hpp \
    include/rpc/msgpack/fbuffer.hpp \
    include/rpc/msgpack/iterator.hpp \
    include/rpc/msgpack/meta.hpp \
    include/rpc/msgpack/object_fwd.hpp \
    include/rpc/msgpack/object.hpp \
    include/rpc/msgpack/pack.hpp \
    include/rpc/msgpack/preprocessor.hpp \
    include/rpc/msgpack/sbuffer.hpp \
    include/rpc/msgpack/type.hpp \
    include/rpc/msgpack/unpack.hpp \
    include/rpc/msgpack/version.hpp \
    include/rpc/msgpack/versioning.hpp \
    include/rpc/msgpack/vrefbuffer.hpp \
    include/rpc/msgpack/zbuffer.hpp \
    include/rpc/msgpack/zone.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    rpclib.pri
