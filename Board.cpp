/* 
 * File:   Board.cpp
 * Author: brandon
 * 
 * Created on May 2, 2011, 8:03 PM
 */

#include "Board.h"
#include "Seaport.h"
#include <math.h>

/**
 * warning! complicated board init logic. sets up the hexagons, and connects the corner nodes
 */
Board::Board() {
    GameLibrary::setCurrentBoard(this);
    boardSize = 1;

    for (int i = 1; i < GameLibrary::BOARDRADIUS; i++) {
        boardSize += i * 6;
    }

    numDeserts = (int) std::max(floor((double) (boardSize / 10)) - 4 * GameLibrary::ISLANDS, 0.0);
    numWaterTiles = 0;

    innerBoardSize = boardSize; // board not including water

    if (GameLibrary::WATER_BORDER) {
        boardSize += GameLibrary::BOARDRADIUS * 6; // board with water
    }

    int* chits = getScrambledChits();
    int* resources = getScrambledResources(numDeserts);

    robber = NULL;

    //PLACING BOARD PIECES

    generateBoard(boardSize, resources, chits);

    //*
    //DEV CARD STUFF
    int temp [] = NUM_EACH_CARD_TYPE;
    for (int i = 0; i < NUM_CARDS; i++) { // set devcards so that the distribution is correct
        int type = 0;
        int count = 0;
        for (int j = 0; j < i; j++) {
            count++;
            if (count == temp[type]) {
                count = 0;
                type++;
            }
        }
        devCards[i] = type;
    }

    scrambleIntArray(devCards, NUM_CARDS);

    placePorts();
    delete chits;
    delete resources;
}

void Board::generateBoard(int size, int* resources, int* chits) {
    //Time for some deep magic!
    boardSize = size;
    innerBoardSize = 0;

    tiles = new Tile*[boardSize];
    memset(tiles, 0, sizeof (tiles));

    int count = 0;
    float connectionCount = 0; // used for connecting hexs to hexes from inner rings
    float angle = PI / 6;
    int previousFirstIndex;
    int currentFirstIndex = 0;
    int aboveTileIndex;
    Vector3f direction = Vector3f(GameLibrary::TILE_SEPARATION * cos(angle), GameLibrary::TILE_SEPARATION * sin(angle), 0); // direction to place next tile in
    Vector3f previousTile; // reference direction to previous tile in same ring
    Vector3f aboveTile; // refrerence direction to tile(s) in above-ring
    Vector3f position = Vector3f(); //position to create tile at

    for (int i = 0; i < GameLibrary::BOARDRADIUS + GameLibrary::WATER_BORDER; i++) { // radially generate the shape

        previousFirstIndex = currentFirstIndex;
        currentFirstIndex = count;
        if (i != 1) {
            connectionCount = 6 * (i - 1);
        } else {
            connectionCount = 1;
        }

        position = Vector3f(0, -i * GameLibrary::TILE_SEPARATION, 0);
        angle = PI / 6;
        for (int j = 0; j < i * 6 || (i == 0 && j == 0); j++) { //repeat until ring is full, or if first hexagon
            tiles[count] = new Tile(position, chits[count], resources[count]); //create new tile at position, with CHIT and RESOURCE

            if (i != 0) { //set node relationships.

                if (fabs(connectionCount - round(connectionCount)) < EPSILON) {
                    connectionCount = round(connectionCount); // prevents error propagation
                }
                // connect to ring above
                aboveTile = Vector3f(GameLibrary::TILE_SEPARATION * cos(angle + PI / 3), GameLibrary::TILE_SEPARATION * sin(angle + PI / 3), 0);
                if (j == (i * 6) - 1) {
                    aboveTileIndex = previousFirstIndex;
                } else {
                    aboveTileIndex = count - ((int) floor(connectionCount));
                }

                try {
                    tiles[count]->setBorderingTile(tiles[aboveTileIndex], aboveTile);
                } catch (Tile* t) {
                    std::cout << " tile count=" << count << " t count=" << aboveTileIndex << std::endl;
                    throw;
                }

                if (connectionCount != floor(connectionCount)) { // tile not on corner, connect to 2 above rings
                    aboveTile = Vector3f(GameLibrary::TILE_SEPARATION * cos(angle + PI * 2 / 3), GameLibrary::TILE_SEPARATION * sin(angle + PI * 2 / 3), 0);
                    tiles[count]->setBorderingTile(tiles[count - ((int) ceil(connectionCount))], aboveTile);
                }

                connectionCount += (1.0 / i);

                if (j != 0) { // connect to previous tile
                    tiles[count]->setBorderingTile(tiles[count - 1], previousTile); //set previous tile
                }
                previousTile = -direction;
                if (j == ((i * 6) - 1)) { // if last tile in ring, link to first tile
                    tiles[count]->setBorderingTile(tiles[count - j], direction);
                }
            }

            if (resources[count] == DESERT) {
                if (!robber)
                    robber = new Robber(tiles[count]); // create robber on desert
            }

            position += direction;

            if (i != 0 && ((count % i) == 0)) { //rotate on corners
                angle += PI / 3;
                direction = Vector3f(GameLibrary::TILE_SEPARATION * cos(angle), GameLibrary::TILE_SEPARATION * sin(angle), 0);
            }
            tiles[count]->assignUnusedNodes(); // create new corner nodes if not initialized
            count++;
        }
    }
}

Board::Board(const char* serial, int length) {
    numDeserts = 0;
    numWaterTiles = 0;
    robber = NULL;

    GameLibrary::setCurrentBoard(this);
    const char* parsePos = strchr(serial, ' ');

    boardSize = atoi(serial);

    int tile_resources[boardSize];
    int tile_chits[boardSize];
    memset(tile_resources, 0, sizeof (tile_resources));
    memset(tile_chits, 0, sizeof (tile_chits));

    for (int i = 0; i < boardSize; i++) {
        parsePos = strchr(parsePos, ' ') + 1;
        const char* split = strchr(parsePos, ':') + 1;
        tile_resources[i] = atoi(parsePos);

        if (tile_resources[i] == WATER) {
            numWaterTiles++;
        } else if (tile_resources[i] == DESERT) {
            numDeserts++;
        }

        tile_chits[i] = atoi(split);
    }
    generateBoard(boardSize, tile_resources, tile_chits);
    placePorts();
    //TODO: read ports into board
}

Board::~Board() {
    glDeleteLists(dLists, 1);
    for (int i = 0; i < boardSize; i++) {
        delete tiles[i];
    }
    delete [] tiles;
    if (robber != NULL)
        delete robber;
}

void Board::scrambleIntArray(int* array, int size) {
    int swap;
    int randNum;
    for (int entropy = 0; entropy < 5; entropy++) { // repeat for added entropy
        for (int i = 0; i < size; i++) { // scramble the array
            randNum = rand() % size;
            swap = array[randNum];
            array[randNum] = array[i];
            array[i] = swap;
        }
    }
}

int* Board::getScrambledChits() {
    int* chits = new int[boardSize];
    int chitOrder [18] = {8, 6, 5, 9, 10, 4, 3, 11, 12, 2, 3, 11, 10, 4, 5, 9, 8, 6}; // standardized chit numbering
    for (int i = 0; i < boardSize; i++) {
        if (i < innerBoardSize) {
            chits[i] = chitOrder[i % 18]; // keeps distribution proper
        } else {
            chits[i] = 0;
        }
    }
    scrambleIntArray(chits, innerBoardSize);
    return chits;
}

int* Board::getScrambledPorts(int ports) {
    int* retPorts = new int [ports];
    int portOrder [10] = {-1, 0, -1, 1, -1, 2, -1, 3, -1, 4};
    for (int i = 0; i < ports; i++) {
        retPorts[i] = portOrder[i % 10];
    }
    scrambleIntArray(retPorts, ports);
    return retPorts;
}

/*
 * gets the resource tiles for the game, scrambled.
 * size includes the number of deserts
 * 
 */
int* Board::getScrambledResources(int deserts) {
    assert(deserts <= boardSize);
    int resourceCount [] = {0, 0, 0, 0, 0};
    int* resources = new int [boardSize];
    for (int i = 0; i < boardSize; i++) {
        if (i < innerBoardSize) {
            if (GameLibrary::ISLANDS) {
                if (i % 2 == 0) {
                    resources[i] = ((i % 10) / 2);
                    resourceCount[resources[i]]++;
                } else {
                    resources[i] = WATER;
                    numWaterTiles++;
                }
            } else {
                resources[i] = (i % NUM_RESOURCES); //loop around from 1 to 5
            }
        } else {
            resources[i] = WATER;
            numWaterTiles++;
        }
    }

    for (int i = 0; i < deserts; i++) {
        if (resources[innerBoardSize - 1 - i] >= 0 && resources[innerBoardSize - 1 - i] < NUM_RESOURCES)
            resourceCount[resources[innerBoardSize - 1 - i]]--;
        resources[innerBoardSize - 1 - i] = DESERT; // assign deserts
    }

    scrambleIntArray(resources, innerBoardSize);
    return resources;
}

//int Board::indexOfMaxFromArray(int* array, int size) { // NOTE: UNUSED
//    int max = array[0];
//    int maxIndex = 0;
//    for (int i = 0; i < size; i++) {
//        if (array[i] > max) {
//            maxIndex = i;
//            max = array[i];
//        }
//    }
//    return maxIndex;
//}
//
//void Board::calculateNumberOfPorts() {
//
//}

void Board::placePorts() {
    numPorts = GameLibrary::BOARDRADIUS * 3; //(int) numWaterTiles / 2;

    int* portNumbers = getScrambledPorts(numPorts);

    ports = new Seaport*[numPorts];

    memset(ports, 0, sizeof (Seaport*[numPorts]));

    int waterTileIndices [numWaterTiles];

    for (int i = 0, currentIndex = 0; i < boardSize; i++) { // find all the water tiles
        if (tiles[i]->getResource() == WATER) {
            waterTileIndices[currentIndex] = i;
            currentIndex++;
        }
    }

    for (int i = 0, currentIndex = 0; i < numPorts && currentIndex < numWaterTiles; i++) {
        int possibilities = 0;
        int placementDirection;
        bool possible [] = {0, 0, 0, 0, 0, 0};

        if (tiles[waterTileIndices[currentIndex]]->bordersPort()) {
            currentIndex++;
            i--;
            continue; // dont put ports next to each other 
        }

        for (int j = 0; j < 6; j++) {
            if (tiles[waterTileIndices[currentIndex]]->getBorderingTile(j) != NULL
                    && (tiles[waterTileIndices[currentIndex]]->getBorderingTile(j)->getResource() != WATER)
                    && !tiles[waterTileIndices[currentIndex]]->getBorderingTile(j)->nextToPort()) {
                possibilities++;
                possible[j] = true;
            }
        }

        if (possibilities == 0) {
            currentIndex++;
            continue; // cant have unreachable port
        }

        placementDirection = 1 + (rand() % possibilities); // which of the possibilities is chosen

        for (int j = 0; j < 6; j++) { //essentially, this grabs a random side from the possible sides the port can be placed
            if (possible[j] == true) {
                placementDirection = j;
                break; // temp fix
            }
            if (placementDirection == 0 && possible[j] == true) {
                placementDirection = j;
                break;
            }
        }
        ports[i] = new Seaport(tiles[waterTileIndices[currentIndex]], placementDirection, GameLibrary::getColorFor(portNumbers[i])); // half of the seaports should be 3-1s

        //placementDirection = (placementDirection + 3) % 6;
        tiles[waterTileIndices[currentIndex]]->addPort(ports[i]->getType(), placementDirection);
        currentIndex++;
    }
    delete portNumbers;
}

/**
 * gets the closest tile to the position; if the vector is very far away, this would be a map edge
 * @param position
 * @return pointer to closest tile
 */
Tile* Board::getClosestTile(Vector3f position) {
    float minimum = (position - tiles[0]->getPosition()).lengthSq();
    int minIndex = 0;
    for (int i = 0; i < boardSize; i++) {
        if ((position - tiles[0]->getPosition()).lengthSq() < minimum) {
            minIndex = i;
        }
    }
    return tiles[minIndex];
}

int Board::getNumberOfTiles() {
    return boardSize;
}

Vector2i Board::tileAndCornerNumber(Tile::CornerNode* node) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < 6; j++) {
            if (tiles[i]->getCorner(j) == node) {
                return Vector2i(i, j);
            }
        }
    }
    return Vector2i();
}

Tile::CornerNode* Board::getCornerNode(int tile, int corner) {
    return tiles[tile]->getCorner(corner);
}

/**
 * move the robber, return true if successful
 * @param tile
 * @return 
 */
bool Board::moveRobber(int tile) {
    if (numDeserts == 0) {
        return true;
    } else if (tiles[tile]->getResource() != WATER) {
        robber->moveTo(tiles[tile]);
        return true;
    }
    return false;
}

float Board::getBorderSize() {
    return (GameLibrary::TILE_SEPARATION - 2 * sin(PI / 3));
}

Tile::CornerNode* Board::getDefaultNodeSelection() {
    return tiles[0]->getCorner(0);
}

Tile::CornerNode* Board::occupyCorner(int tile, int corner) {
    return tiles[tile]->occupyCorner(corner);
}

Tile::CornerNode* Board::occupyCorner(Tile::CornerNode* corner) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < 6; j++) {
            if (tiles[i]->getCorner(j) == corner) {
                return occupyCorner(i, j);
            }
        }
    }
    throw "corner not found";
}

Vector2f Board::rollDice() {
    return Vector2f(rand() % 6 + 1, rand() % 6 + 1);
}

int Board::getDevCardNumber() {
    int ret = devCards[0];
    memmove(devCards, devCards + 1, sizeof (int[NUM_CARDS - 1]));
    devCards[NUM_CARDS - 1] = ret;
    return ret;
}

void Board::Draw() {
    glDisable(GL_TEXTURE_2D);
    glCallList(dLists);

    for (int i = 0; i < boardSize; i++) {
        tiles[i]->drawNumber();
    }
    //glColor3f(1, 1, 1);
    //glCallList(numbersDisplayList);
    if (numDeserts > 0) {
        robber->Draw(); 
    }
}

/**
 * get display list for supah fast generation
 * @param index
 */
void Board::buildLists() {
    dLists = glGenLists(2);


    glNewList(dLists, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < boardSize; i++) {
        tiles[i]->glGenerateDisplayList();
    }

    for (int i = 0; i < numPorts; i++) {
        if (ports[i] != NULL)
            ports[i]->glGenerateDisplayList();
    }
    glEnd();
    glEndList();

    glNewList(dLists + 1, GL_COMPILE);
    glColor3f(0, 0, 0);
    for (int i = 0; i < boardSize; i++) {
        // tiles[i]->drawNumber(); //fixed numbers lagging behind by using stoke string instead of bitmap
    }
    glEndList();
}

bool Board::Update() {
    if (numDeserts != 0)
        return robber->Update();
    return false;
}

std::string Board::serialize() {
    std::string ret = "";
    ret.append(GameLibrary::IntToStr(boardSize) + " ");
    for (int i = 0; i < boardSize; i++) {
        ret.append(GameLibrary::IntToStr(tiles[i]->getResource()) + ":" + GameLibrary::IntToStr(tiles[i]->getToken()) + " ");
    }
    for (int i = 0; i < numPorts; i++) {
        //TODO: ADD PORTS
    }
    return ret;
}