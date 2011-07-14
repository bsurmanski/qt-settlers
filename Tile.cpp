/*
 * File:   Tile.cpp
 * Author: brandon
 *
 * Created on May 2, 2011, 8:00 PM
 */

#include "Tile.h"
#include "GameLibrary.h"
#include "GLFrame.h"
#include <math.h>

Tile::Tile(Vector3f position, int chit, int resource) {
    corners = new CornerNode*[6];
    tiles = new Tile* [6];

    for (int i = 0; i < 6; i++) {
        corners[i] = NULL;
        tiles[i] = NULL;
    }

    blocked = false;
    port = false;
    token = chit;
    //model = new Model("hextile.mdl");
    model = GameLibrary::getInstance()->getCachedModel("hextile.mdl"); 
    this->position = position;
    this->resource = resource;
    model->changeColor(GameLibrary::getColorFor(resource));
    color = GameLibrary::getColorFor(resource);
    if (resource == WATER || resource == DESERT) {
        blocked = true;
    }
}

Tile::~Tile() {
    for (int i = 0; i < 6; i++) {
        //delete corners[i]; // TODO: MEMORY LEAK LOST ON CORNERS, fix
    }
    delete [] corners;
}

int Tile::getResource() {
    return resource;
}

/**
 * sets the nodes for this tile
 * @param the tile to correct to
 * @param position the side this tile is bordering on. starting at the bottom, and going arround CCW
 * @return
 */
void Tile::setBorderingTile(Tile* tile, Vector3f tileDirection) {
    int previous;
    if (tileDirection.y < -EPSILON) {
        if (tileDirection.x < -EPSILON) { // x less than 0
            previous = 5;
        } else {
            if (tileDirection.x > EPSILON) // x greater than 0
                previous = 1;
            else { // x equal 0
                previous = 0;
            }
        }
    } else { // y > 0
        if (tileDirection.x < -EPSILON) { // x less than  0
            previous = 4;
        } else {
            if (tileDirection.x > EPSILON) // x greater than 0
                previous = 2;
            else { // x equals 0
                previous = 3;
            }
        }
    }

    tiles[previous] = tile;
    tile->tiles[(previous + 3) % 6] = this;

    corners[previous] = tile->corners[(previous + 4) % 6];
    corners[(previous + 1) % 6] = tile->corners[(previous + 3) % 6];
    corners[previous]->setBorderringTile(tile);
    corners[(previous + 1) % 6]->setBorderringTile(tile);
    corners[previous]->setBorderringTile(this);
    corners[(previous + 1) % 6]->setBorderringTile(this);
}

Tile* Tile::getBorderingTile(int direction) {
    return tiles[direction];
}

void Tile::assignUnusedNodes() {
    Vector3f direction;
    float angle;
    for (int i = 0; i < 6; i++) {
        angle = (4 + i)*(PI / 3);
        direction = Vector3f(cos(angle), sin(angle), 0); // DOES NOT HAVE TILE SEPERATION IN. ASSUMES DISTANCE = 1
        if (corners[i] == NULL) {
            corners[i] = new CornerNode(this);
            corners[i]->setPosition(position + direction);

            corners[i]->setIsLeft(i % 2 == 0);
            //set node connection
            if (resource == WATER) {
                corners[i]->water = true;
            }
        }
    }
    updateConnected();
}

/**
 * updates all indicies
 */
void Tile::updateConnected() {
    for (int i = 0; i < 6; i++) {
        if (corners[i] != NULL && corners[i - 1] != NULL) {
            updateConnected(i);
        }
    }
}

/**
 * updates individual index to connect to previous node
 * @param i index of node
 */
void Tile::updateConnected(int i) {
    if (i != 0) {
        if (i == 2 || i == 3) {
            corners[i]->setConnectedNode(corners[i - 1], 0); // if moving upwards, previous is below
        } else if (i == 5) {
            corners[i]->setConnectedNode(corners[i - 1], 2); // moving upwards, previous is above
            corners[i]->setConnectedNode(corners[0], 0); // connect to first node, first node below
        } else { // applys to 1 or 4
            corners[i]->setConnectedNode(corners[i - 1], 1); // applys to 3rd node, connected is beside
        }
    }
}

/**
 * The token is the number that you want to roll to get resources
 * @return
 */
int Tile::getToken() {
    return token;
}

Vector3f Tile::getPosition() {
    return position;
}

bool Tile::hasPort() {
    return port;
}

/**
 * difference between hasPort bordersPort, and nextToPort is that the water tiles have the port, the surrounding tiles border port, and a tile which has a corner on a port is nextToPort
 * @return 
 */
bool Tile::nextToPort() {
    for (int i = 0; i < 6; i++) {
        if (corners[i]->getPort() != WATER) {
            return true;
        }
    }
    return false;
}

void Tile::addPort(int type, int direction) {
    port = true;
    corners[(direction)]->port = type;
    corners[(direction + 1) % 6]->port = type;
}

bool Tile::bordersPort() {
    for (int i = 0; i < 6; i++) {
        if (getBorderingTile(i) != NULL && getBorderingTile(i)->hasPort()) {
            return true; // dont put ports next to each other 
        }
    }
    return false;
}

bool Tile::isBlocked() {
    return blocked;
}

void Tile::setBlocked(bool blocked) {
    this->blocked = blocked;
}

void Tile::Draw() {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);

    glColor3f(color.r, color.g, color.b);
    model->Draw();
    if (!isBlocked())
        drawNumber();
}

void Tile::drawNumber() {
    glDisable(GL_LIGHTING);
    glColor3f(0.7, 0.7, 0.9);
    if (resource != WATER && resource != DESERT) {
        GameLibrary::getGLFrame()->renderText(position.x, position.y, position.z + 0.1, QString::fromStdString(GameLibrary::IntToStr(token)));
    }
    glEnable(GL_LIGHTING);
}

void Tile::glGenerateDisplayList() {
    glColor3f(color.r, color.g, color.b);
    model->glGenerateDisplayList(position);
}

Tile::CornerNode* Tile::occupyCorner(int corner) {
    if (!corners[corner]->isOccupied() && !corners[corner]->isOnWater()) {
        for (int i = 0; i < 3; i++) {
            if (corners[corner]->connected[i]->isOccupied()) {
                throw "placing here breaks proximity";
            }
        }
        corners[corner]->setOccupied();
        return corners[corner];
    }
    throw "Could not place piece at corner " + corner;
}

Tile::CornerNode::CornerNode(Tile* creator) {
    occupied = false;
    water = false;
    port = WATER; // -2 signifies no port
    connected = new CornerNode*[3];
    tiles = new Tile*[3];
    roads = new Road*[3];
    for (int i = 0; i < 3; i++) { // NULLify references
        tiles[i] = NULL;
        connected[i] = NULL;
        roads[i] = NULL; // no players own roads
    }
    tiles[0] = creator;
}

Tile::CornerNode::~CornerNode() {
    //NOTE, only works if tiles are deleted too, otherwise need to nullify tiles and connected
    delete [] tiles;
    delete [] connected;
    delete [] roads;
}

void Tile::CornerNode::setOccupied() {
    occupied = true;
}

bool Tile::CornerNode::isOccupied() {
    return occupied;
}

bool Tile::CornerNode::isBridge(Tile::CornerNode* node) {
    int sharedWaterTiles = 0;
    int toWaterTile = 0;
    for (int i = 0; i < 3; i++) {
        if (node->tiles[i]->getResource() == WATER) {
            toWaterTile++;
            for (int j = 0; j < 3; j++) {
                if (this->tiles[j] == node->tiles[i]) {
                    sharedWaterTiles++;
                    break;
                }
            }
        }
    }
    return (sharedWaterTiles == 2 && toWaterTile != 3);
}

void Tile::CornerNode::setIsLeft(bool left) {
    this->left = left;
}

bool Tile::CornerNode::isLeft() {
    return left;
}

int Tile::CornerNode::getPort() {
    return port;
}

Vector3f Tile::CornerNode::getPosition() {
    return position;
}

void Tile::CornerNode::setPosition(Vector3f position) {
    this->position = position;
}

Tile::CornerNode* Tile::getCorner(int corner) {
    return corners[corner];
}

Road* Tile::CornerNode::getRoad(int index) {
    return roads[index];
}

/**
 * Gets node in the given direction
 * @param direction
 * @return
 */
Tile::CornerNode* Tile::CornerNode::getSelection(int direction) {
    switch (direction) {
        case 0: // down
            return connected[0]; // return below
            break;
        case 1: // right
            if (left)
                return connected [1];
            break;
        case 2: // up
            return connected [2]; // return above
            break;
        case 3: // left
            if (!left)
                return connected [1]; // return side
            break;
    }
    return NULL;
}

Vector2f Tile::CornerNode::getVectorDirection(int direction) {
    Vector2f dir;
    switch (direction) {
        case 0:
            return Vector2f(sin(30), cos(30));
            break;
        case 1:
        case 3:
            return Vector2f(1, 0);
            break;
        case 2:
            return Vector2f(sin(30), -cos(30));
            break;
    }
    if (left)
        dir.x *= -1;
    return dir;
}

void Tile::CornerNode::setBorderringTile(Tile* tile) {
    for (int i = 0; i < 3; i++) {
        if (tiles[i] == tile) { // dont add duplicates
            return;
        }
        if (tiles[i] == NULL) {
            tiles[i] = tile;
            return;
        }
    }
    std::cout << this->position << " cannot border " << tile->position;
    throw tile; // bordering tiles is full, something is wrong
}

void Tile::CornerNode::setConnectedNode(CornerNode* node, int position) {
    connected[position] = node;
    switch (position) {
        case 0: // bottom
            node->connected[2] = this; // set reference to self as top;
            break;
        case 1: // middle
            node->connected[1] = this; //set reference to self as middle
            break;
        case 2: // top
            node->connected[0] = this; //set reference to self as bottom
            break;
    }
}

void Tile::CornerNode::registerRoad(Road* road, int direction) {
    this->roads[direction] = road;

}

bool Tile::CornerNode::cornerIsOn(int roll) {
    for (int i = 0; i < 3; i++) {
        if (tiles[i]->getToken() == roll) {
            return true;
        }
    }
    return false;
}

bool Tile::CornerNode::isOnWater() {
    if (water == true) {
        for (int i = 0; i < 3; i++) { // HACK, CUZ IF CORNERS GENERATED ON TILE, WILL BE SET AS water=true BY DEFAULT
            if (tiles[i] != NULL && (tiles[i]->getResource() != WATER)) {
                water = false;
                return false;
            }
        }
    }
    return water;
}

int Tile::CornerNode::getTileToken(int tile) {
    if (tile >= 0 && tile <= 3) {
        return tiles[tile]->getToken();
    }
    throw "invalid tile number"; // invalid tile number
}

int Tile::CornerNode::getResource(int tile) {
    return tiles[tile]->getResource();
}

