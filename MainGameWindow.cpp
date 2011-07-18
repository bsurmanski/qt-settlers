/*
 * File:   MainGameWindow.cpp
 * Author: brandon
 *
 * Created on June 27, 2011, 10:45 PM
 */


#include <QtGui/QMenuBar>
#include <QtGui/QLayout>
#include <QtGui/QKeyEvent>
#include <QtCore/QString>
#include <QtGui/QDockWidget>
#include <QtGui/QSplitter>
#include <QtGui/QMessageBox>

#include "GLFrame.h"
#include "GameDockWidget.h"
#include "StatisticsWindow.h"
#include "NetworkManager.h"
#include "SettingsWidget.h"

#include "MainGameWindow.h"

MainGameWindow::MainGameWindow() : QMainWindow() {
    GameLibrary::setCurrentMainWindow(this);
    GameLibrary* single = new GameLibrary();

    setObjectName(QObject::tr("Settlers!"));

    settingsWidget = new SettingsWidget(this);
    this->setCentralWidget(settingsWidget);
    settingsWidget->show();
    connect(settingsWidget, SIGNAL(startGame()), this, SLOT(startGame()));

    statusBar = new QStatusBar(this);
    single->setQStatusBar(statusBar);
    this->setStatusBar(statusBar);

    this->setGeometry(0, 0, 640, 480);

    stat = NULL;
}

bool MainGameWindow::client_start() {
    if (GameLibrary::getNetworkManager()->isConnected()) {
        splitterWidget = new QSplitter(this);
        gameFrame = new GLFrame(splitterWidget);
        splitterWidget->addWidget(gameFrame);

        dockContent = new GameDockWidget(splitterWidget);
        splitterWidget->addWidget(dockContent);
        splitterWidget->setOrientation(Qt::Vertical);
        this->setCentralWidget(splitterWidget);
        gameFrame->newGame();
        dockContent->updatePlayerInfo();
        return true;
    } else {
        QMessageBox::warning(this, "No Connection", "Could not connect to host server. Please check your IP and Port settings and try again. \nIf you would like to host a game, please check the 'Host Server' Option. \nIf you would like to play on a single computer, please uncheck the 'Networking' option");
        return false;
    }
}

MainGameWindow::~MainGameWindow() {
    delete menuBar;
    delete statusBar;
    delete menuFile;
    delete actionQuit;
    delete menuWindow;
    delete actionStatistics;
    delete menuHelp;
    delete splitterWidget;
    delete dockContent;
    delete gameFrame;
    if (stat != NULL) {
        delete stat;
    }
}

void MainGameWindow::openStatistics() {
    if (stat != NULL) {
        delete stat;
    }
    stat = new StatisticsWindow();
    stat->show();
}

void MainGameWindow::startGame() {
    NetworkManager* nm = new NetworkManager(settingsWidget->getIP(), settingsWidget->getPort().toInt(), settingsWidget->serverSet());
    GameLibrary::setNetworkManager(nm);

    if (client_start()) {
        menuBar = new QMenuBar(this);
        menuFile = new QMenu(QObject::tr("File")); //File menu
        actionQuit = new QAction(QObject::tr("&Quit"), this); // Quit Option
        menuFile->addAction(actionQuit);
        menuWindow = new QMenu(QObject::tr("Window")); // Window menu
        actionStatistics = new QAction(QObject::tr("&Statistics"), this);
        menuWindow->addAction(actionStatistics);
        menuHelp = new QMenu(QObject::tr("Help")); // help menu
        menuBar->addMenu(menuFile);
        menuBar->addMenu(menuWindow);
        menuBar->addMenu(menuHelp);
        QObject::connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
        QObject::connect(actionStatistics, SIGNAL(triggered()), this, SLOT(openStatistics()));
        this->setMenuBar(menuBar);
    } else {
        delete nm;
    }
}

