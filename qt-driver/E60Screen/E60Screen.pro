#-------------------------------------------------
#
# Project created by QtCreator 2011-02-03T22:47:30
#
#-------------------------------------------------

QT       += core gui

TARGET = E60Screen
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/gfxdrivers

SOURCES += e60screenplugin.cpp \
    e60screen.cpp

HEADERS += e60screenplugin.h \
    e60screen.h \
    s3c_epd_userland.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
