#-------------------------------------------------
#
# Project created by QtCreator 2017-06-06T00:08:53
#
#-------------------------------------------------

QT       -= core gui

TEMPLATE = subdirs # leave SUBDIRS variable unassigned

TARGET = sqlite_modern_cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$top_srcdir/sqlite-amalgamation
DEPENDPATH += $$top_srcdir/sqlite-amalgamation

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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
