/* 
 * File:   Player.cpp
 * Author: brandon
 * 
 * Created on May 3, 2011, 10:00 PM
 */

#include <iostream>
#include <sstream>

#include <QtGui/QMessageBox>

#include "Player.h"
#include "Board.h"
#include "TradeOffer.h"
#include "GameDockWidget.h"
#include "NetworkManager.h"

//maximum allowed pieces
#define NUMROADS 15
#define SETTLEMENTS 5
#define CITIES 4

//Player States
#define NORMAL 0
#define PLACING_ROAD 1

Player::Player(Vector3f color, const std::string name) {
    devCards = new QList<DevelopmentCard*>();
    this->color = color;
    selection = NULL;
    selector = new Sprite("selector.png");
    selector->addState("roadPlace.png");

    roads = new Road*[NUMROADS];
    settlements = new Settlement*[SETTLEMENTS + CITIES]; //holds all settlements and 

    startOfGame = true;
    currentPlayer = false;

    this->name = std::string(name);

    remotePlayer = false;
}

Player::~Player() {
    for (int i = 0; i < SETTLEMENTS + CITIES; i++) {
        if (settlements[i] != NULL)
            delete settlements[i];
    }
    for (int i = 0; i < NUMROADS; i++) {
        if (roads[i] != NULL)
            delete roads[i];
    }
    delete [] settlements;
    delete [] roads;
    delete selector;
}

void Player::reset() {
    for (int i = 0; i < SETTLEMENTS + CITIES; i++) { // reset settlements
        if (settlements[i] != NULL) {
            //delete settlements[i]; //TODO: fix memory management here
            settlements[i] = NULL;
        }
    }

    for (int i = 0; i < NUMROADS; i++) { //reset roads
        if (roads[i] != NULL) {
            //delete roads[i];
            roads[i] = NULL;
        }
    }

    memset(roads, 0, sizeof (Road*[NUMROADS]));
    memset(settlements, 0, sizeof (Settlement*[SETTLEMENTS + CITIES]));
    memset(resources, 0, sizeof (int[NUM_RESOURCES]));
    memset(resourcePorts, 0, sizeof (bool[NUM_RESOURCES]));

    placedSettlements = 0;
    placedCities = 0;
    placedRoads = 0;
    longestRoad = 0;
    freeRoads = 0;

    playerHasLongestRoad = false;
    knightsPlayed = 0;
    playerHasLargestArmy = false;

    standardPort = false;
    rolled = false;

    points = 0;
    state = NORMAL;

    monopolyWarning = NULL;
}

bool Player::canPlaceSettlement() {
    if (remotePlayer) {
        return true;
    }
    if (placedSettlements < SETTLEMENTS && currentPlayer) {
        if (!startOfGame) {
            for (int i = 0; i < 4; i++) {
                if (resources[i] == 0) {
                    std::cout << "not enough resources";
                    return false;
                }
            }
        } else
            if (placedSettlements != placedRoads) {
            std::cout << "wait your turn. Placing too many settlements at a time at start of game";
            return false;
        }
        if (!isOnNetwork(selection) && !startOfGame) {
            std::cout << "not on network, and not start of game";
            return false;
        }
        if (selection->isOccupied()) {
            return false;
        }
        for (int i = 0; i < 4; i++) {
            if (selection->getSelection(i) != NULL && selection->getSelection(i)->isOccupied()) {
                return false;
            }
        }
    }
    return true;
}

void Player::placeSettlement() {
    if (canPlaceSettlement()) {
        Tile::CornerNode* placement = GameLibrary::getCurrentBoard()->occupyCorner(selection);
        settlements[placedSettlements + placedCities] = new Settlement(placement, color);
        placedSettlements++;

        if (selection->getPort() != WATER) { //add ports if exist on corner
            if (selection->getPort() == DESERT) {
                standardPort = true;
            } else {
                resourcePorts[selection->getPort()] = true;
            }
        }

        if (startOfGame) {
            if (GameLibrary::STARTING_RESOURCES && placedSettlements == 1) {
                for (int i = 0; i < 3; i++) { // give starting resources for first settlement
                    int resource = selection->getResource(i);
                    if (resource >= 0)
                        resources[resource]++;
                }
            }
        } else if (!remotePlayer) {
            int lose [NUM_RESOURCES] = {1, 1, 1, 1, 0};
            subtractResource(lose);
        }
        Vector2i tileCorner = GameLibrary::getCurrentBoard()->tileAndCornerNumber(placement);
        GameLibrary::getNetworkManager()->sendPackets("set", GameLibrary::colorToStr(color) + " " + GameLibrary::IntToStr(tileCorner.s) + " " + GameLibrary::IntToStr(tileCorner.t));
        points++;
        statusChanged();
    }
}

bool Player::placeSettlement(int tile, int corner) {
    Tile::CornerNode* placement = GameLibrary::getCurrentBoard()->getCornerNode(tile, corner);
    GameLibrary::getCurrentBoard()->occupyCorner(placement);
    settlements[placedSettlements + placedCities] = new Settlement(placement, color);
    placedSettlements++;
    return true;
}

/**
 * used for initialization of placing roads
 */
void Player::placeRoad() {
    if (state == NORMAL && placedRoads < NUMROADS) {
        state = PLACING_ROAD;
    } else {
        state = NORMAL;
        statusChanged();
    }
    selector->changeState(state);
}

bool Player::canPlaceRoad(Tile::CornerNode* corner, int direction) {
    if (remotePlayer) {
        return true;
    }

    if (corner->getSelection(direction) == NULL) { //cannot build off of map
        return false;
    }

    if (!isOnNetwork(corner)) { // not allowed road off of player's network.
        return false;
    }

    if (corner->isOccupied()) {
        for (int i = 0; i < placedSettlements + placedCities; i++) { // check to see if player owns settlement on selection
            if (settlements[i]->getCorner() == corner) { // player's settlement. Allow road (if all else is fine)
                break;
            } else if (i == (placedSettlements + placedCities) - 1) { //searched through whole list, not player's settlement
                return false; // cant build road through other player's city
            }
        }
    }

    int roadIndex = direction;
    if (roadIndex == 3)
        roadIndex = 1; // direction is cardinal, yet nodes only have 3 directions; left/right are treated as one

    if (corner->getRoad(roadIndex) != NULL) { // road placing already occupied
        return false;
    }

    if (corner->isBridge(corner->getSelection(direction))) {
        if (!GameLibrary::BRIDGES) { // cannot make bridge, not allowed
            std::cout << "attempt to make bridge";
            return false;
        }
        if ((!startOfGame) && resources[ROCK] < 1 && freeRoads == 0) { // insufficient resources
            return false;
        }
    }

    if (corner->getSelection(direction)->isOnWater()) {
        if (!GameLibrary::BOATS) { // only allowed boats if boats setting on
            std::cout << "attempt to make boat";
            return false;
        }
        if ((!startOfGame) && (resources[SHEEP] < 1 || resources[WOOD] < 1) && freeRoads == 0) { // insufficient resources
            return false;
        }
    }

    if (!startOfGame
            && (
            placedRoads >= NUMROADS
            || (resources[WOOD] < 1 && resources[BRICK] < 1)
            )
            && (freeRoads == 0)) {
        return false;
    }


    if (startOfGame && placedRoads >= placedSettlements) {
        return false;
    }

    return true;
}

void Player::placeRoad(int tile, int corner, int direction) {
    Tile::CornerNode* t_corner = GameLibrary::getCurrentBoard()->getCornerNode(tile, corner);
    placeRoad(t_corner, direction);
}

void Player::placeRoad(Tile::CornerNode* corner, int direction) {
    if (canPlaceRoad(corner, direction)) {
        int roadIndex = direction;
        if (roadIndex == 3)
            roadIndex = 1; // direction is cardinal, yet nodes only have 3 directions; left/right are treated as one

        if (corner->isBridge(corner->getSelection(direction)) && (!startOfGame && freeRoads == 0)) {
            resources[ROCK]--;
        } else if (corner->getSelection(direction)->isOnWater()) {
            if (GameLibrary::BOATS) {
                if (!startOfGame) {
                    if (freeRoads > 0) {
                        freeRoads--;
                        GameLibrary::addStatusBarMessage(GameLibrary::IntToStr(freeRoads) + " free roads remaining", 5000);
                    } else {
                        resources[SHEEP]--;
                        resources[WOOD]--;
                    }
                }
            }
        }
        if (!startOfGame) {
            if (freeRoads > 0) {
                freeRoads--;
                GameLibrary::addStatusBarMessage(GameLibrary::IntToStr(freeRoads) + " free roads remaining", 5000);
            } else {
                resources[WOOD]--;
                resources[BRICK]--;
            }
        } else {
            currentPlayer = false; //start of game, only allowed to place one road/ settlement at a time
            if (placedRoads == 1) {
                startOfGame = false;
                rolled = true;
            }
        }

        statusChanged();
        state = NORMAL;
        selector->changeState(state);
        roads[placedRoads] = new Road(corner, corner->getSelection(direction), color);
        int roadLength = roads[placedRoads]->getLongestRoad();
        placedRoads++;

        Vector2i tileCorner = GameLibrary::getCurrentBoard()->tileAndCornerNumber(corner);
        GameLibrary::getNetworkManager()->sendPackets("rd ", GameLibrary::colorToStr(color) + " " + GameLibrary::IntToStr(tileCorner.s) + " " + GameLibrary::IntToStr(tileCorner.t) + " " + GameLibrary::IntToStr(direction));

        if (roadLength > longestRoad) {
            longestRoad = roadLength;
            GameLibrary::checkLongestRoad(this);
        }
    }
}

/**
 * used once already 'placingRoad', called once a direction is chosen
 * @param direction direction to place road
 */
void Player::placeRoad(int direction) {
    placeRoad(selection, direction);
}

bool Player::hasLongestRoad() {
    return playerHasLongestRoad;
}

void Player::setLongestRoad(bool longest) {
    if (longest != playerHasLongestRoad) {
        points += (longest ? 2 : -2);
        playerHasLongestRoad = longest;
    }
}

int Player::getLongestRoad() {
    return longestRoad;
}

bool Player::hasLargestArmy() {
    return playerHasLargestArmy;
}

void Player::setLargestArmy(bool largest) {
    if (largest != playerHasLargestArmy) {
        points += (largest ? 2 : -2);
        playerHasLargestArmy = largest;
    }
}

int Player::getLargestArmy() {
    return knightsPlayed;
}

bool Player::activateDevCard(DevelopmentCard* card) {
    if (card->activate()) {
        if (card->getType() == KNIGHT) {
            knightsPlayed++;
            GameLibrary::checkLargestArmy(this);
        } else if (card->getType() == ROAD_BUILD) {
            freeRoads += 2;
        }
        getdevCardList()->removeOne(card);
        delete card;
        return true;
    }
    return false;
}

int Player::getKnightsPlayed() {
    return knightsPlayed;
}

Vector3f Player::getColor() {
    return color;
}

bool Player::hasPort(int type) {
    if (type >= 0 && type <= NUM_RESOURCES) {
        return resourcePorts[type];
    }
    return standardPort;
}

bool Player::endTurn() {
    if ((state == NORMAL && rolled) || (startOfGame && placedRoads == placedSettlements && placedRoads == 1)) {
        rolled = false;
        currentPlayer = false;
        if (monopolyWarning != NULL) {
            delete monopolyWarning;
            monopolyWarning = NULL;
        }
        if (freeRoads > 0 && !remotePlayer) {
            return (QMessageBox::question((QWidget*) GameLibrary::getDockWidget(), "lose free roads?", QString::fromStdString("You still have " + GameLibrary::IntToStr(freeRoads) + " free roads, are you sure you want to end your turn and lose them?"), QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton) == QMessageBox::Yes);
        }
        return true;
    }
    return false;
}

bool Player::isRemote() {
    return remotePlayer;
}

void Player::setRemote(bool rmt) {
    remotePlayer = rmt;
}

void Player::beginTurn() {
    currentPlayer = true;
    statusChanged();
    if (monopolyWarning != NULL) {
        monopolyWarning->show();
    }
}

void Player::addPoint() {
    points++;
}

int Player::getScore() {
    return points;
}

void Player::setCurrentBoard(Board* b) {
    selection = b->getDefaultNodeSelection();
    selector->easeTo(selection->getPosition() + Vector3f(0, 0, 2));
}

/**
 * 
 * @param node
 * @return
 */
bool Player::isOnNetwork(Tile::CornerNode* node) {
    for (int i = 0; i < 3; i++) { // check for connecting roads
        if (selection->getRoad(i) != NULL && selection->getRoad(i)->getPlayerColor() == color) {
            return true;
        }
    }
    if (selection->isOccupied()) {
        for (int i = 0; i < placedSettlements + placedCities; i++) { // check to see if it is current player's settlement
            if (settlements[i]->getCorner() == node)
                return true;
        }
    }
    return false;
}

void Player::upgradeSettlement() {
    upgradeSettlement(selection);
}

void Player::upgradeSettlement(int tile, int corner) {
    Tile::CornerNode* t_corner = GameLibrary::getCurrentBoard()->getCornerNode(tile, corner);
    upgradeSettlement(t_corner);
}

void Player::upgradeSettlement(Tile::CornerNode* pos) {
    if (resources[WHEAT] >= 2 && resources [ROCK] >= 3) {
        for (int i = 0; i < placedSettlements + placedCities; i++) {
            if (settlements[i]->position == pos->getPosition()) {
                if (!settlements[i]->isCity()) { //only upgrade if not city
                    settlements[i]->upgradeToCity();
                    placedSettlements--;
                    placedCities++;
                    resources[WHEAT] -= 2;
                    resources[ROCK] -= 3;
                    points++;
                    statusChanged();

                    Vector2i tileCorner = GameLibrary::getCurrentBoard()->tileAndCornerNumber(pos);
                    GameLibrary::getNetworkManager()->sendPackets("upg", GameLibrary::colorToStr(color) + " " + GameLibrary::IntToStr(tileCorner.s) + " " + GameLibrary::IntToStr(tileCorner.t));
                    break;
                } else {
                    return;
                }
            }
        }
    }
}

std::string Player::getResourceString() {
    std::string str = "";
    for (int i = 0; i < NUM_RESOURCES; i++) {
        str += GameLibrary::IntToStr(resources[i]) + " ";
    }
    return str;
}

int Player::getResource(int resource) {
    return resources[resource];
}

bool Player::Update() {
    return selector->Update();
}

void Player::Draw() {
    for (int i = 0; i < placedSettlements + placedCities; i++) {
        settlements[i]->Draw();
    }
    for (int i = 0; i < placedRoads; i++) {
        roads[i]->Draw();
    }
    if (currentPlayer && !remotePlayer)
        selector->Draw();
}

void Player::select(int direction) {
    if (state == NORMAL && currentPlayer) {
        if (selection->getSelection(direction) != NULL)
            selection = selection->getSelection(direction);
        selector->easeTo(selection->getPosition() + Vector3f(0, 0, 2));
    } else if (state == PLACING_ROAD) { //placing road
        placeRoad(direction);
    }
}

Vector2f Player::rollDice() {
    if (state == NORMAL && !rolled && !startOfGame) {
        rolled = true;
        return GameLibrary::getCurrentBoard()->rollDice();
    }
    return Vector2f();
}

void Player::CollectResourcesOn(int num) {
    Vector2i collection;

    for (int i = 0; i < placedSettlements + placedCities; i++) {
        if (settlements[i]->tileIsOn(num)) {
            for (int j = 0; j < 3; j++) {
                collection = settlements[i]->collectionFromTile(num, j);
                resources[collection.s] += collection.t;
            }
            statusChanged();
        }
    }
}

std::string Player::getName() {
    return name;
}

void Player::statusChanged() {
    if (GameLibrary::getDockWidget()) {
        GameLibrary::getDockWidget()->updatePlayerInfo();
    }
}

bool Player::hasResources(int check[NUM_RESOURCES]) {
    for (int i = 0; i < NUM_RESOURCES; i++) {
        if (resources[i] < check[i]) {
            return false;
        }
    }
    return true;
}

void Player::addResources(int amount[NUM_RESOURCES]) {
    for (int i = 0; i < NUM_RESOURCES; i++) {
        resources[i] += amount[i];
    }
}

void Player::subtractResource(int amount [NUM_RESOURCES]) {
    for (int i = 0; i < NUM_RESOURCES; i++) {
        resources[i] -= amount[i];
    }
}

int Player::stealResource(int resource) {
    monopolyWarning = new MonopolyWarning(GameLibrary::getCurrentLocalPlayer()->getName(), resource, resources[resource]);
    return takeResource(resource);
}

int Player::takeResource(int resource) {
    int ret = resources[resource];
    resources[resource] = 0;
    return ret;
}

void Player::giveResource(int type, int quantity) {
    resources[type] += quantity;
}

int Player::tradeRatio(int resource) {
    if (resourcePorts[resource]) {
        return 2;
    } else if (standardPort) {
        return 3;
    }
    return 4;
}

QList<DevelopmentCard*>* Player::getdevCardList() {
    return devCards;
}

int Player::getID() {
    return id;
}

void Player::setID(int id) {
    this->id = id;
}

std::string Player::serialize() {
    return GameLibrary::colorToStr(color) + " :" + name;
}