/* 
 * File:   GameDockWidget.h
 * Author: brandon
 *
 * Created on June 10, 2011, 8:51 PM
 */

#ifndef GAMEDOCKWIDGET_H
#define	GAMEDOCKWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QLineEdit>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>
#include <QtGui/QListWidget>
#include <QtGui/QGroupBox>


#include "TradeMenu.h"
class Player;

class GameDockWidget : public QTabWidget {
    Q_OBJECT
public:
    GameDockWidget(QWidget* parent = 0);

    virtual ~GameDockWidget() {
    }

    void newTradeOffer(int give[NUM_RESOURCES], int take[NUM_RESOURCES], Player* offerer);

    void updatePlayerInfo();
    
    void newChatMessage(std::string msg);

protected slots:
    //main
    void buyNewDevCard();
    void activateDevCard();
    //trade
    void openTradeWindow();
    void acceptTradeOffer();
    //chat
    void addChatMessage();

private:
    QWidget* tab1;
    QWidget* tab2;
    QWidget* tab3;

    //main tab widgets
    QHBoxLayout* mainLayout;
    QGridLayout* resourceLayout;
    QGroupBox* resourceBox;
    QLineEdit* resourceCount [NUM_RESOURCES];
    QLabel* resourceIcons [NUM_RESOURCES];
    QLabel* portIcons [NUM_RESOURCES];
    QLCDNumber* playerScoreDisplay;
    QLabel* scoreLabel;
    QSpacerItem* resourceSpacer;
    QLabel* playerLabel;

    QLabel* longestRoadLabel;
    QLabel* largestArmyLabel;

    QGroupBox* devCardBox;
    QVBoxLayout* devCardLayout;
    QSpacerItem* devCardSpacer;
    QListWidget* devCardList;
    QPushButton* useDevCardButton;
    QPushButton* buyDevCardButton;

    //Trade tab widgets
    QHBoxLayout* tradeLayout1;
    QListWidget* tradeList;

    QVBoxLayout* tradeLayout2;
    QGroupBox* buttonGroupBox;
    QPushButton* tradeAccept;
    QPushButton* tradeNew;

    TradeMenu* tradeWindow;

    //Chat tab widgets
    QString chatMessages;
    QVBoxLayout* chatLayout;
    QTextEdit* textArea;
    QLineEdit* lineArea;
    QPushButton* chatSubmitButton;


};

#endif	/* GAMEDOCKWIDGET_H */

