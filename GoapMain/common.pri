CONFIG += c++11

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
