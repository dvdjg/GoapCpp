TARGET = botan
QT       -= core gui

TEMPLATE = lib
CONFIG+= staticlib

!include(../../../common.pri) {
    error(Could not find the common.pri file!)
}

#win32-msvc*:
QMAKE_CXXFLAGS += -bigobj
QMAKE_LFLAGS += -bigobj

SOURCES += \
    botan_all.cpp \
    botan_all_aesni.cpp \
    botan_all_avx2.cpp \
    botan_all_rdrand.cpp \
    botan_all_ssse3.cpp

HEADERS += \
    botan_all.h \
    botan_all_internal.h \
    pkcs11.h \
    pkcs11f.h \
    pkcs11t.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}