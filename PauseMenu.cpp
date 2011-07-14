/* 
 * File:   PauseMenu.cpp
 * Author: brandon
 * 
 * Created on May 31, 2011, 1:21 PM
 */

#include "PauseMenu.h"

PauseMenu::PauseMenu() {
    pauseModel = new Model("PAUSE.mdl");
    rotation = Vector3f(0, 0, 0);
}

PauseMenu::~PauseMenu() {
}

bool PauseMenu::Update() {
    rotation.y++;
    if (rotation.y > 360) rotation.y = 0;

    return true;
}

void PauseMenu::Draw() {
    glPushMatrix();
    glTranslatef(0, 0, -10);
    glRotatef(rotation.x, 1, 0, 0);
    glRotatef(rotation.y, 0, 1, 0);
    glRotatef(rotation.z, 0, 0, 1);
    pauseModel->Draw();
    glPopMatrix();
}

