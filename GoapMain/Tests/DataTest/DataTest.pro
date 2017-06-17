TARGET = tst_data

QT       -= gui core
TEMPLATE = app
CONFIG += console static
CONFIG -= qt app_bundle


!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += \
    tst_data.cpp \
    main.cpp \
    tst_sqlite_encrypted.cpp \
    tst_sqlite_modern.cpp

INCLUDEPATH += $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/3rdparty/sqlite_modern_cpp $$top_srcdir/3rdparty/sqlite
DEPENDPATH += $$top_srcdir/Factory $$top_srcdir/Interfaces $$top_srcdir/GoapLib $$top_srcdir/3rdparty/sqlite_modern_cpp $$top_srcdir/3rdparty/sqlite

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/release/libGoapLib.a $$OUT_PWD/../../sqlite/release/libsqlite.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/debug/libGoapLib.a $$OUT_PWD/../../sqlite/debug/libsqlite.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/release/GoapLib.lib $$OUT_PWD/../../sqlite/release/sqlite.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/debug/GoapLib.lib $$OUT_PWD/../../sqlite/debug/sqlite.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/libGoapLib.a $$OUT_PWD/../../sqlite/libsqlite.a

LIBS += \
    -lbotan$$SUFFIX \
    -lsqlite$$SUFFIX \
    -lGoapLib$$SUFFIX \
    -lz$$SUFFIX

unix {
QMAKE_CXXFLAGS += -pthread -fno-strict-aliasing
QMAKE_CFLAGS += -pthread -fno-strict-aliasing
QMAKE_LFLAGS += -pthread -fopenmp
LIBS += -lgtest -lgmock
LIBS += -lbz2 -llzma -ldl
LIBS +=  -lboost_system -lboost_filesystem
} else:win32-msvc* {
LIBS += \
    -lgtest$$SUFFIX \
    -lgmock$$SUFFIX \
    -lUser32 -lAdvapi32
}
