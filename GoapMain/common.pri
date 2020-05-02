CONFIG += c++14

CONFIG(debug, debug|release){
    SUFFIX=d
    CONFDIR=debug
} else {
    SUFFIX=
    CONFDIR=release
}

TARGET=$$TARGET$$SUFFIX

DEFINES += SRCDIR=\\\"$$top_srcdir/\\\"

DEFINES += HAS_BOOST_SMART_INTRUSIVE_PTR QT_DEPRECATED_WARNINGS _GNU_SOURCE \
    _MODERN_SQLITE_BOOST_OPTIONAL_SUPPORT SQLITE_ENABLE_FTS SQLITE_ENABLE_RTREE SQLITE_ENABLE_COLUMN_METADATA \
    SQLITE_ENABLE_MEMORY_MANAGEMENT SQLITE_ALLOW_URI_AUTHORITY SQLITE_ENABLE_API_ARMOR SQLITE_ENABLE_COLUMN_METADATA SQLITE_ENABLE_DBSTAT_VTAB \
    SQLITE_ENABLE_EXPLAIN_COMMENTS SQLITE_ENABLE_FTS5 SQLITE_ENABLE_JSON1 SQLITE_ENABLE_PREUPDATE_HOOK SQLITE_ENABLE_RBU \
    SQLITE_ENABLE_SESSION SQLITE_ENABLE_STAT4 SQLITE_ENABLE_UPDATE_DELETE_LIMIT SQLITE_ENABLE_UNLOCK_NOTIFY \
    SQLITE_SOUNDEX SQLITE_USE_FCNTL_TRAC SQLITE_THREADSAFE SQLITE_HAS_CODEC  # SQLITE_ENABLE_SQLLOG SQLITE_OMIT_LOAD_EXTENSION SQLITE_ENABLE_ICU

DEFINES -= UNICODE _UNICODE

win32-*:DEFINES += \
    WIN32_LEAN_AND_MEAN \
    NOMINMAX \
    VC_EXTRALEAN \
    _SCL_SECURE_NO_DEPRECATE \
    _CRT_SECURE_NO_DEPRECATE \
    _CRT_SECURE_NO_WARNINGS \
    _CRT_NONSTDC_NO_DEPRECATE \
    _AFX_SECURE_NO_WARNINGS \
    _ATL_SECURE_NO_WARNINGS \
    _WIN32_WINNT=0x0501

DEFINES += HAVE_ZLIB
INCLUDEPATH += $$top_srcdir/3rdparty/zlib

*-g++:QMAKE_TARGET.arch = $$QMAKE_HOST.arch
*-g++-32:QMAKE_TARGET.arch = x86
*-g++-64:QMAKE_TARGET.arch = x86_64

SPEC=$$basename(QMAKESPEC)
SPEC=$$replace(SPEC, win32, windows)-$$QMAKE_TARGET.arch

#       "D:/OneDrive/Programa/googletest/googletest/include" \
#       "D:/OneDrive/Programa/googletest/googlemock/include"
#       "F:/Programa/googletest/googletest/include" \
#       "F:/Programa/googletest/googlemock/include"
win32-msvc* {
    LIBPRE=
    LIBPOST=.lib
    INCLUDEPATH += \
        D:\Programa\boost_1_71_0
    LIBS += \
        -LD:\Programa\boost_1_71_0/stage/lib \
         -lUser32 -lAdvapi32 -lws2_32
    QMAKE_CXXFLAGS +=  -openmp
    QMAKE_CFLAGS += -openmp
    POST_DESTDIR=msvc
    DEFINES += COMPILER_MSVC
} else:win32-g++ {
    LIBPRE=lib
    LIBPOST=.a
    INCLUDEPATH += \
        F:\Programa\boost_1_71_0
    LIBS += \
        -LF:\Programa\boost_1_71_0/stage/lib \
         -lboost_system-mgw73-mt-x64-1_71 -lboost_filesystem-mgw73-mt-x64-1_71 \
         -lUser32 -lAdvapi32 -lws2_32
    QMAKE_CXXFLAGS+= -pthread -fno-strict-aliasing -fopenmp -Wno-comment
    QMAKE_CFLAGS  += -pthread -fno-strict-aliasing -fopenmp -Wno-comment
    QMAKE_LFLAGS  += -pthread -fopenmp
    POST_DESTDIR=mingw
    DEFINES += COMPILER_MINGW
} else:unix {
    LIBPRE=lib
    LIBPOST=.a
    INCLUDEPATH += /opt/arrayfire-3/include
    LIBS += -L /opt/arrayfire-3/lib -laf \
        -lboost_system -lboost_filesystem
    QMAKE_CXXFLAGS+= -pthread -fno-strict-aliasing -fopenmp -Wno-comment
    QMAKE_CFLAGS  += -pthread -fno-strict-aliasing -fopenmp -Wno-comment
    QMAKE_LFLAGS  += -pthread -fopenmp
    POST_DESTDIR=unix
    DEFINES += COMPILER_GCC
}


LIBDIR=$$top_srcdir/lib_$$POST_DESTDIR
staticlib {
    DEFINES += STATIC LZMA_API_STATIC
    DESTDIRCOMMON=$$LIBDIR
} else {
    static:DEFINES += STATIC LZMA_API_STATIC
    # DESTDIRCOMMON and dynamic libs goes to bin
    DESTDIRCOMMON=$$top_srcdir/bin_$$POST_DESTDIR
}

LIBS += -L$$LIBDIR -L$$LIBDIR/$$SPEC -L$$LIBDIR/$$SPEC/$$CONFDIR

DESTDIR=$${DESTDIRCOMMON}

