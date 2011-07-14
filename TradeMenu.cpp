/*
 * File:   TradeMenu.cpp
 * Author: brandon
 *
 * Created on May 19, 2011, 9:15 PM
 * 
 */

#include "TradeMenu.h"
#include "GameDockWidget.h"
#include <QtGui/QStatusBar>

TradeMenu::TradeMenu(QWidget* parent) : QWidget(parent) {
    this->setWindowTitle("Settlers! - Add New Trade");
    this->player = GameLibrary::getCurrentLocalPlayer();

    formLayout = new QGridLayout();
    giveLabel = new QLabel();
    giveLabel->setText("Give");
    takeLabel = new QLabel();
    takeLabel->setText("Take");

    formLayout->addWidget(giveLabel, 0, 0, 1, 1);
    formLayout->addWidget(takeLabel, 2, 0, 1, 1);

    for (int i = 0; i < NUM_RESOURCES; i++) {
        give[i] = new QSpinBox();
        give[i]->setRange(0, player->getResource(i)); //set maximum tradable amount to player's resource
        formLayout->addWidget(give[i], 0, 1 + i, 1, 1);
    }

    for (int i = 0; i < NUM_RESOURCES; i++) {
        iconLabels[i] = new QLabel();
        //Vector3f color = GameLibrary::getColorFor(i) * 255; //get color, and convert from float to 255 rgb
        QImage iconImg = QImage(QString::fromStdString("Resources/" + GameLibrary::getResourceString(i) + ".png"));
        //iconImg.setColor(0, QRgb(color.r, color.g, color.b, 255));
        QPixmap icon = QPixmap::fromImage(iconImg);

        iconLabels[i]->setPixmap(icon);
        formLayout->addWidget(iconLabels[i], 1, 1 + i, 1, 1);
        formLayout->setAlignment(iconLabels[i], Qt::AlignHCenter);
    }

    for (int i = 0; i < NUM_RESOURCES; i++) { // give and take are in separate for statements for tab-order
        take[i] = new QSpinBox();
        formLayout->addWidget(take[i], 2, 1 + i, 1, 1);
    }
    submit = new QPushButton();
    submit->setText("Create");
    formLayout->addWidget(submit, 3, NUM_RESOURCES - 1, 1, 2);
    this->setLayout(formLayout);
    connect(submit, SIGNAL(pressed()), this, SLOT(submitTrade()));
}

int TradeMenu::getTradeRatio(int resource) {
    if (player->hasPort(resource))
        return 2;
    if (player->hasPort(-1))
        return 3;
    return 4;
}

void TradeMenu::submitTrade() {
    int numGive[NUM_RESOURCES];
    int numTake[NUM_RESOURCES];
    bool giving = false;
    bool taking = false;

    for (int i = 0; i < NUM_RESOURCES; i++) {
        numTake[i] = take[i]->value();
        if (numTake[i] > 0) {
            taking = true;
        }
        numGive[i] = give[i]->value();
        if (numGive[i] > 0) {
            giving = true;
        }
    }
    if (giving && taking) {
        GameLibrary::getDockWidget()->newTradeOffer(numGive, numTake, player);
    } else {
        GameLibrary::addStatusBarMessage("You can't trade for nothing!", 5000);
    }
    this->close();
}

