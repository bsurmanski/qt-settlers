/* 
 * File:   main.cpp
 * Author: brandon
 *
 * Note to self, next time create a registry list for drawing, so one call to register(gameComponent) will add it to the stack, 
 * and a call to main::Draw() will draw all the registered, or main::Update() will update game logic.
 * Have different lists for different global game states?
 * 
 * THINGS LEFT TO DO:
 * Main menu. 
 * Win Condition. 
 * Longest Road fix. 
 * Sounds/Music. 
 * Largest Army for networking. 
 * Help menu. 
 * wood/rock icons. 
 * Trades only during turns. 
 * trades across networking.
 * remove trades. 
 * Player color picking. 
 * Ports for networking. 
 * proper robber movement & stealing with robber
 * send advanced options across network
 * yay! particle effects
 */
#include <QtGui/QApplication>
#include "MainGameWindow.h"

MainGameWindow* mainWindow;

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    mainWindow = new MainGameWindow();

    mainWindow->show();
    return app.exec();
}
