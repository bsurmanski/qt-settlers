/* 
 * File:   main.cpp
 * Author: brandon
 *
 *
 * Note to self, next time create a registry list for drawing, so one call to register(gameComponent) will add it to the stack, 
 * and a call to main::Draw() will draw all the registered, or main::Update() will update game logic.
 * Have different lists for different global game states?
 * 
 * Quick TODO: networking, menus
 * 
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
