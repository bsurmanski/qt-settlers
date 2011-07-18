/* 
 * File:   MainGameWindow.h
 * Author: brandon
 *
 * Created on June 27, 2011, 10:45 PM
 */

#ifndef MAINGAMEWINDOW_H
#define	MAINGAMEWINDOW_H

#include <QtGui/QMainWindow>

class QSplitter;
class GameDockWidget;
class GLFrame;
class StatisticsWindow;
class NetworkManager;
class SettingsWidget;

class MainGameWindow : public QMainWindow {
    Q_OBJECT
public:
    MainGameWindow();
    virtual ~MainGameWindow();
    
    bool client_start();

protected slots:
    void openStatistics();
    void startGame();
private:
    QMenuBar* menuBar;
    QStatusBar* statusBar;
    QMenu* menuFile;
    QAction* actionQuit;
    QMenu* menuWindow;
    QAction* actionStatistics;
    QMenu* menuHelp;
    QSplitter* splitterWidget;
    GameDockWidget* dockContent;
    GLFrame* gameFrame;
    
    StatisticsWindow* stat;
    SettingsWidget* settingsWidget;
    NetworkManager* networker;

};

#endif	/* MAINGAMEWINDOW_H */

