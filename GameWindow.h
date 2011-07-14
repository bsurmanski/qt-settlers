/* 
 * File:   GameWindow.h
 * Author: brandon
 *
 * Created on June 2, 2011, 12:00 PM
 */

#ifndef GAMEWINDOW_H
#define	GAMEWINDOW_H

#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QFont>
#include <QtGui/QMenuBar>
#include <QtGui/QLayout>

#include "GLFrame.h"

class GameWindow : public QWidget {
    
    Q_OBJECT
    
public:

    GameWindow(QWidget* parent = 0) : QWidget(parent) {
        GLFrame* glFrame = new GLFrame(this);
        QMenuBar* menuBar = new QMenuBar(this);

        /*
         * Menu Bar
         */

        // File Menu
        QMenu* fileMenu = new QMenu(tr("File"), this);
        QAction* exitAction = new QAction(tr("&Quit"), this); // Quit Option
        fileMenu->addAction(exitAction);
        menuBar->addMenu(fileMenu);
        connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

        QVBoxLayout *layout = new QVBoxLayout;
        layout->setMenuBar(menuBar);
        layout->addWidget(glFrame);
        setLayout(layout);
    }
};

#endif	/* GAMEWINDOW_H */

