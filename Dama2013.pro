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

#QMAKE_CXX = /usr/local/bin/g++-4.7
#QMAKE_CXXFLAGS += -std=c++11

TARGET = dama2013
TEMPLATE = app


SOURCES += main.cpp\
    draughts.cpp \
    stone.cpp \
    board.cpp \
    stroke.cpp \
    client.cpp \
    server.cpp \
    about.cpp \
    new_game.cpp \
    ngame_demand.cpp \
    settings.cpp \
    sure_about.cpp \
    whether_replay.cpp \
    dama.cpp \
    game.cpp \
    insert_gamecode.cpp

HEADERS  += \
    draughts.h \
    stone.h \
    board.h \
    stroke.h \
    client.h \
    server.h \
    about.h \
    new_game.h \
    ngame_demand.h \
    settings.h \
    sure_about.h \
    whether_replay.h \
    dama.h \
    game.h \
    insert_gamecode.h

FORMS    += \
    dama.ui \
    settings.ui \
    new_game.ui \
    about.ui \
    ngame_demand.ui \
    sure_about.ui \
    whether_replay.ui \
    game.ui \
    insert_gamecode.ui

OTHER_FILES += \
    dama-cli.txt

RESOURCES += \
    resource1.qrc

TRANSLATIONS = dama_sk.ts \
               dama_cs.ts

CODECFORTR = UTF-8
