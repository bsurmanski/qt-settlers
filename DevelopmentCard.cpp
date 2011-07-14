/* 
 * File:   DevelopmentCard.cpp
 * Author: brandon
 * 
 * Created on May 31, 2011, 7:15 PM
 */

#include "DevelopmentCard.h"
#include "GameLibrary.h"
#include "Player.h"
#include "Board.h"
#include "GameDockWidget.h"

#include <QtCore/QStringList>
#include <QtGui/QInputDialog>

DevelopmentCard::DevelopmentCard(QListWidget* parent) : QListWidgetItem(parent) {
    type = rand() % DEVCARD_TYPES;
    this->setText(QString::fromStdString(toString()));
}

DevelopmentCard::DevelopmentCard(int type, QListWidget* parent) : QListWidgetItem(parent) {
    this->type = type;
    this->setText(QString::fromStdString(toString()));
}

DevelopmentCard::~DevelopmentCard() {
}

bool DevelopmentCard::activate() {
    bool ok = false;
    QPair<int, int> yopResources;
    QStringList s_list;
    std::string toSteal;
    std::vector<Player*>* players;
    int stolenCount;

    switch (type) {
        case KNIGHT:
            while (!GameLibrary::getCurrentBoard()->moveRobber(rand() % GameLibrary::getCurrentBoard()->getNumberOfTiles()));
            ok = true;
            //TODO: and knight card should steal from player... 
            break;
        case ROAD_BUILD:
            //GameLibrary::getCurrentLocalPlayer()->giveResource(WOOD, 2);
            //GameLibrary::getCurrentLocalPlayer()->giveResource(BRICK, 2);
            QMessageBox::information(GameLibrary::getDockWidget(),"Free Roads!", "You can now build 2 free roads!", QDialogButtonBox::Ok, QDialogButtonBox::Ok);
            ok = true;
            break;
        case YEAR_PLENTY:
            yopResources = YearOfPlentyDialog::getResourcePair(GameLibrary::getDockWidget());
            if (yopResources.first > 0 && yopResources.second > 0) {
                GameLibrary::getCurrentLocalPlayer()->giveResource(yopResources.first, 1);
                GameLibrary::getCurrentLocalPlayer()->giveResource(yopResources.second, 1);
                ok = true;
            }
            break;
        case MONOPOLY:
            s_list = QStringList();
            for (int i = 0; i < NUM_RESOURCES; i++) {
                s_list.append(QString::fromStdString(GameLibrary::getResourceString(i)));
            }
            toSteal = QInputDialog::getItem(GameLibrary::getDockWidget(), "Settlers | Monopoly Card", "What resource would you like to steal from everyone?", s_list, 0, false, &ok).toStdString();
            if (ok) {
                players = GameLibrary::getPlayers();
                stolenCount = 0;
                for (int i = 0; i < GameLibrary::getNumberOfPlayers(); i++) {
                    if (players->at(i) != GameLibrary::getCurrentLocalPlayer()) {
                        stolenCount += players->at(i)->stealResource(GameLibrary::getResource(toSteal));
                    }
                }
                GameLibrary::getCurrentLocalPlayer()->giveResource(GameLibrary::getResource(toSteal), stolenCount);
            }
            break;
        case VICTORY_POINT:
            GameLibrary::getCurrentLocalPlayer()->addPoint();
            ok = true;
            break;
    }
    return ok;
}

int DevelopmentCard::getType() {
    return type;
}

std::string DevelopmentCard::toString() {
    switch (type) {
        case KNIGHT:
            return "Knight Card";
            break;
        case ROAD_BUILD:
            return "Road Builder's Card";
            break;
        case YEAR_PLENTY:
            return "Year of Plenty Card";
            break;
        case MONOPOLY:
            return "Monopoly Card";
            break;
        case VICTORY_POINT:
            return "Victory Point Card";
            break;
    }
    return "error card";
}

