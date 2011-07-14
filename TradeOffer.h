/* 
 * File:   TradeOffer.h
 * Author: brandon
 *
 * Created on May 18, 2011, 3:14 PM
 */

#ifndef TRADEOFFER_H
#define	TRADEOFFER_H
#include <QtGui/QWidget>
#include <QtGui/QListWidgetItem>
#include "GameLibrary.h"

class Player;

class TradeOffer : public QListWidgetItem {
public:

    TradeOffer(int giveOffer[NUM_RESOURCES], int takeOffer[NUM_RESOURCES], Player* player, QListWidget* parent = 0);

    bool isCompleted();
    bool isInvalid();

    std::string toString();

    int* getOffer();
    int* getRequest();
    
    Player* getOfferingPlayer();
    
    bool canTradeToBank();

private:
    int give[NUM_RESOURCES];
    int take[NUM_RESOURCES];
    bool completed;
    Player* offerer;
};

#endif	/* TRADEOFFER_H */

