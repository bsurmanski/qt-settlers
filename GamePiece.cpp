/* 
 * File:   GamePiece.cpp
 * Author: brandon
 * 
 * Created on May 2, 2011, 8:04 PM
 */

#include "GamePiece.h"
#include <QtOpenGL/QtOpenGL>

GamePiece::GamePiece() {
    model = NULL;
}

Vector3f GamePiece::getPosition(){
    return position;
}

GamePiece::~GamePiece() {
}

