# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/GNU-Linux-x86
TARGET = Settlers
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
QT = core gui network opengl
SOURCES += SettingsWidget.cpp Sprite.cpp GameDockWidget.cpp main.cpp NetworkManager.cpp Seaport.cpp StatisticsWindow.cpp Robber.cpp GameLibrary.cpp Model.cpp Player.cpp vmath.cpp TradeOffer.cpp GLFrame.cpp PauseMenu.cpp DevelopmentCard.cpp Settlement.cpp MainGameWindow.cpp Road.cpp GamePiece.cpp Tile.cpp YearOfPlentyDialog.cpp TradeMenu.cpp Board.cpp MainMenu.cpp
HEADERS += Robber.h Board.h SettingsWidget.h Sprite.h vmath.h PauseMenu.h GameLibrary.h YearOfPlentyDialog.h Settlement.h Road.h Player.h GameDockWidget.h MainGameWindow.h Seaport.h TradeOffer.h GamePiece.h Tile.h NetworkManager.h Model.h MainMenu.h GameWindow.h StatisticsWindow.h GLFrame.h TradeMenu.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += 
