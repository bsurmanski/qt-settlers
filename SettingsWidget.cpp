/* 
 * File:   SettingsWidget.cpp
 * Author: brandon
 * 
 * Created on June 29, 2011, 10:51 AM
 */

#include "SettingsWidget.h"
#include "GameLibrary.h"
#include "Player.h"
#include <QtGui/QMessageBox>

SettingsWidget::SettingsWidget(QWidget* parent) : QWidget(parent) {
    GameLibrary::setSettingsWidget(this);
    layout = new QGridLayout();
    networkCheckBox = new QCheckBox();
    networkCheckBox->setText("Enable Networking");
    networkCheckBox->setChecked(true);
    connect(networkCheckBox, SIGNAL(stateChanged(int)), this, SLOT(toggleNetworking(int)));
    layout->addWidget(networkCheckBox, 0, 0, 1, 1);
    networkBox = new QGroupBox();
    networkLayout = new QFormLayout();
    networkBox->setTitle("Networking");
    serverCheckBox = new QCheckBox();
    serverCheckBox->setText("Host Server");
    connect(serverCheckBox, SIGNAL(stateChanged(int)), this, SLOT(toggleServer(int)));
    networkLayout->addWidget(serverCheckBox);
    ipLabel = new QLabel();
    ipLabel->setText("Host IP");
    ipLine = new QLineEdit();
    ipLine->setText("127.0.0.1");
    //ipLine->setInputMask("000.000.000.000;");
    networkLayout->addRow(ipLabel, ipLine);
    tcpPortLabel = new QLabel();
    tcpPortLabel->setText("TCP Network Port");
    tcpPortLine = new QLineEdit();
    tcpPortLine->setText("9876");
    networkLayout->addRow(tcpPortLabel, tcpPortLine);
    networkBox->setLayout(networkLayout);
    layout->addWidget(networkBox, 1, 0, 1, 3);

    playerBox = new QGroupBox();
    playerBox->setTitle("Local Players");
    playerLayout = new QGridLayout();
    playerTable = new QListWidget();
    addPlayerButton = new QPushButton();
    addPlayerButton->setText("Add New Player");
    connect(addPlayerButton, SIGNAL(pressed()), this, SLOT(addPlayer()));
    removePlayerButton = new QPushButton();
    removePlayerButton->setText("Remove Player");
    connect(removePlayerButton, SIGNAL(pressed()), this, SLOT(removePlayer()));
    playerLayout->addWidget(playerTable, 0, 0, 2, 1);
    playerLayout->addWidget(addPlayerButton, 0, 1, 1, 1);
    playerLayout->addWidget(removePlayerButton, 1, 1, 1, 1);
    playerBox->setLayout(playerLayout);

    layout->addWidget(playerBox, 2, 0, 1, 3);

    advancedButton = new QCommandLinkButton();
    advancedButton->setText("Advanced Options");
    advancedButton->setEnabled(false);
    connect(advancedButton, SIGNAL(pressed()), this, SLOT(openAdvancedOptions()));
    startButton = new QPushButton();
    startButton->setText("Start Game!");
    spacer = new QSpacerItem(30, 0, QSizePolicy::Expanding);
    connect(startButton, SIGNAL(pressed()), this, SLOT(startPressed()));
    layout->addWidget(advancedButton, 3, 0, 1, 1);
    layout->addItem(spacer, 3, 1, 1, 1);
    layout->addWidget(startButton, 3, 2, 1, 1);
    this->setLayout(layout);

    advancedOptions = NULL;
}

SettingsWidget::~SettingsWidget() {
}

void SettingsWidget::addPlayer() {
    QString playerName = QString::fromStdString("Player " + GameLibrary::IntToStr(playerTable->count()));
    QListWidgetItem* item = new QListWidgetItem();
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item->setText(playerName);
    playerTable->addItem(item);
}

void SettingsWidget::removePlayer() {
    delete playerTable->takeItem(playerTable->row(playerTable->currentItem()));
}

void SettingsWidget::openAdvancedOptions() {
    if (!advancedOptions) {
        advancedOptions = new AdvancedOptions();
    }
    advancedOptions->exec();
    advancedOptions->apply();
}

std::string SettingsWidget::getAdvancedOptionsSerial(){
    return advancedOptions->serialize();
}

void SettingsWidget::toggleNetworking(int state) {
    networkBox->setEnabled(state);
}

void SettingsWidget::toggleServer(int state) {
    ipLine->setEnabled(state == 0);
    advancedButton->setEnabled(state != 0);
}

void SettingsWidget::createPlayers() {
    int numPlayers = playerTable->count();
    for (int i = 0; i < numPlayers; i++) { //initialize Players
        std::string playerName = playerTable->item(i)->text().toStdString();
        Vector3f color = possiblePlayerColors[rand() % 8];
        Player* p = new Player(color, playerName);
        p->reset();
        GameLibrary::addPlayer(p);
    }
}

void SettingsWidget::startPressed() {
    if (playerTable->count() > 0) {
        emit startGame();
    } else {
        QMessageBox::information(this, "No players!", "You need to add at least one player to start!");
    }
}

QString SettingsWidget::getIP() {
    return ipLine->text();
}

QString SettingsWidget::getPort() {
    return tcpPortLine->text();
}

bool SettingsWidget::serverSet() {
    return serverCheckBox->isChecked();
}

