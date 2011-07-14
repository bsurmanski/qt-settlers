/* 
 * File:   Player.h
 * Author: brandon
 *
 * Created on May 3, 2011, 10:00 PM
 */

#ifndef PLAYER_H
#define	PLAYER_H
#include "Road.h"
#include "Settlement.h"
#include "Sprite.h"
#include "DevelopmentCard.h"

#include <QMessageBox>
#include <QtCore/QList>

class Board;
class TradeOffer;

class Player {
public:
    Player(Vector3f color);
    Player(Vector3f color, std::string name);
    virtual ~Player();
    
    bool canPlaceSettlement();
    void placeSettlement();
    bool placeSettlement(int tile, int corner);
    
    bool canPlaceRoad(Tile::CornerNode* corner, int direction);
    void placeRoad();
    void placeRoad(int direction);
    void placeRoad(int tile, int corner, int direction); 
    void placeRoad(Tile::CornerNode* corner, int direction);
    bool isPlacingRoad();
    
    void upgradeSettlement();
    void upgradeSettlement(int tile, int corner);
    void upgradeSettlement(Tile::CornerNode* pos);
    
    bool isRemote();
    
    std::string serialize();
    
    void Draw();
    bool Update();
    void select(int direction);
    std::string getResourceString();
    int getResource(int resource);

    bool hasPort(int type);
    int tradeRatio(int resource);

    Vector3f getColor();

    Vector2f rollDice();

    bool endTurn();
    void beginTurn();

    void CollectResourcesOn(int num);

    void reset();

    std::string getName();

    int getScore();

    void statusChanged();

    bool hasResources(int check[NUM_RESOURCES]);
    void addResources(int amount [NUM_RESOURCES]);
    void subtractResource(int amount [NUM_RESOURCES]);
    int stealResource(int resource);
    int takeResource(int resource);
    void giveResource(int type, int quantity);
    void addPoint();

    bool hasLongestRoad();
    void setLongestRoad(bool longest);
    int getLongestRoad();
    bool hasLargestArmy();
    void setLargestArmy(bool largest);
    int getLargestArmy();


    bool activateDevCard(DevelopmentCard* card);
    int getKnightsPlayed();

    void setID(int id);
    int getID();

    QList<DevelopmentCard*>* getdevCardList();

private:

    class MonopolyWarning : public QMessageBox {
    public:

        MonopolyWarning(std::string theifName, int stolenResource, int amount) {
            this->theifName = theifName;
            this->stolenResource = stolenResource;
            this->amount = amount;
            this->setText(QString::fromStdString(theifName + " stole " + GameLibrary::IntToStr(amount) + " " + GameLibrary::getResourceString(stolenResource) + "!"));
            this->setWindowTitle("Settlers | Monopoly Warning");
        }
        std::string theifName;
        int stolenResource;
        int amount;
    };

    MonopolyWarning* monopolyWarning;

    bool isOnNetwork(Tile::CornerNode* node);
    bool startOfGame;
    bool rolled;
    bool currentPlayer;

    int state;

    int resources [NUM_RESOURCES];

    Tile::CornerNode* selection; // pointer to current node selection
    Sprite* selector; //visual representation of selection

    Road** roads; // array of pointers to all the roads placed
    int placedRoads; //number of placed roads

    Settlement** settlements; // array of pointers to all settlements placed

    int placedSettlements; // all placed settlements
    int placedCities;

    bool resourcePorts[NUM_RESOURCES];
    bool standardPort;

    Vector3f color; //player color

    int points;
    
    int freeRoads;

    int longestRoad;
    bool playerHasLongestRoad;
    int knightsPlayed;
    bool playerHasLargestArmy;

    std::string name;
    int id;
    
    bool remotePlayer;

    QList<DevelopmentCard*>* devCards;
};

#endif	/* PLAYER_H */

