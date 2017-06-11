TEMPLATE = app
CONFIG += console
CONFIG += static
CONFIG -= app_bundle
CONFIG -= qt

TARGET = tst_factorytesttest

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += \
    tst_factorytesttest.cpp \
    main.cpp \
    tst_signals.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += $$top_srcdir/../Factory $$top_srcdir/../Interfaces $$top_srcdir/../GoapLib $$top_srcdir/../sqlite_modern_cpp
DEPENDPATH += $$top_srcdir/../Factory $$top_srcdir/../Interfaces $$top_srcdir/../GoapLib $$top_srcdir/../sqlite_modern_cpp

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/release/libGoapLib.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/debug/libGoapLib.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/release/GoapLib.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/debug/GoapLib.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../GoapLib/libGoapLib.a

LIBS += \
    -lGoapLib$$SUFFIX \
    -lbotan$$SUFFIX

LIBS += -lgtest$$SUFFIX -lgmock

