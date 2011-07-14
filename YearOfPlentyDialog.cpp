/* 
 * File:   YearOfPlentyDialog.cpp
 * Author: brandon
 * 
 * Created on June 22, 2011, 9:40 PM
 */

#include "YearOfPlentyDialog.h"
#include "GameLibrary.h"

YearOfPlentyDialog::YearOfPlentyDialog(QWidget* parent) : QDialog(parent) {
    ok = false;
    this->setWindowTitle("Settlers | Year of Plenty");
    layout = new QGridLayout(this);
    text = new QLabel(this);
    text->setText("Choose two resources to aquire");
    QStringList resources = QStringList();
    for (int i = 0; i < NUM_RESOURCES; i++) {
        resources.append(QString::fromStdString(GameLibrary::getResourceString(i)));
    }
    first = new QComboBox(this);
    first->setEditable(false);
    first->clear();
    first->addItems(resources);
    second = new QComboBox(this);
    second->setEditable(false);
    second->clear();
    second->addItems(resources);
    buttonBox = new QDialogButtonBox(this);
    buttonBox->setOrientation(Qt::Horizontal);
    connect (buttonBox, SIGNAL(accepted()),this,(SLOT(accept())));
    connect(buttonBox,SIGNAL(rejected()),this, SLOT(cancel()));

    buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    layout->addWidget(text, 0, 0, 1, 2);
    layout->addWidget(first, 1, 0, 1, 1);
    layout->addWidget(second, 1, 1, 1, 1);
    layout->addWidget(buttonBox, 2, 0, 1, 2);
    this->setLayout(layout);
}

YearOfPlentyDialog::~YearOfPlentyDialog() {
    delete layout;
    delete text;
    delete first;
    delete second;
    delete buttonBox;
}

QPair<int, int> YearOfPlentyDialog::getResourcePair(QWidget* parent) {
    YearOfPlentyDialog dialog(parent);
    dialog.exec();
    return QPair<int, int>(dialog.ok ? dialog.first->currentIndex() : -1, dialog.ok ? dialog.second->currentIndex() : -1);
}

void YearOfPlentyDialog::accept() {
    ok = true;
    this->close();
}

void YearOfPlentyDialog::cancel() {
    ok = false;
    this->close();
}


