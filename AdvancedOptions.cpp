/* 
 * File:   AdvancedOptions.cpp
 * Author: brandon
 * 
 * Created on July 17, 2011, 6:30 PM
 */

#include "AdvancedOptions.h"
#include "GameLibrary.h"

AdvancedOptions::AdvancedOptions() {
    this->setWindowTitle("Settlers | Advanced Options");
    layout = new QFormLayout();
    radiusLabel = new QLabel();
    radiusLabel->setText("Board radius (In tiles)");
    boardRadius = new QSpinBox();
    boardRadius->setRange(2, 11);
    boardRadius->setValue(GameLibrary::BOARDRADIUS);
    separationLabel = new QLabel();
    separationLabel->setText("Tile separation (1.8 is default)");
    tileSeparation = new QDoubleSpinBox();
    tileSeparation->setRange(1.7, 2);
    tileSeparation->setDecimals(1);
    tileSeparation->setValue(GameLibrary::TILE_SEPARATION);
    borderLabel = new QLabel();
    borderLabel->setText("Include a water border around the map?");
    waterBorder = new QCheckBox();
    waterBorder->setChecked(GameLibrary::WATER_BORDER);
    islandLabel = new QLabel();
    islandLabel->setText("Allow water in play area (islands)?");
    islands = new QCheckBox();
    islands->setChecked(GameLibrary::ISLANDS);
    bridgesLabel = new QLabel();
    bridgesLabel->setText("Allow bridges (roads across straits)?");
    bridges = new QCheckBox();
    bridges->setChecked(GameLibrary::BRIDGES);
    boatsLabel = new QLabel();
    boatsLabel->setText("Allow boats? (roads across water tiles)?");
    boats = new QCheckBox();
    boats->setChecked(true);
    startingResourcesLabel = new QLabel();
    startingResourcesLabel->setText("Gain resources surrounding first settlement at start of game?");
    startingResources = new QCheckBox();
    startingResources->setChecked(GameLibrary::STARTING_RESOURCES);
    layout->addRow(radiusLabel, boardRadius);
    layout->addRow(separationLabel, tileSeparation);
    layout->addRow(borderLabel, waterBorder);
    layout->addRow(islandLabel, islands);
    layout->addRow(bridgesLabel, bridges);
    layout->addRow(boatsLabel, boats);
    layout->addRow(startingResourcesLabel, startingResources);
    closeButton = new QPushButton();
    closeButton->setText("Close");
    connect(closeButton, SIGNAL(pressed()), this, SLOT(accept()));
    layout->addWidget(closeButton);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    this->setLayout(layout);
}

void AdvancedOptions::apply() {
    GameLibrary::BOARDRADIUS = boardRadius->value();
    GameLibrary::BOATS = boats->isChecked();
    GameLibrary::BRIDGES = bridges->isChecked();
    GameLibrary::ISLANDS = islands->isChecked();
    GameLibrary::STARTING_RESOURCES = startingResources->isChecked();
    GameLibrary::TILE_SEPARATION = tileSeparation->value();
    GameLibrary::WATER_BORDER = waterBorder->isChecked();
}

AdvancedOptions::~AdvancedOptions() {
    delete layout;
    delete radiusLabel;
    delete boardRadius;
    delete separationLabel;
    delete tileSeparation;
    delete borderLabel;
    delete waterBorder;
    delete islandLabel;
    delete islands;
    delete bridgesLabel;
    delete bridges;
    delete boatsLabel;
    delete boats;
    delete startingResourcesLabel;
    delete startingResources;
    delete closeButton;
}

