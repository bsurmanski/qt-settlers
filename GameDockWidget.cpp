#include "GameDockWidget.h"
#include "Player.h"
#include "TradeOffer.h"
#include "DevelopmentCard.h"
#include "Board.h"
#include "NetworkManager.h"
#include <QMessageBox>

GameDockWidget::GameDockWidget(QWidget* parent) : QTabWidget(parent) {
    GameLibrary::setCurrentDockWidget(this);
    this->setTabPosition(this->West);
    this->setMinimumHeight(155);
    this->setDocumentMode(true);

    /**
     * main tab
     */
    tab1 = new QWidget(this);
    mainLayout = new QHBoxLayout();
    //devcard box
    devCardBox = new QGroupBox();
    devCardBox->setTitle("Development Cards");
    devCardLayout = new QVBoxLayout();
    devCardSpacer = new QSpacerItem(420, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    devCardList = new QListWidget();
    devCardList->verticalScrollBar()->hide();
    useDevCardButton = new QPushButton();
    useDevCardButton->setText("Activate");
    connect(useDevCardButton, SIGNAL(pressed()), this, SLOT(activateDevCard()));
    buyDevCardButton = new QPushButton();
    buyDevCardButton->setText("Buy New");
    connect(buyDevCardButton, SIGNAL(pressed()), this, SLOT(buyNewDevCard()));
    devCardLayout->addWidget(devCardList);
    devCardLayout->addWidget(useDevCardButton);
    devCardLayout->addItem(devCardSpacer);
    devCardLayout->addWidget(buyDevCardButton);
    devCardBox->setLayout(devCardLayout);
    mainLayout->addWidget(devCardBox);

    // resources box
    resourceLayout = new QGridLayout();
    resourceBox = new QGroupBox();
    resourceBox->setTitle("Resources");

    for (int i = 0; i < NUM_RESOURCES; i++) {
        resourceCount[i] = new QLineEdit();
        resourceCount[i]->setReadOnly(true);
        resourceCount[i]->setAlignment(Qt::AlignHCenter);
        resourceLayout->addWidget(resourceCount[i], 0, i, 1, 1); // set up all the LCD displays

        QPixmap icon = QPixmap(QString::fromStdString("Resources/" + GameLibrary::getResourceString(i) + ".png"));

        resourceIcons[i] = new QLabel();
        resourceIcons[i]->setPixmap(icon); //add all of the resource icons to the main tab resource display
        resourceIcons[i]->setAlignment(Qt::AlignHCenter);
        resourceLayout->addWidget(resourceIcons[i], 1, i, 1, 1);

        //DOCK ANCHORS, REMOVED
        //        icon = QPixmap("Resources/anchor.png");
        //
        //        portIcons[i] = new QLabel;
        //        portIcons[i]->setPixmap(icon); // do: update port, so that it changes color when port gained
        //        portIcons[i]->setAlignment(Qt::AlignHCenter);
        //resourceLayout->addWidget(portIcons[i], 2, i, 1, 1); //update so general port is shown?
    }
    resourceSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    resourceLayout->addItem(resourceSpacer, 3, NUM_RESOURCES - 1, 1, 1);
    playerScoreDisplay = new QLCDNumber();
    playerScoreDisplay->setNumDigits(2);
    playerScoreDisplay->setSegmentStyle(QLCDNumber::Flat);
    resourceLayout->addWidget(playerScoreDisplay, 4, NUM_RESOURCES - 1, 1, 1);
    scoreLabel = new QLabel();
    scoreLabel->setText("Score");
    scoreLabel->setAlignment(Qt::AlignRight);
    resourceLayout->addWidget(scoreLabel, 4, NUM_RESOURCES - 2, 1, 1);

    playerLabel = new QLabel();
    playerLabel->setAlignment(Qt::AlignHCenter);
    resourceLayout->addWidget(playerLabel, 4, 2, 1, 1);

    longestRoadLabel = new QLabel();
    largestArmyLabel = new QLabel();
    resourceLayout->addWidget(longestRoadLabel, 3, 0, 1, 1);
    resourceLayout->addWidget(largestArmyLabel, 4, 0, 1, 1);
    resourceBox->setLayout(resourceLayout);
    mainLayout->addWidget(resourceBox);
    tab1->setLayout(mainLayout);

    /**
     * Trade tab
     */
    tab2 = new QWidget(this);
    tradeLayout1 = new QHBoxLayout();
    tradeList = new QListWidget();
    buttonGroupBox = new QGroupBox();
    tradeLayout2 = new QVBoxLayout();
    tradeAccept = new QPushButton();
    tradeAccept->setText("Accept");
    tradeNew = new QPushButton();
    tradeNew->setText("Create New...");
    tradeLayout2->addWidget(tradeAccept);
    tradeLayout2->addWidget(tradeNew);
    buttonGroupBox->setLayout(tradeLayout2);
    tradeLayout1->addWidget(tradeList);
    tradeLayout1->addWidget(buttonGroupBox);
    tab2->setLayout(tradeLayout1);
    connect(tradeNew, SIGNAL(pressed()), this, SLOT(openTradeWindow()));
    connect(tradeAccept, SIGNAL(pressed()), this, SLOT(acceptTradeOffer()));

    /**
     * chat tab
     */
    tab3 = new QWidget(this); // Chat
    chatMessages = QString();
    chatLayout = new QVBoxLayout(tab3);
    textArea = new QTextEdit();
    lineArea = new QLineEdit();
    chatSubmitButton = new QPushButton();
    chatSubmitButton->setText("Submit");
    connect(chatSubmitButton, SIGNAL(pressed()), this, SLOT(addChatMessage()));
    connect(lineArea, SIGNAL(returnPressed()), this, SLOT(addChatMessage()));
    chatLayout->addWidget(textArea);
    textArea->setReadOnly(true);
    textArea->setText(chatMessages);
    chatLayout->addWidget(lineArea);
    chatLayout->addWidget(chatSubmitButton);
    tab3->setLayout(chatLayout);

    addTab(tab1, "Main");
    addTab(tab2, "Trade");
    addTab(tab3, "Chat");
}

/**
 * Main Methods
 */

void GameDockWidget::updatePlayerInfo() {
    Player* currentPlayer = GameLibrary::getCurrentLocalPlayer();
    Vector3f color = currentPlayer->getColor()*255;
    std::string textColor = GameLibrary::IntToStr(color.r) + "," + GameLibrary::IntToStr(color.g) + "," + GameLibrary::IntToStr(color.b); // convert the vector to a string representation
    playerLabel->setText(QString::fromStdString(currentPlayer->getName())); //set the player's name
    playerLabel->setStyleSheet(QString::fromStdString("QLabel{color: rgb(" + textColor + ");}"));

    if (currentPlayer->isRemote()) {
        //resourceBox->hide();
        //devCardBox->hide();
    } else {
        resourceBox->show();
        devCardBox->show();

        for (int i = 0; i < NUM_RESOURCES; i++) { // update player resource count
            resourceCount[i]->setText(QString::fromStdString(GameLibrary::IntToStr(currentPlayer->getResource(i))));
            playerScoreDisplay->display(currentPlayer->getScore());
        }

        while (devCardList->count() > 0) { // remove all items from the list
            devCardList->takeItem(0);
        }
        QList<DevelopmentCard*>* cards = currentPlayer->getdevCardList();
        for (int i = 0; i < cards->length(); i++) { // add all the development cards from the players hand
            devCardList->addItem(cards->at(i));
        }
    }

    if (GameLibrary::checkLargestArmy(currentPlayer)) {
        largestArmyLabel->setText("Largest Army!");
    } else {
        largestArmyLabel->clear();
    }

    if (GameLibrary::checkLongestRoad(currentPlayer)) {
        longestRoadLabel->setText("Longest Road!");
    } else {
        longestRoadLabel->clear();
    }

}

void GameDockWidget::buyNewDevCard() {
    int resourceCheck[] = {0, 1, 1, 0, 1}; // resources required for devCard
    if (GameLibrary::getCurrentLocalPlayer()->hasResources(resourceCheck)) {
        GameLibrary::getCurrentLocalPlayer()->subtractResource(resourceCheck);
        DevelopmentCard* card = new DevelopmentCard(GameLibrary::getCurrentBoard()->getDevCardNumber(), devCardList);
        QMessageBox* devCardDialog = new QMessageBox();
        devCardDialog->setText(QString::fromStdString("Received a new " + card->toString() + "!"));
        devCardDialog->setWindowTitle("Settlers | New Development Card!");
        devCardDialog->setAttribute(Qt::WA_DeleteOnClose);
        devCardDialog->show();

        GameLibrary::getCurrentLocalPlayer()->getdevCardList()->append(card);
    }
    updatePlayerInfo();
}

void GameDockWidget::activateDevCard() {
    QList<QListWidgetItem*> selected = devCardList->selectedItems();
    while (!selected.empty()) {
        DevelopmentCard* card = (DevelopmentCard*) selected.takeFirst();
        GameLibrary::getCurrentLocalPlayer()->activateDevCard(card);
    }
    updatePlayerInfo();
}

/**
 * Trade Methods
 */

/**
 * opens the trade window
 */
void GameDockWidget::openTradeWindow() {
    tradeWindow = new TradeMenu();
    tradeWindow->show();
    this->setCurrentIndex(0);
    tradeWindow->setAttribute(Qt::WA_DeleteOnClose); // deletes itself when closed;
}

void GameDockWidget::newTradeOffer(int give[NUM_RESOURCES], int take[NUM_RESOURCES], Player* offerer) {
    TradeOffer* offer = new TradeOffer(give, take, offerer, tradeList);
    bool toBank = false;
    QMessageBox tradeInfo;
    if (offer->isInvalid()) {
        tradeInfo.setInformativeText("Sorry, you cannot trade a resource for itself");
        tradeInfo.setStandardButtons(QMessageBox::Ok);
        tradeInfo.exec();
        return;
    }
    if (offer->canTradeToBank()) {
        tradeInfo.setWindowTitle("Settlers | Bank");
        tradeInfo.setInformativeText("This is a valid bank trade");
        tradeInfo.setText("Would you like to trade to the bank?");
        tradeInfo.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        tradeInfo.setDefaultButton(QMessageBox::Yes);
        toBank = (tradeInfo.exec() == QMessageBox::Yes);
    }
    if (toBank) {

        offer->getOfferingPlayer()->subtractResource(offer->getOffer());
        offer->getOfferingPlayer()->addResources(offer->getRequest());
        tradeList->removeItemWidget(offer);
        updatePlayerInfo();
        GameLibrary::addStatusBarMessage(offer->getOfferingPlayer()->getName() + " traded to the bank.", 5000);
        delete offer;
    } else {
        GameLibrary::addStatusBarMessage("New trade offer!", 5000);
    }
}

void GameDockWidget::acceptTradeOffer() {
    QList<QListWidgetItem*> selected = tradeList->selectedItems();
    if (selected.length() == 1) {
        TradeOffer* offer = (TradeOffer*) selected.first();
        Player* giver = offer->getOfferingPlayer(); //TODO: when networking, confirm that at least one of the players is current player. Maybe hide all the trades if network player's turn
        Player* taker = GameLibrary::getCurrentLocalPlayer();
        if (giver->hasResources(offer->getOffer()) && taker->hasResources(offer->getRequest())) {
            giver->addResources(offer->getRequest());
            taker->subtractResource(offer->getRequest());
            giver->subtractResource(offer->getOffer());
            taker->addResources(offer->getOffer());
            tradeList->removeItemWidget(offer);
            updatePlayerInfo();
            GameLibrary::addStatusBarMessage(taker->getName() + " accepted " + giver->getName() + "'s trade offer!", 5000);
            delete offer;
        }
    }
}

/**
 * Chat Methods
 */

void GameDockWidget::newChatMessage(std::string msg) {
    chatMessages.append(QString::fromStdString(msg));
    textArea->setText(chatMessages);

    QScrollBar *sb = textArea->verticalScrollBar();
    sb->setValue(sb->maximum()); // scroll the text area to the bottom
    lineArea->clear();
    GameLibrary::addStatusBarMessage("New chat message.", 5000);
}

void GameDockWidget::addChatMessage() {
    QString writer = QString::fromStdString(GameLibrary::getCurrentLocalPlayer()->getName());
    QString message = lineArea->text();
    QString addition = writer + ": " + message + "\n\n";


    NetworkManager* nm = GameLibrary::getNetworkManager();
    //if (nm->isServer()) {
    nm->sendPackets("msg", addition.toStdString());
    //}


    chatMessages.append(addition);
    textArea->setText(chatMessages);
    QScrollBar *sb = textArea->verticalScrollBar();
    sb->setValue(sb->maximum()); // scroll the text area to the bottom
    lineArea->clear();
    GameLibrary::addStatusBarMessage("New chat message.", 5000);
}
