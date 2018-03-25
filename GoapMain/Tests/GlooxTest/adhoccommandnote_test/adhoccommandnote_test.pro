TARGET = tst_gloox_adhoccommandnote

include(../GlooxTest.pri)

SOURCES += ../main.cpp adhoccommandnote_test.cpp

LIBS += \
    -lgloox$$SUFFIX \
    -lz$$SUFFIX
