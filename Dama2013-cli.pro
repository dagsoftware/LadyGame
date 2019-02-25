#-------------------------------------------------
#
# Project created by QtCreator 2013-04-30T23:39:19
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#contains(QT_VERSION, ^4\\.[^7]\\..*) {
#    message("Cannot build Qt Creator with Qt version $${QT_VERSION}.")
#    error("Use at last Qt 4.7.")
#}

#QMAKE_CXXFLAGS += -std=c++11

TARGET = dama2013-cli
TEMPLATE = app


SOURCES += main-cli.cpp\
    draughts.cpp \
    stone.cpp \
    board.cpp \
    stroke.cpp \
    client.cpp \
    server.cpp \
    game-cli.cpp

HEADERS  += draughts.h \
    stone.h \
    board.h \
    stroke.h \
    client.h \
    server.h \
    game-cli.h

OTHER_FILES += \
    dama-cli.txt
