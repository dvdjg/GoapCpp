CONFIG += c++14

# in a new gcc_64/mkspecs/linux-g++-64v5:
# QMAKE_CC = gcc-5
# QMAKE_CXX = g++-5

DEFINES +=

CONFIG(debug, debug|release){
    SUFFIX=d
    CONFDIR=debug
} else {
    SUFFIX=
    CONFDIR=release
}

TARGET=$$TARGET$$SUFFIX

DEFINES += SRCDIR=\\\"$$top_srcdir/\\\"

DEFINES += QT_DEPRECATED_WARNINGS \
    _MODERN_SQLITE_BOOST_OPTIONAL_SUPPORT SQLITE_ENABLE_FTS SQLITE_ENABLE_RTREE SQLITE_ENABLE_COLUMN_METADATA \
    SQLITE_ENABLE_MEMORY_MANAGEMENT SQLITE_ALLOW_URI_AUTHORITY SQLITE_ENABLE_API_ARMOR SQLITE_ENABLE_COLUMN_METADATA SQLITE_ENABLE_DBSTAT_VTAB \
    SQLITE_ENABLE_EXPLAIN_COMMENTS SQLITE_ENABLE_FTS5 SQLITE_ENABLE_JSON1 SQLITE_ENABLE_PREUPDATE_HOOK SQLITE_ENABLE_RBU \
    SQLITE_ENABLE_SESSION SQLITE_ENABLE_SQLLOG SQLITE_ENABLE_STAT4 SQLITE_ENABLE_UPDATE_DELETE_LIMIT SQLITE_ENABLE_UNLOCK_NOTIFY \
    SQLITE_SOUNDEX SQLITE_USE_FCNTL_TRAC SQLITE_THREADSAFE=1 SQLITE_HAS_CODEC _CRT_SECURE_NO_WARNINGS # SQLITE_OMIT_LOAD_EXTENSION SQLITE_ENABLE_ICU

*-g++:QMAKE_TARGET.arch = $$QMAKE_HOST.arch
*-g++-32:QMAKE_TARGET.arch = x86
*-g++-64:QMAKE_TARGET.arch = x86_64

SPEC=$$basename(QMAKESPEC)
SPEC=$$replace(SPEC, win32, windows)-$$QMAKE_TARGET.arch

LIBDIR=$$top_srcdir/lib
staticlib {
    DEFINES += STATIC
    DESTDIRCOMMON=$$LIBDIR
} else {
    # DESTDIRCOMMON and dynamic libs goes to bin
    DESTDIRCOMMON=$$top_srcdir/bin
}

DESTDIR=$$DESTDIRCOMMON

LIBS += -L$$LIBDIR -L$$LIBDIR/$$SPEC -L$$LIBDIR/$$SPEC/$$CONFDIR

win32 {
    INCLUDEPATH += \
        D:/local/boost_1_64_0 \
        "C:/Program Files/ArrayFire/v3/include" \
        "D:/OneDrive/Programa/googletest/googletest/include" \
        "D:/OneDrive/Programa/googletest/googlemock/include"

    win32-msvc*:LIBS += -L"C:/Program Files/ArrayFire/v3/lib" -laf
}
unix {
    INCLUDEPATH += /opt/arrayfire-3/include
    LIBS += -L /opt/arrayfire-3/lib -laf
}
