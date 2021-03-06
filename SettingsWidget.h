/* 
 * File:   SettingsMenu.h
 * Author: brandon
 *
 * Created on June 29, 2011, 10:51 AM
 */

#ifndef SETTINGSWIDGET_H
#define	SETTINGSWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QFormLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QSpacerItem>
#include <QtGui/QPushButton>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QListWidget>
#include "AdvancedOptions.h"

class SettingsWidget : public QWidget {
    Q_OBJECT
public:
    SettingsWidget(QWidget* parent = 0);
    virtual ~SettingsWidget();
    //void initialize();
    QString getIP();
    QString getPort();
    bool serverSet();
    void createPlayers();
    std::string getAdvancedOptionsSerial();

signals:
void startGame();
protected slots:
    void addPlayer();
    void removePlayer();
    void openAdvancedOptions();
    void startPressed();
    void toggleNetworking(int state);
    void toggleServer(int state);
private:
    AdvancedOptions* advancedOptions;
    
    QGridLayout* layout;
    QCheckBox* networkCheckBox;
    QGroupBox* networkBox;
    QFormLayout* networkLayout;
    QCheckBox* serverCheckBox;
    QLabel* ipLabel;
    QLineEdit* ipLine;
    QLabel* tcpPortLabel;
    QLineEdit* tcpPortLine;

    QGroupBox* playerBox;
    QGridLayout* playerLayout;
    QListWidget* playerTable;
    QPushButton* addPlayerButton;
    QPushButton* removePlayerButton;

    QCommandLinkButton* advancedButton;
    QPushButton* startButton;
    QSpacerItem* spacer;
};

#endif	/* SETTINGSWIDGET_H */

