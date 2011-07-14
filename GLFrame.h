/* 
 * File:   GLWindow.h
 * Author: brandon
 *
 * Created on June 1, 2011, 3:14 PM
 */

#ifndef GLWINDOW_H
#define	GLWINDOW_H

#define MENU 0
#define INGAME 1
#define TRADE 2
#define PAUSE 3

#include <Qt/qgl.h>
#include <QtGui/QKeyEvent>
#include <QtCore/QTimer>

#include "Board.h"
#include "Player.h"
#include "TradeMenu.h"
#include "PauseMenu.h"
#include "Sprite.h"

#include "vmath.h"

class GLFrame : public QGLWidget {
    Q_OBJECT
public:
    GLFrame(QWidget* parent = 0);
    virtual ~GLFrame();

    void newGame();

signals:
    void playerInfoUpdate();

protected slots:
    void update();




private:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void buildLists();

    void keyPressEvent(QKeyEvent* q);
    void keyReleaseEvent(QKeyEvent* q);
    void enterEvent(QEvent* ev);

    void initResources();

    void drawPlayerPorts();
    void drawGUI();

    int state;

    Vector3f globalRotation;
    Vector3f globalPosition;
    Board* board;
    GLuint boardList;
    std::vector<Player*>* players;

    PauseMenu* pauseMenu;

    Vector2i dice;

    Sprite* portIcons;
    QTimer* timer;
};

#endif	/* GLWINDOW_H */

