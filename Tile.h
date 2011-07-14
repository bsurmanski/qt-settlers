/* 
 * File:   Tile.h
 * Author: brandon
 *
 * Created on May 2, 2011, 8:00 PM
 */

#ifndef TILE_H
#define	TILE_H
#include "GameLibrary.h"
#include "GamePiece.h"

class Road;

class Tile : public GamePiece {
public:
    class CornerNode;
    Tile(Vector3f position, int chit, int resource);
    Tile(const Tile& orig);
    virtual ~Tile();
    int getToken();
    Vector3f getPosition();
    int getResource();
    bool isBlocked();
    bool hasPort();
    bool bordersPort();
    bool nextToPort();
    void addPort(int type, int direction);

    void setBorderingTile(Tile* tile, Vector3f moveDirection);
    Tile* getBorderingTile(int direction);
    CornerNode* getCorner(int corner);
    void assignUnusedNodes();
    CornerNode* occupyCorner(int corner);
    void updateConnected();

    void Draw();
    void drawNumber();

    void glGenerateDisplayList();

    class CornerNode {
    public:
        CornerNode(Tile* creator);
        ~CornerNode();
        void setOccupied();
        bool isOccupied();
        Vector3f getPosition();
        CornerNode* getSelection(int direction);
        Vector2f getVectorDirection(int direction);
        void setConnectedNode(CornerNode* node, int position);
        void setBorderringTile(Tile* tile);
        void registerRoad(Road* road, int direction);
        void setIsLeft(bool left);
        bool isLeft();
        bool cornerIsOn(int roll);
        int getTileToken(int tile);
        int getResource(int tile);

        bool isOnWater();
        bool isBridge(Tile::CornerNode* node);
        int getPort();

        Road* getRoad(int num);

    private:
        void setPosition(Vector3f position);
        Vector3f position;
        bool occupied;
        bool water;

        int port;

        Road** roads; //array to say who owns which road where. index 0 = bottom, 1 = middle, 2 = top
        bool left;
        //connected represents the other connected nodes to the current
        CornerNode** connected; //bottom, middle, top
        //tiles represents the tiles bordering the current node
        Tile** tiles; // going from bottom left CCW
        friend class Tile;
    };

private:
    CornerNode** corners; // represent the corners of a hexagon starting bottom-left, going CCW
    Tile** tiles;
    bool port;
    
    Vector3f color;

    void setBlocked(bool blocked);
    void updateConnected(int i);

    int token; //number to roll
    int resource;
    bool blocked;

    friend class Robber;
};

#endif	/* TILE_H */

