
!include(../botan.pri) {
    error(Could not find the botan.pri file!)
}

SOURCES += \
    botan_all_rdseed.cpp \
    botan_all_sha_sse4.1_ssse3.cpp \
    botan_all_sha_sse4.1.cpp

HEADERS +=

