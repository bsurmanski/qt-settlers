/* 
 * File:   StatisticsWindow.cpp
 * Author: brandon
 * 
 * Created on June 27, 2011, 10:50 PM
 */

#include "StatisticsWindow.h"
#include "GameLibrary.h"
#include "Player.h"

StatisticsWindow::StatisticsWindow() : QDialog() {
    this->setWindowTitle("Settlers | Statistics");
    longestLabel = new QLabel();
    longestLabel->setText("Longest Road:");
    longestNum = new QLineEdit();
    longestNum->setText(QString::fromStdString(GameLibrary::IntToStr(GameLibrary::getCurrentLocalPlayer()->getLongestRoad())));
    longestNum->setReadOnly(true);

    knightsLabel = new QLabel();
    knightsLabel->setText("Knights Played:");
    knightsNum = new QLineEdit();
    knightsNum->setText(QString::fromStdString(GameLibrary::IntToStr(GameLibrary::getCurrentLocalPlayer()->getKnightsPlayed())));
    knightsNum->setReadOnly(true);

    layout = new QFormLayout();
    layout->addRow(longestLabel, longestNum);
    layout->addRow(knightsLabel, knightsNum);
    this->setLayout(layout);
}

StatisticsWindow::~StatisticsWindow() {
}

