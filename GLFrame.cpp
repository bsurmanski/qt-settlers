/*
 * File:   GLFrame.cpp
 * Author: brandon
 *
 * Created on June 1, 2011, 3:14 PM
 */

#include <sstream>

#include "GLFrame.h"
#include "PauseMenu.h"
#include "GameLibrary.h"
#include "GameDockWidget.h"
#include "NetworkManager.h"

GLFrame::GLFrame(QWidget* parent) : QGLWidget(parent) {
    GameLibrary::setCurrentGLFrame(this);
    setMinimumSize(QSize(320, 240));

    globalRotation = Vector3f(-40, 0, 0);
    globalPosition = Vector3f(0, 5, -30);

    //GameLibrary::setNumberOfPlayers(3);
    //networkPlayers = 0;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(20);
    setFocus();
}

GLFrame::~GLFrame() {
    delete timer;

    delete pauseMenu;
    delete board;
    while (!players->empty()) {
        delete players->back();
        players->pop_back();
    }
    delete players;
}

void GLFrame::initializeGL() //call this right after our OpenGL window is created.
{
    GameLibrary::getInstance()->cacheModels();
    GameLibrary::getInstance()->cacheTextures();

    buildLists();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // This Will Clear The Background Color To Black
    glClearDepth(1.0); // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS); // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH); // Enables Smooth Color Shading
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLFrame::buildLists() {
    board->buildLists();
}

void GLFrame::resizeGL(int width, int height) {
    int parentHeight = parentWidget()->geometry().height();

    if (parentHeight == 0) { // Prevent A Divide By Zero If The Window Is Too Small
        height = 1;
        parentHeight = 1;
    }

    glEnable(GL_TEXTURE_2D);
    glViewport(0, height - parentHeight, width, parentHeight); // Reset The Current Viewport And Perspective Transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat) width / (GLfloat) parentHeight, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLFrame::newGame() {
    srand(time(NULL));

    state = INGAME;

    int numPlayers = 1;
    if (GameLibrary::getNetworkManager()->isServer()) {
        board = new Board();

        for (int i = 0; i < numPlayers; i++) { //initialize Players
            GameLibrary::addPlayer(new Player(possiblePlayerColors[rand() % 8]));
        }

        players = GameLibrary::getPlayers();

        GameLibrary::setCurrentPlayer(players->at(rand() % numPlayers));
        GameLibrary::getCurrentLocalPlayer()->beginTurn();
    } else {
        board = GameLibrary::getCurrentBoard();
        numPlayers = GameLibrary::getNumberOfPlayers();
        for (int i = 0; i < numPlayers; i++) { //initialize Players
            GameLibrary::addPlayer(new Player(possiblePlayerColors[rand() % 8]));
        }
        players = GameLibrary::getPlayers();
    }

    dice = Vector2i();

    portIcons = new Sprite("anchor.png");
    portIcons->setSize(Vector2f(0.05, 0.05));

    pauseMenu = new PauseMenu();
}

void GLFrame::update() {
    switch (state) {
        case MENU:
            break;

        case PAUSE:

            if (pauseMenu->Update()) updateGL();

        case INGAME:
            if (board->Update()) updateGL();
            for (int i = 0; i < GameLibrary::getNumberOfPlayers(); i++) {
                if (players->at(i)->Update()) updateGL();
            }
            break;
    }
    // updateGL();
}

void GLFrame::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    switch (state) {
        case MENU:
            break;
        case PAUSE:
            pauseMenu->Draw();
            break;

        case INGAME:
            glPushMatrix(); // add a view matrix to the stack
            glTranslatef(globalPosition.x, globalPosition.y, globalPosition.z); // move the camera 20 units backwards
            glRotatef(globalRotation.x, 1, 0, 0);
            glRotatef(globalRotation.y, 0, 1, 0);
            glRotatef(globalRotation.z, 0, 0, 1);
            glCallList(boardList);
            board->Draw();
            for (int i = 0; i < GameLibrary::getNumberOfPlayers(); i++)
                players->at(i)->Draw();
            glPopMatrix(); // remove a view matrix from the stack
            drawGUI();
            break;
    }
}

void GLFrame::drawGUI() {
    glDisable(GL_DEPTH_TEST);
    switch (state) {
        case INGAME:
            glPushMatrix();
            glTranslatef(0.375, 0.65, -2);
            drawPlayerPorts();
            glPopMatrix();
            break;
    }
    glEnable(GL_DEPTH_TEST);
}

void GLFrame::drawPlayerPorts() { // draw owned ports of the player
    Player* player = GameLibrary::getCurrentLocalPlayer();
    for (int i = 0; i < 6; i++) {
        glTranslatef(0.1, 0, 0);
        if (player && player->hasPort(i)) {
            portIcons->setTint(GameLibrary::getColorFor(i));
        } else {
            portIcons->setTint(Vector3f(0.05, 0.05, 0.05));
        }
        portIcons->Draw();
    }
}

void GLFrame::keyPressEvent(QKeyEvent* q) {
    bool remotePlayer = GameLibrary::getCurrentLocalPlayer()->isRemote();
    int selectionDir = -1;

    if (state == TRADE) {
        updateGL();
        if (q->key() != Qt::Key_Escape)
            return;
    }
    switch (q->key()) {
        case Qt::Key_Escape:
            if (state == INGAME) {
                state = PAUSE;
                GameLibrary::getDockWidget()->hide();
            } else {
                state = INGAME;
                GameLibrary::getDockWidget()->show();
            }
            break;

        case Qt::Key_Return:
            // connect((QObject*) GameLibrary::getCurrentLocalPlayer(), SIGNAL(statusChanged()), (QObject*) GameLibrary::getDockWidget(), SLOT(updatePlayerInfo()));
            if (state == INGAME && !remotePlayer) {
                GameLibrary::nextPlayerTurn();
            }
            break;

        case Qt::Key_Space:
            /*
            delete board;
            board = new Board();
            initializeGL();
            // board->buildLists();
            for (int i = 0; i < GameLibrary::getNumberOfPlayers(); i++) {
                players->at(i)->reset();
            }*/
            break;

        case Qt::Key_8: // up
            globalPosition.y += 1;
            break;
        case Qt::Key_2: // down
            globalPosition.y -= 1;
            break;
        case Qt::Key_4: // left
            globalPosition.x -= 1;
            break;
        case Qt::Key_6: // right
            globalPosition.x += 1;
            break;
        case Qt::Key_7: // rotate left
            globalRotation.z += 5;
            break;
        case Qt::Key_9: // rotate right
            globalRotation.z -= 5;
            break;
        case Qt::Key_1: // spin up
            if (globalRotation.x < 0)
                globalRotation.x += 5;
            break;
        case Qt::Key_3: // spin down
            if (globalRotation.x > -90)
                globalRotation.x -= 5;
            break;
        case Qt::Key_5: // reset view
            globalRotation = Vector3f(-40, 0, 0);
            globalPosition = Vector3f(0, 0, -30);
            break;
        case Qt::Key_Plus: // zoom view in
            if (globalPosition.z < -5)
                globalPosition.z++;
            break;
        case Qt::Key_Minus: // zoom view out
            if (globalPosition.z > -70)
                globalPosition.z--;
            break;
        case Qt::Key_R:
            if (!remotePlayer) {
                dice = GameLibrary::getCurrentLocalPlayer()->rollDice();
                GameLibrary::getNetworkManager()->sendPackets("rol", GameLibrary::IntToStr(dice.s) + " " + GameLibrary::IntToStr(dice.t));
                if (dice.s + dice.t == 7) {
                    int randPos = 0;
                    while (!board->moveRobber((randPos = (rand() % board->getNumberOfTiles()))));
                    GameLibrary::getNetworkManager()->sendPackets("rbr", GameLibrary::IntToStr(randPos));
                } else {
                    for (int i = 0; i < GameLibrary::getNumberOfPlayers(); i++) {
                        players->at(i)->CollectResourcesOn(dice.s + dice.t);
                    }
                }
            }
            break;
        case Qt::Key_X: // upgrade
            if (!remotePlayer) {
                GameLibrary::getCurrentLocalPlayer()->upgradeSettlement();
            }
            break;
        case Qt::Key_Z: // place settlement
            if (!remotePlayer) {
                GameLibrary::getCurrentLocalPlayer()->placeSettlement();
            }
            break;
        case Qt::Key_A: // place road
            if (!remotePlayer) {
                GameLibrary::getCurrentLocalPlayer()->placeRoad();
            }
            break;


        case Qt::Key_Up:
            selectionDir = 2;
            break;
        case Qt::Key_Down:
            selectionDir = 0;
            break;
        case Qt::Key_Left:
            selectionDir = 3;
            break;
        case Qt::Key_Right:
            selectionDir = 1;
            break;
    }

    if (selectionDir != -1) {
        switch (state) {
            case INGAME:
                GameLibrary::getCurrentLocalPlayer()->select(selectionDir);
                break;
        }
    }

    updateGL();
}

void GLFrame::keyReleaseEvent(QKeyEvent* q) {
    switch (q->key()) {
        case Qt::Key_Up:
        case Qt::Key_Down:
            break;
        case Qt::Key_Left:
        case Qt::Key_Right:
            break;
    }
}

void GLFrame::enterEvent(QEvent* ev) {
    QGLWidget::enterEvent(ev);
    setFocus();
}


