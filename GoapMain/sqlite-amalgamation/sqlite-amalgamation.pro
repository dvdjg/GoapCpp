#-------------------------------------------------
#
# Project created by QtCreator 2017-06-05T22:20:14
#
#-------------------------------------------------

QT       -= core gui

TARGET = sqlite-amalgamation
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS SQLITE_ENABLE_FTS SQLITE_ENABLE_RTREE SQLITE_ENABLE_COLUMN_METADATA \
    SQLITE_ENABLE_MEMORY_MANAGEMENT SQLITE_ALLOW_URI_AUTHORITY SQLITE_ENABLE_API_ARMOR SQLITE_ENABLE_COLUMN_METADATA SQLITE_ENABLE_DBSTAT_VTAB \
    SQLITE_ENABLE_EXPLAIN_COMMENTS SQLITE_ENABLE_FTS5 SQLITE_ENABLE_JSON1 SQLITE_ENABLE_PREUPDATE_HOOK SQLITE_ENABLE_RBU \
    SQLITE_ENABLE_SESSION SQLITE_ENABLE_SQLLOG SQLITE_ENABLE_STAT4 SQLITE_ENABLE_UPDATE_DELETE_LIMIT SQLITE_ENABLE_UNLOCK_NOTIFY \
    SQLITE_SOUNDEX SQLITE_USE_FCNTL_TRAC SQLITE_THREADSAFE=1 _CRT_SECURE_NO_WARNINGS # SQLITE_OMIT_LOAD_EXTENSION SQLITE_ENABLE_ICU

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32-msvc:INCLUDEPATH += D:/OneDrive/Programa/GoapCpp/GoapMain/3rdparty/botan/build_msvc/build/include
win32-msvc:LIBS += -L"D:/OneDrive/Programa/GoapCpp/GoapMain/3rdparty/botan/build_msvc" -lbotan

SOURCES += \
    sqlite3.c \
    codec.cpp \
    codecext.c

HEADERS += \
    sqlite3.h \
    sqlite3ext.h \
    codec_c_interface.h \
    codec.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
