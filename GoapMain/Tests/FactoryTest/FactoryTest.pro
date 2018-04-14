TARGET = tst_factorytesttest

TEMPLATE = app
CONFIG += console static
CONFIG -= app_bundle
CONFIG -= qt

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += \
    tst_factory_all.cpp \
    tst_factory_nocount.cpp \
    main.cpp \
    tst_signals.cpp \
    factory_registrations.cpp \
    tst_goap.cpp

INCLUDEPATH += \
    $$top_srcdir/3rdparty/gmock-gtest \
    $$top_srcdir/Factory \
    $$top_srcdir/Interfaces \
    $$top_srcdir/GoapLibrary \
    $$top_srcdir/3rdparty/half \
    $$top_srcdir/3rdparty/sqlite_modern_cpp

DEPENDPATH += $$INCLUDEPATH

PRE_TARGETDEPS += \
    $$top_srcdir/lib/$${LIBPRE}half$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}GoapLibrary$$SUFFIX$$LIBPOST \
    $$top_srcdir/lib/$${LIBPRE}gmock-gtest$$SUFFIX$$LIBPOST

POSTLIBS = $$LIBS
LIBS += \
    -lGoapLibrary$$SUFFIX \
    -lhalf$$SUFFIX \
    -lgmock-gtest$$SUFFIX \
    $$POSTLIBS

DEFINES += HAS_BOOST_SMART_INTRUSIVE_PTR

