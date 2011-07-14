/* 
 * File:   Robber.cpp
 * Author: brandon
 * 
 * Created on May 3, 2011, 10:03 PM
 */

#include "Robber.h"

Robber::Robber(Tile* tile) {
    position = tile->getPosition();
    target = position;
    tile->setBlocked(true);
    model = GameLibrary::getInstance()->getCachedModel("robber.mdl");
    color = Vector3f (0.5, 0.5, 0.5);
    //model->changeColor(Vector3f(0.5, 0.5, 0.5));
    currentTile = tile;
}

Robber::~Robber() {
}

void Robber::moveTo(Tile* tile) {
    currentTile->setBlocked(false);
    currentTile = tile;
    target = tile->getPosition();
    tile->setBlocked(true);
}

bool Robber::Update() {
    if (position != target) {
        position += Vector3f((target.x - position.x), (target.y - position.y), pow((target.y - position.y),2)*1+target.z-position.z)/10; //((position + target) / 2 - position).x);
        return true;
    }
    return false;
}

void Robber::Draw() {
    glPushMatrix();
    glColor3f (color.r, color.g, color.b);
    glTranslatef(position.x, position.y, position.z);
    model->Draw();
    glPopMatrix();
}

