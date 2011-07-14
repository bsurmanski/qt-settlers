/* 
 * File:   MainMenu.cpp
 * Author: brandon
 * 
 * Created on June 29, 2011, 10:39 AM
 */

#include "MainMenu.h"

MainMenu::MainMenu(QWidget* parent) : QGLWidget(parent) {
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(20);
    setFocus();
}

MainMenu::~MainMenu() {
    delete timer;
}

void MainMenu::initializeGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // This Will Clear The Background Color To Black
    glClearDepth(1.0); // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS); // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH); // Enables Smooth Color Shading
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void MainMenu::paintGL() {

}

void MainMenu::update() {

}
