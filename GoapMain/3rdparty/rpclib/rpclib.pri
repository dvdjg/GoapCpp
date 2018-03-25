
DEFINES += \
    RPCLIB_ARCH_X64 \
    ASIO_STANDALONE \
    RPCLIB_ASIO=clmdep_asio \
    RPCLIB_FMT=clmdep_fmt \
    RPCLIB_MSGPACK=clmdep_msgpack

win32-msvc*:DEFINES += \
    RPCLIB_WIN32 \
    ASIO_HAS_STD_ADDRESSOF \
    ASIO_HAS_STD_ARRAY \
    ASIO_HAS_CSTDINT \
    ASIO_HAS_STD_SHARED_PTR \
    ASIO_HAS_STD_TYPE_TRAITS

unix:DEFINES += \
    RPCLIB_LINUX

INCLUDEPATH += $$top_srcdir/3rdparty/rpclib/include $$top_srcdir/3rdparty/rpclib/include/rpc $$top_srcdir/3rdparty/dependencies/include # $$top_srcdir/3rdparty/rpclib/include
