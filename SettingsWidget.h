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
#include <QtGui/QTableWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QSpacerItem>
#include <QtGui/QPushButton>

class SettingsWidget : public QWidget {
    Q_OBJECT
public:
    SettingsWidget(QWidget* parent = 0);
    virtual ~SettingsWidget();
    //void initialize();

signals:
void startGame();
protected slots:
    void startPressed();
    void toggleNetworking(int state);
    void toggleServer(int state);
private:
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
    QTableWidget* playerTable;
    QPushButton* addPlayerButton;
    QPushButton* removePlayerButton;

    QPushButton* startButton;
    QSpacerItem* spacer;
};

#endif	/* SETTINGSWIDGET_H */

