/* 
 * File:   Road.h
 * Author: brandon
 *
 * Created on May 2, 2011, 8:01 PM
 */

#ifndef ROAD_H
#define	ROAD_H
#include "GamePiece.h"
#include "Tile.h"
#include <QtCore/QList>
//class Tile;
//class Tile::CornerNode;

class Road : public GamePiece{
public:
    Road();
    Road(Tile::CornerNode* node, Tile::CornerNode* node2, Vector3f color);
    Road(const Road& orig);
    virtual ~Road();
    int getLongestRoad(QList<Road*>* roadList = NULL, Tile::CornerNode* node = NULL);
    float getRotation(int direction, Tile::CornerNode* node);
    void setIsBridge();
    void setIsBoat();
    bool getIsBridge();
    bool getIsBoat();
    Vector3f getPlayerColor();
    
    void Draw();
private:
    bool roadVisited (Road** list, int length, Road* node);
    
    Tile::CornerNode** nodes;
    //Vector3f color;
    int direction;
    float rotation;

    bool isBridge;
    bool isBoat;
    
    bool visited;
};

#endif	/* ROAD_H */

