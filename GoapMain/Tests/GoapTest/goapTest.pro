TARGET = tst_goap

TEMPLATE = app
CONFIG += console static
CONFIG -= app_bundle
CONFIG -= qt

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

SOURCES += \
    main.cpp \
    tst_goap.cpp \
    tst_goap_istatevalue.cpp

INCLUDEPATH += \
    $$top_srcdir/3rdparty/gmock-gtest \
    $$top_srcdir/Factory \
    $$top_srcdir/Interfaces \
    $$top_srcdir/GoapLibrary \
    $$top_srcdir/3rdparty/half \
    $$top_srcdir/3rdparty/sqlite_modern_cpp

DEPENDPATH += $$INCLUDEPATH

PRE_TARGETDEPS += \
    $$LIBDIR/$${LIBPRE}half$$SUFFIX$$LIBPOST \
    $$LIBDIR/$${LIBPRE}GoapLibrary$$SUFFIX$$LIBPOST \
    $$LIBDIR/$${LIBPRE}gmock-gtest$$SUFFIX$$LIBPOST

POSTLIBS = $$LIBS
LIBS += \
    -lGoapLibrary$$SUFFIX \
    -lhalf$$SUFFIX \
    -lgmock-gtest$$SUFFIX \
    $$POSTLIBS

DEFINES += HAS_BOOST_SMART_INTRUSIVE_PTR

