/* 
 * File:   Road.cpp
 * Author: brandon
 * 
 * Created on May 2, 2011, 8:01 PM
 */

#include "Road.h"
#include "Board.h"
//#include <QtCore/QList>

Road::Road() {

}

Road::Road(Tile::CornerNode* node, Tile::CornerNode* node2, Vector3f color) {
    if (node == NULL || node->isOnWater() || node2 == NULL || node2->isOnWater()) {
        if (GameLibrary::BOATS) {
            isBoat = true;
            isBridge = false;
            //throw "boats not supported yet"; // FIX BOATS
        } else {
            throw "attempt to make boat, or not valid road"; // not a valid road, points to something it shouldnt
        }
    } else if (node->isBridge(node2)) {
        if (!GameLibrary::BRIDGES) {
            throw "attempt to make bridge";
        }
        isBoat = false;
        isBridge = true;
    } else {
        isBoat = false;
        isBridge = false;
    }

    int direction = -1;
    for (int i = 0; i < 4; i++) { //find direction
        if (node->getSelection(i) == node2) {
            direction = i;
            break;
        }
    }
    if (direction == 3) { // left/right are same direction; but node->getSelection treats them seperately
        direction = 1;
    }

    int node2Dir = 1;
    if (direction != 1) {
        node2Dir = (direction == 0) ? 2 : 0;
    }

    if (node->getRoad(direction) != NULL || node2->getRoad(node2Dir) != NULL) { //see if road is occupied
        throw "road occupied";
    }

    nodes = new Tile::CornerNode*[2];
    nodes[0] = node;
    nodes[1] = node2;
    node->registerRoad(this, direction);

    node2->registerRoad(this, node2Dir);

    if (isBoat) {
        //model = new Model("ship.mdl");
        model = GameLibrary::getInstance()->getCachedModel("ship.mdl");
    } else if (isBridge) {
        model = GameLibrary::getInstance()->getCachedModel("bridge.mdl");
        //model = new Model("bridge.mdl");
    } else {
        model = GameLibrary::getInstance()->getCachedModel("road.mdl");
        //model = new Model("road.mdl");
    }
    //model->changeColor(color);
    this->color = color;
    this->position = ((node->getPosition() + node2->getPosition()) / 2);
    this->direction = direction;
    this->rotation = getRotation(direction, node);
    visited = false;
}

Road::~Road() {
}

float Road::getRotation(int direction, Tile::CornerNode* node) {
    switch (direction) {
        case 0:
            if (node->isLeft())
                return 60;
            return -60;
            break;
        case 1:
            return 0;
            break;
        case 2:
            if (node->isLeft())
                return -60;
            return 60;
            break;
        case 3:
            return 0;
            break;
    }
    return 0;
}

//int Road::getLongestRoad() {
//    int maxSize = 15;
//    int length = 0;
//    Road** visited = new Road*[maxSize];
//    memset(visited, 0, sizeof (visited));
//    int a = getLongestRoadRecursive();
//
//    for (int i = 0; i < maxSize; i++) { //clean up
//        delete visited[i];
//    }
//    delete visited;
//    return length;
//}

/**
 * Thoughts: roads system could be separated. but if you check on each road placement, then it will always have longest
 * @param node node is the cornernode that you are entering from while looking for longest road.
 * therefore, if you enter from node A, do no search in that direction
 * @return 
 * 
 * ok, so i got a new idea for an algorythm that might work. first, it will not be recursive. It will store all the visited
 * roads in a list. It will first go as far as possible on one end of the list. It will then go as far as possible on the 
 * other end of the list. At side 1, it will take a step back, at side 2 it will see if it can take a step forward. It will
 * continue this, searching all possible routes.
 */
int Road::getLongestRoad(QList<Road*>* roadList, Tile::CornerNode* node) {
    if (visited) {
        return 0;
    }
    bool cleanup = false;
    if (roadList == NULL) {
        roadList = new QList<Road*>();
        cleanup = true;
    }
    int ROADS_PER_NODE = 3;
    int NODES_PER_ROAD = 2;
    int longest = 1;
    this->visited = true;
    for (int i = 0; i < NODES_PER_ROAD; i++) {
        Tile::CornerNode* cornerptr = this->nodes[i];
        if (node == cornerptr) {
            continue;
        }
        int longestOnSide = 0;
        for (int j = 0; j < ROADS_PER_NODE; j++) {
            Road* rdptr = cornerptr->getRoad(j);

            if (!roadList->contains(rdptr) && rdptr != NULL && rdptr != this && rdptr->color == color) {
                roadList->append(rdptr);
                int temp = rdptr->getLongestRoad(roadList, cornerptr);
                if (temp > longestOnSide) {
                    longestOnSide = temp;
                }
            }
        }
        longest += longestOnSide;
    }
    this->visited = false;
    if (cleanup) {
        delete roadList;
    }
    return longest;
}

//bool Road::roadVisited(Road** list, int length, Road* road) {
//    for (int i = 0; i < length; i++) {
//        if (list[i] == road) {
//            return true;
//        }
//    }
//    return false;
//}

Vector3f Road::getPlayerColor() {
    return color;
}

void Road::setIsBridge() {
    isBridge = true;
    delete model;
    model = new Model("bridge.mdl");
    model->changeColor(color);
}

bool Road::getIsBridge() {
    return isBridge;
}

void Road::setIsBoat() {
    isBoat = true;
    delete model;
    model = new Model("ship.mdl");
    model->changeColor(color);
}

bool Road::getIsBoat() {
    return isBoat;
}

void Road::Draw() {
    glPushMatrix();
    glColor3f(color.r, color.g, color.b);
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation, 0, 0, 1);
    model->Draw();
    glPopMatrix();
}

