#-------------------------------------------------
#
# Project created by QtCreator 2017-06-06T00:08:53
#
#-------------------------------------------------

QT       -= core gui

TEMPLATE = subdirs # leave SUBDIRS variable unassigned

TARGET = sqlite_modern_cpp

INCLUDEPATH += $$top_srcdir/3rdparty/sqlite
DEPENDPATH += $$top_srcdir/3rdparty/sqlite

SOURCES +=

HEADERS += \
    sqlite_modern_cpp.h \
    sqlite_modern_cpp/errors.h \
    sqlite_modern_cpp/log.h \
    sqlite_modern_cpp/sqlcipher.h \
    sqlite_modern_cpp/lists/error_codes.h \
    sqlite_modern_cpp/utility/function_traits.h \
    sqlite_modern_cpp/utility/uncaught_exceptions.h \
    sqlite_modern_cpp/utility/variant.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
