TARGET = tst_msgpack

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
    array_ref.cpp \
    boost_fusion.cpp \
    boost_optional.cpp \
    boost_string_ref.cpp \
    boost_string_view.cpp \
    boost_variant.cpp \
    buffer.cpp \
    carray.cpp \
    cases.cpp \
    convert.cpp \
    fixint.cpp \
    fixint_c.cpp \
    iterator_cpp11.cpp \
    json.cpp \
    limit.cpp \
    msgpack_basic.cpp \
    msgpack_c.cpp \
    msgpack_container.cpp \
    msgpack_cpp11.cpp \
    msgpack_cpp17.cpp \
    msgpack_stream.cpp \
    msgpack_tuple.cpp \
    msgpack_vref.cpp \
    msgpack_x3_parse.cpp \
    object.cpp \
    object_with_zone.cpp \
    pack_unpack.cpp \
    pack_unpack_c.cpp \
    raw.cpp \
    reference.cpp \
    reference_cpp11.cpp \
    reference_wrapper_cpp11.cpp \
    shared_ptr_cpp11.cpp \
    size_equal_only.cpp \
    streaming.cpp \
    streaming_c.cpp \
    unique_ptr_cpp11.cpp \
    user_class.cpp \
    version.cpp \
    visitor.cpp \
    zone.cpp \
    testmain.cc

HEADERS += \
    test_allocator.hpp


INCLUDEPATH += $$top_srcdir/3rdparty/zlib $$top_srcdir/3rdparty/gmock-gtest
DEPENDPATH += $$INCLUDEPATH

PRE_TARGETDEPS += \
    $$top_srcdir/lib/$${LIBPRE}rpc$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}dependencies$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}gmock-gtest$$SUFFIX$$LIBPOST

LIBS += \
    -lrpc$$SUFFIX \
    -lgmock-gtest$$SUFFIX \
    -ldependencies$$SUFFIX \
    -lz$$SUFFIX

unix {
QMAKE_CXXFLAGS += -pthread -fno-strict-aliasing
QMAKE_CFLAGS += -pthread -fno-strict-aliasing
QMAKE_LFLAGS += -pthread -fopenmp
} else:win32-msvc* {
LIBS += \
    -lUser32 -lAdvapi32
}

