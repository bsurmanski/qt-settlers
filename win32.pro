# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/WIN32
TARGET = Settlers
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
QT = core gui
SOURCES += Sprite.cpp main.cpp Seaport.cpp Robber.cpp Model.cpp Player.cpp vmath.cpp TradeOffer.cpp GLFrame.cpp PauseMenu.cpp Menu.cpp DevelopmentCard.cpp Settlement.cpp Road.cpp GamePiece.cpp Tile.cpp TradeMenu.cpp Board.cpp GameLibrary.cpp
HEADERS += Robber.h Board.h Sprite.h vmath.h PauseMenu.h Settlement.h Road.h Menu.h Player.h Seaport.h TradeOffer.h GamePiece.h Tile.h Model.h GameWindow.h GLFrame.h Button.h TradeMenu.h GameLibrary.h GameDockWidget.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/WIN32
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += 
