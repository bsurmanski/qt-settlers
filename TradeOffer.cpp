/* 
 * File:   TradeOffer.cpp
 * Author: brandon
 * 
 * Created on May 18, 2011, 3:14 PM
 */

#include "TradeOffer.h"
#include "Player.h"

TradeOffer::TradeOffer(int giveOffer[NUM_RESOURCES], int takeOffer[NUM_RESOURCES], Player* player, QListWidget* parent) : QListWidgetItem(parent) {
    memcpy(give, giveOffer, sizeof (int[NUM_RESOURCES]));
    memcpy(take, takeOffer, sizeof (int[NUM_RESOURCES]));
    offerer = player;
    completed = false;
    this->setText(QString::fromStdString(this->toString()));
}

bool TradeOffer::isCompleted() {
    return completed;
}

std::string TradeOffer::toString() {
    std::string tradeString = "";
    tradeString.append(offerer->getName() + " OFFERS: ");
    for (int i = 0; i < NUM_RESOURCES; i++) {
        if (give[i] != 0) {
            tradeString.append(GameLibrary::IntToStr(give[i]) + " " + GameLibrary::getResourceString(i) + " ");
        }
    }
    tradeString.append("FOR ");

    for (int i = 0; i < NUM_RESOURCES; i++) {
        if (take[i] != 0) {
            tradeString.append(GameLibrary::IntToStr(take[i]) + " " + GameLibrary::getResourceString(i) + " ");
        }
    }
    return tradeString;
}

int* TradeOffer::getOffer() {
    return give;
}

int* TradeOffer::getRequest() {
    return take;
}

Player* TradeOffer::getOfferingPlayer() {
    return offerer;
}

bool TradeOffer::isInvalid() {
    bool empty = true;
    for (int i = 0; i < NUM_RESOURCES; i++) {
        if (give[i] > 0 || take[i] > 0) {
            empty = false;
            if (give[i] > 0 && take[i] > 0) {
                return true;
            }
        }
    }
    return empty;
}

bool TradeOffer::canTradeToBank() {
    int count = 0;
    for (int i = 0; i < NUM_RESOURCES; i++) {
        if (!(give[i] % offerer->tradeRatio(i) == 0)) {
            return false;
        } else if (give[i] > 0) {
            count += give[i] / offerer->tradeRatio(i);
        }
    }
    for (int i = 0; i < NUM_RESOURCES; i++) {
        count -= take[i];
    }
    if (count == 0) {
        return true;
    }
    return false;
}

