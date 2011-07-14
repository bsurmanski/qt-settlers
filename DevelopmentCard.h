/* 
 * File:   DevelopmentCard.h
 * Author: brandon
 *
 * Created on May 31, 2011, 7:15 PM
 */

#ifndef DEVELOPMENTCARD_H
#define	DEVELOPMENTCARD_H

#include <QtGui/QListWidgetItem>
#include <QtGui/QWidget>
#include <string>

#include "GameLibrary.h"
#include "YearOfPlentyDialog.h"

class Player;

class DevelopmentCard : public QListWidgetItem {
public:
    DevelopmentCard(QListWidget* parent = 0);
    DevelopmentCard(int type, QListWidget* parent = 0);
    DevelopmentCard(const DevelopmentCard& orig);
    virtual ~DevelopmentCard();
    int getType();

    std::string toString();

    bool activate();
protected:

    YearOfPlentyDialog yopDialog;
    int type;
};

#endif	/* DEVELOPMENTCARD_H */

