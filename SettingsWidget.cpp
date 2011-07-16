/* 
 * File:   SettingsWidget.cpp
 * Author: brandon
 * 
 * Created on June 29, 2011, 10:51 AM
 */

#include "SettingsWidget.h"
#include "GameLibrary.h"

SettingsWidget::SettingsWidget(QWidget* parent) : QWidget(parent) {
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
    layout->addWidget(networkBox, 1, 0, 1, 2);

    playerBox = new QGroupBox();
    playerBox->setTitle("Local Players");
    playerLayout = new QGridLayout();
    playerTable = new QTableWidget();
    playerTable->setColumnCount(2);
    QStringList columns;
    columns.append("Name");
    columns.append("Color");
    playerTable->setHorizontalHeaderLabels(columns);
    addPlayerButton = new QPushButton();
    addPlayerButton->setText("Add Player");
    removePlayerButton = new QPushButton();
    removePlayerButton->setText("Remove Player");
    playerLayout->addWidget(playerTable, 0, 0, 2, 1);
    playerLayout->addWidget(addPlayerButton, 0, 1, 1, 1);
    playerLayout->addWidget(removePlayerButton, 1, 1, 1, 1);
    playerBox->setLayout(playerLayout);
    layout->addWidget(playerBox, 2, 0, 1, 2);
    startButton = new QPushButton();
    startButton->setText("Start Game!");
    connect(startButton, SIGNAL(pressed()), this, SLOT(startPressed()));
    layout->addWidget(startButton, 3, 1, 1, 1);
    this->setLayout(layout);
}

SettingsWidget::~SettingsWidget() {
}

//void SettingsWidget::initialize() {
//    int rows = playerTable->rowCount();
//    int cols = playerTable->columnCount();
//    QString cellText;
//    for (int i = 0; i < rows; i++) {
//        for (int j = 0; j < cols; j++) {
//            cellText = playerTable->takeItem(i, j)->text();
//        }
//    }
//}

void SettingsWidget::toggleNetworking(int state) {
    networkBox->setEnabled(state);
}

void SettingsWidget::toggleServer(int state) {
    ipLine->setEnabled(state == 0);
}

void SettingsWidget::startPressed() {
    emit startGame();
}

