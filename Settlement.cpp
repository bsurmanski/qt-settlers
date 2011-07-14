/* 
 * File:   Settlement.cpp
 * Author: brandon
 * 
 * Created on May 2, 2011, 8:02 PM
 */

#include "Settlement.h"

Settlement::Settlement() {
}

Settlement::Settlement(Tile::CornerNode* corner, Vector3f color) {
    this->corner = corner;
    city = false;
    this->position = corner->getPosition();
    model = GameLibrary::getInstance()->getCachedModel("settlement.mdl");
    //model = new Model("settlement.mdl");
    this->color = color;
    //model->changeColor(color);
}

Settlement::~Settlement() {
}

void Settlement::Draw() {
    glPushMatrix();
    glColor3f(color.r, color.g, color.b);
    glTranslatef(position.x, position.y, position.z);
    model->Draw();
    glPopMatrix();
}

void Settlement::upgradeToCity() {
    model = GameLibrary::getInstance()->getCachedModel("city.mdl");
    city = true;
}

bool Settlement::tileIsOn(int roll) {
    return corner->cornerIsOn(roll);
}

Tile::CornerNode* Settlement::getCorner() {
    return corner;
}

bool Settlement::isCity() {
    return city;
}

Vector2i Settlement::collectionFromTile(int roll, int tile) {
    if (corner->getResource(tile) < 0) {
        return Vector2i();
    }
    if (corner->getTileToken(tile) == roll) {
        return Vector2i(corner->getResource(tile), city ? 2 : 1);
    }
    return Vector2i();
}

