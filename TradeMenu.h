/* 
 * File:   TradeMenu.h
 * Author: brandon
 *
 * Created on May 19, 2011, 9:15 PM
 */

#ifndef TRADEMENU_H
#define	TRADEMENU_H
#include "Player.h"
#include <QtGui/QWidget>
#include <QtGui/QSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QPushButton>

class TradeMenu : public QWidget {
    Q_OBJECT
public:
    TradeMenu(QWidget* parent = 0);

    //void acceptOffer(Player* player);

protected slots:
    void submitTrade();

private:
    int getTradeRatio(int resource);

    QGridLayout* formLayout;
    QLabel* giveLabel;
    QLabel* takeLabel;
    QLabel* iconLabels [NUM_RESOURCES];
    QSpinBox* give[NUM_RESOURCES];
    QSpinBox* take[NUM_RESOURCES];
    QPushButton* submit;

    Player* player;
};

#endif	/* TRADEMENU_H */

