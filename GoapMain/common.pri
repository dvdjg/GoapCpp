CONFIG += c++14

# in a new gcc_64/mkspecs/linux-g++-64v5:
# QMAKE_CC = gcc-5
# QMAKE_CXX = g++-5

DEFINES += _MODERN_SQLITE_BOOST_OPTIONAL_SUPPORT

CONFIG(debug, debug|release){
    SUFFIX=d
    CONFDIR=debug
} else {
    SUFFIX=
    CONFDIR=release
}

TARGET=$$TARGET$$SUFFIX

DEFINES += SRCDIR=\\\"$$top_srcdir/\\\"

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
    #static:DESTDIR=$$DESTDIR-static
}

LIBDIR=$$LIBDIR/$$SPEC
DESTDIR=$$DESTDIRCOMMON

LIBS += -L$$LIBDIR -L$$LIBDIR/$$CONFDIR

win32 {
    INCLUDEPATH += \
        D:/local/boost_1_64_0 \
        "C:/Program Files/ArrayFire/v3/include" \
        "D:/OneDrive/Programa/googletest/googletest/include" \
        "D:/OneDrive/Programa/googletest/googlemock/include"

    #  D:/OneDrive/Programa/GoapCpp/GoapMain/include
    # win32-g++:LIBS += -LD:/OneDrive/Programa/GoapCpp/GoapMain/lib/windows-g++-x86
    win32-msvc*:LIBS += -L"C:/Program Files/ArrayFire/v3/lib" -laf
    #win32-msvc*:LIBS += -LD:/OneDrive/Programa/GoapCpp/GoapMain/lib/windows-msvc-mt-x86_64
    #CONFIG(release, debug|release): LIBS += -LD:/OneDrive/Programa/GoapCpp/GoapMain/lib/windows-msvc-mt-x86_64/Release
    #else:CONFIG(debug, debug|release): LIBS += -LD:/OneDrive/Programa/GoapCpp/GoapMain/lib/windows-msvc-mt-x86_64/Debug
#    LIBS += -LC:/boost/lib \
#            -lboost_serialization-mgw46-mt-d-1_54 \
#            -lboost_filesystem-mgw46-mt-d-1_54 \
#            -lboost_system-mgw46-mt-d-1_54
}
unix {
    INCLUDEPATH += /opt/arrayfire-3/include
    LIBS += -L /opt/arrayfire-3/lib -laf
}
