CONFIG += c++11

# in a new gcc_64/mkspecs/linux-g++-64v5:
# QMAKE_CC = gcc-5
# QMAKE_CXX = g++-5

DEFINES += _MODERN_SQLITE_BOOST_OPTIONAL_SUPPORT

win32 {
    INCLUDEPATH += D:/local/boost_1_64_0
    INCLUDEPATH += D:/OneDrive/Programa/GoapCpp/GoapMain/include
    LIBS += -L D:/OneDrive/Programa/GoapCpp/GoapMain/lib/windows-g++-x86
#    LIBS += -LC:/boost/lib \
#            -lboost_serialization-mgw46-mt-d-1_54 \
#            -lboost_filesystem-mgw46-mt-d-1_54 \
#            -lboost_system-mgw46-mt-d-1_54
}
unix {
    INCLUDEPATH += /opt/arrayfire-3/include
    LIBS += -L /opt/arrayfire-3/lib -laf
}
