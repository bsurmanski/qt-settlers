/* 
 * File:   Settlement.h
 * Author: brandon
 *
 * Created on May 2, 2011, 8:02 PM
 */

#ifndef SETTLEMENT_H
#define	SETTLEMENT_H
#include "GamePiece.h"
#include "Tile.h"

class Settlement : public GamePiece {
public:
    Settlement();
    Settlement(Tile::CornerNode* corner, Vector3f color);
    Settlement(const Settlement& orig);
    virtual ~Settlement();
    bool tileIsOn(int roll);
    Tile::CornerNode* getCorner();
    Vector2i collectionFromTile(int roll, int tile);
    void upgradeToCity();
    bool isCity();

    void Draw();
private:
    //Vector3f color;
    bool city;
    Tile::CornerNode* corner;
};

#endif	/* SETTLEMENT_H */

