/* 
 * File:   Robber.h
 * Author: brandon
 *
 * Created on May 3, 2011, 10:03 PM
 */

#ifndef ROBBER_H
#define	ROBBER_H
#include "GamePiece.h"
#include "Tile.h"

class Robber : public GamePiece{
public:
    Robber(Tile* tile);
    Robber(const Robber& orig);
    virtual ~Robber();

    void moveTo(Tile* tile);
    void Draw();
    bool Update();
    
private:
    void parabolicMove();
    Vector3f position;
    Vector3f target;
    Tile* currentTile;
};

#endif	/* ROBBER_H */

