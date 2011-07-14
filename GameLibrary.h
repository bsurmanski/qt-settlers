/* 
 * File:   GameLibrary.h
 * Author: brandon
 *
 * Created on June 9, 2011, 2:08 PM
 */

#ifndef GAMELIBRARY_H
#define	GAMELIBRARY_H

#include "vmath.h"
#include <vector>
#include <sstream>
#include <QtOpenGL/QtOpenGL>

//math
#define PI M_PI

//options
#define MAX_PLAYERS 8
#define BOARDRADIUS 5 //TODO: if boardsize > 12, throws on L81. fix
#define TILE_SEPARATION 1.8
#define WATER_BORDER true
#define ISLANDS true
#define BRIDGES true
#define BOATS true
#define STARTING_RESOURCES true

//resources
#define NUM_RESOURCES 5

#define WATER -2
#define DESERT -1
#define WOOD 0
#define SHEEP 1
#define WHEAT 2
#define BRICK 3
#define ROCK 4

//Development cards
#define DEVCARD_TYPES 5
#define KNIGHT 0
#define ROAD_BUILD 1
#define YEAR_PLENTY 2
#define MONOPOLY 3
#define VICTORY_POINT 4
#define NUM_CARDS 14+2+2+2+5
#define NUM_EACH_CARD_TYPE {14,2,2,2,5}


//Love :)
#define Natasha <3

static const Vector3f WATER_COLOR = Vector3f(0, 0.2, 0.8);
static const Vector3f DESERT_COLOR = Vector3f(0.95, 0.95, 0.7);
static const Vector3f WOOD_COLOR = Vector3f(0.1, 0.3, 0.1);
static const Vector3f SHEEP_COLOR = Vector3f(0.2, 0.6, 0.3);
static const Vector3f WHEAT_COLOR = Vector3f(0.6, 0.6, 0.1);
static const Vector3f BRICK_COLOR = Vector3f(0.6, 0.2, 0.1);
static const Vector3f ROCK_COLOR = Vector3f(0.4, 0.4, 0.4);

const Vector3f possiblePlayerColors [] = {
    Vector3f(1, 0, 0),
    Vector3f(0, 0, 0.6),
    Vector3f(0.7, 0.7, 0),
    Vector3f(1, 1, 1),
    Vector3f(0, 1, 0),
    Vector3f(0.5, 0.2, 0.2),
    Vector3f(0.4, 0.4, 0.4),
    Vector3f(0.6, 0.1, 0.6)
};

class Model;
class Player;
class GameDockWidget;
class GLFrame;
class Board;
class NetworkManager;
class MainGameWindow;

class QStatusBar;

class GameLibrary {
public:

    GameLibrary() {
        GameLibrary::resourceDir = std::vector<std::string > ();
        GameLibrary::getdir("Resources/", GameLibrary::resourceDir);
        GameLibrary::cacheModels();
        GameLibrary::instance = this;
    }

    static GameLibrary* getInstance() {
        if (GameLibrary::instance == 0)
            GameLibrary::instance = new GameLibrary();
        return GameLibrary::instance;
    }

    static Player* getCurrentLocalPlayer() {
        return currentPlayer;
    }

    static std::vector<Player*>* getPlayers();
    static void setPlayers(std::vector<Player*>* plrs); 
    static void setCurrentPlayer(Player* player);
    static void addPlayer(Player* player);

    static GameDockWidget* getDockWidget() {
        return currentDockWidget;
    }

    static void setCurrentDockWidget(GameDockWidget* dock) {
        currentDockWidget = dock;
    }

    static void setCurrentMainWindow(MainGameWindow* win) {
        mainWindow = win;
    }

    static MainGameWindow* getCurrentMainWindow() {
        return mainWindow;
    }

    static GLFrame* getGLFrame() {
        return currentGLWidget;
    }

    static void setCurrentGLFrame(GLFrame* frame) {
        currentGLWidget = frame;
    }

    static void setNetworkManager(NetworkManager* nm) {
        networkManager = nm;
    }

    static NetworkManager* getNetworkManager() {
        return networkManager;
    }

    static bool isServer() {
        return server;
    }

    static void setServer(bool s) {
        server = s;
    }

    static void setQStatusBar(QStatusBar* bar) {
        statusBar = bar;
    }

    static Board* getCurrentBoard() {
        return currentBoard;
    }

    static void setCurrentBoard(Board* newBoard) {
        currentBoard = newBoard;
    }

    static void addStatusBarMessage(std::string message, int timeout);

    static int getResourceForColor(Vector3f color) {
        if (color == WATER_COLOR) {
            return WATER;
        } else if (color == DESERT_COLOR) {
            return DESERT;
        } else if (color == WOOD_COLOR) {
            return WOOD;
        } else if (color == SHEEP_COLOR) {
            return SHEEP;
        } else if (color == WHEAT_COLOR) {
            return WHEAT;
        } else if (color == BRICK_COLOR) {
            return BRICK;
        } else if (color == ROCK_COLOR) {
            return ROCK;
        }
        return -1;
    }

    static Vector3f getColorFor(int resource) {
        switch (resource) {
            case WATER:
                return WATER_COLOR;
                break;

            case DESERT:
                return DESERT_COLOR;
                break;

            case WOOD:
                return WOOD_COLOR;
                break;

            case SHEEP:
                return SHEEP_COLOR;
                break;

            case WHEAT:
                return WHEAT_COLOR;
                break;

            case BRICK:
                return BRICK_COLOR;
                break;

            case ROCK:
                return ROCK_COLOR;
                break;
            default:
                return DESERT_COLOR;
        }
        return Vector3f();
    }

    static int getResource(std::string resource) {
        if (resource.compare("wood") == 0) {
            return 0;
        } else if (resource.compare("sheep") == 0) {
            return 1;
        } else if (resource.compare("wheat") == 0) {
            return 2;
        } else if (resource.compare("brick") == 0) {
            return 3;
        } else if (resource.compare("rock") == 0) {
            return 4;
        }
        return -1;
    }

    static std::string getResourceString(int resource) {
        switch (resource) {
            case 0:
                return "wood";
            case 1:
                return "sheep";
            case 2:
                return "wheat";
            case 3:
                return "brick";
            case 4:
                return "rock";
        }
        return "";
    }

    static std::string IntToStr(int n) {
        std::ostringstream result;
        result << n;
        return result.str();
    }

    static std::string colorToStr(Vector3f color) {
        std::ostringstream result;
        result << color.r << " " << color.g << " " << color.b;
        return result.str();
    }

    static bool checkLargestArmy(Player* player);
    static bool checkLongestRoad(Player* player);

    static void setNumberOfPlayers(int num);
    static int getNumberOfPlayers();
    static void nextPlayerTurn();

    GLuint getCachedTexture(std::string name);
    Model* getCachedModel(std::string name);

private:

    template <class T>
    struct Association {
        std::string key;
        T value;
    };

    GLuint loadTexture(std::string filename);

    int getdir(std::string dir, std::vector<std::string> &files);

    void cacheModels();
    void cacheTextures();

    static int localPlayers;
    static int networkPlayers;
    static int currentPlayerIndex;
    static bool server;

    static GameLibrary* instance;
    static Player* currentPlayer;
    static std::vector<Player*>* players;
    static GameDockWidget* currentDockWidget;
    static GLFrame* currentGLWidget;
    static Board* currentBoard;
    static NetworkManager* networkManager;
    static MainGameWindow* mainWindow;

    static QStatusBar* statusBar;

    std::vector<Association< GLuint> > textureCache;
    std::vector<Association< Model*> > modelCache;
    std::vector<std::string> resourceDir;

    friend class GLFrame;
};

#endif	/* GAMELIBRARY_H */

