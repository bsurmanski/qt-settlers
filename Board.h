/* 
 * File:   Board.h
 * Author: brandon
 *
 * Created on May 2, 2011, 8:03 PM
 */

#ifndef BOARD_H
#define	BOARD_H
#include <QtOpenGL/QtOpenGL>
#include <string>
#include "GameLibrary.h"
#include "Tile.h"
#include "Robber.h"
#include "Seaport.h"

class Board {
public:
    Board();
    Board(const char* serial, int length);
    virtual ~Board();
    void Draw();
    bool Update();
    Tile* getClosestTile(Vector3f position);
    Tile::CornerNode* getCornerNode(int tile, int corner);
    int getNumberOfTiles();
    bool moveRobber(int tile);
    Tile::CornerNode* occupyCorner(int tile, int corner);
    Tile::CornerNode* occupyCorner(Tile::CornerNode* corner);
    Tile::CornerNode* getDefaultNodeSelection();
    Vector2f rollDice();
    Vector2i tileAndCornerNumber(Tile::CornerNode* node);
    int getDevCardNumber();
    void buildLists();
    std::string serialize();

private:
    Tile** tiles;
    Robber* robber;
    Seaport** ports;
    int boardSize;
    int innerBoardSize;

    GLuint dLists;

    int numWaterTiles;
    int numDeserts;
    int numPorts;

    int devCards[NUM_CARDS];
    
    void generateBoard(int size, int* resources, int* chits);

    float getBorderSize();
    
    Tile* getTileNeighbor(int tile, int direction);

    int* getScrambledChits();
    int* getScrambledResources(int deserts);
    int* getScrambledPorts(int ports);
    int indexOfMaxFromArray(int* array, int size);
    void scrambleIntArray(int* array, int size);
    void placePorts();
    void calculateNumberOfPorts();
};

#endif	/* BOARD_H */

