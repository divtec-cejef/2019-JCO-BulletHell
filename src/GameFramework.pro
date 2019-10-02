#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T13:38:06
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GameFramework
TEMPLATE = app

#DEFINES += DEBUG_SPRITE_COUNT
#DEFINES += DEBUG_BBOX
#DEFINES += DEBUG_SHAPE

SOURCES += main.cpp\
        mainfrm.cpp \
    gamescene.cpp \
    sprite.cpp \
    gamecore.cpp \
    resources.cpp \
    gameview.cpp \
    utilities.cpp \
    gamecanvas.cpp \
    walkingman.cpp \
    blueball.cpp \
    spinningpinwheel.cpp \
    spritetickhandler.cpp \
    bouncingspritehandler.cpp \
    manualwalkinghandler.cpp \
    automaticwalkinghandler.cpp

HEADERS  += mainfrm.h \
    gamescene.h \
    sprite.h \
    gamecore.h \
    resources.h \
    gameview.h \
    utilities.h \
    gamecanvas.h \
    walkingman.h \
    blueball.h \
    spinningpinwheel.h \
    spritetickhandler.h \
    bouncingspritehandler.h \
    manualwalkinghandler.h \
    automaticwalkinghandler.h

FORMS    += mainfrm.ui
