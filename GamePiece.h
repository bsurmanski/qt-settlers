/* 
 * File:   GamePiece.h
 * Author: brandon
 *
 * Created on May 2, 2011, 8:04 PM
 */

#ifndef GAMEPIECE_H
#define	GAMEPIECE_H
#include "Model.h"

class GamePiece {
public:
    GamePiece();
    GamePiece(const GamePiece& orig);
    virtual ~GamePiece();
    
    Vector3f getPosition();
    
    virtual void Draw() = 0;
//protected:
    Model* model;
    Vector3f color;
    Vector3f position;

};

#endif	/* GAMEPIECE_H */

