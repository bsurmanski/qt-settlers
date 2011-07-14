/* 
 * File:   Seaport.h
 * Author: brandon
 *
 * Created on May 13, 2011, 9:20 PM
 */

#ifndef SEAPORT_H
#define	SEAPORT_H
#include "GamePiece.h"
class Tile;

class Seaport : GamePiece{
public:
    Seaport();
    Seaport(Tile* tile, int direction, Vector3f color);
    Seaport(const Seaport& orig);
    virtual ~Seaport();
    
    int getType();
    
    void Draw();
       void glGenerateDisplayList();
    
private:
    int type;
    float rotation;
};

#endif	/* SEAPORT_H */

