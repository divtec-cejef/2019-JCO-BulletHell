#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T13:38:06
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia core

TARGET = GameFramework
TEMPLATE = app

#DEFINES += DEBUG_SPRITE_COUNT
#DEFINES += DEBUG_BBOX
#DEFINES += DEBUG_SHAPE
#DEFINES += DEPLOY

SOURCES += main.cpp\
        mainfrm.cpp \
    gamescene.cpp \
    sprite.cpp \
    gamecore.cpp \
    resources.cpp \
    gameview.cpp \
    utilities.cpp \
    gamecanvas.cpp \
    spritetickhandler.cpp \
    manualwalkinghandler.cpp \
    automaticwalkinghandler.cpp \
    livingentity.cpp \
    player.cpp \
    enemy.cpp \
    bullet.cpp \
    item.cpp \
    eyeofchaos.cpp \
    dummy.cpp

HEADERS  += mainfrm.h \
    gamescene.h \
    sprite.h \
    gamecore.h \
    resources.h \
    gameview.h \
    utilities.h \
    gamecanvas.h \
    spritetickhandler.h \
    manualwalkinghandler.h \
    automaticwalkinghandler.h \
    livingentity.h \
    player.h \
    enemy.h \
    bullet.h \
    item.h \
    eyeofchaos.h \
    dummy.h

FORMS    += mainfrm.ui
