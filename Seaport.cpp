/* 
 * File:   Seaport.cpp
 * Author: brandon
 * 
 * Created on May 13, 2011, 9:20 PM
 */

#include "Seaport.h"
#include "Tile.h"

Seaport::Seaport() {
}

Seaport::Seaport(Tile* tile, int direction, Vector3f color) {
    model = new Model("anchor.mdl");
    model->changeColor(color);
    position = tile->getPosition();
    rotation = (-direction * PI / 3);
    type = GameLibrary::getInstance()->getResourceForColor(color);
}

Seaport::~Seaport() {
}

void Seaport::Draw() {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation, 0, 0, 1);
    model->Draw();
    glPopMatrix();
}

int Seaport::getType() {
    return type;
}

void Seaport::glGenerateDisplayList() {
    Matrix3f rotationMat = Matrix3f();
    rotationMat = rotationMat.createRotationAroundAxis(0, 0, rotation);
    model->glGenerateDisplayList(position, rotationMat);
}




