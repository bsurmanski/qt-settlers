
#include <vector>
#include <dirent.h>
#include <QtOpenGL/QtOpenGL>
#include <QtGui/QStatusBar>
#include <assert.h>
#include <errno.h>
#include <iostream>

#include "GameLibrary.h"
#include "Model.h"
#include "Player.h"
#include "GameDockWidget.h"
#include "NetworkManager.h"

int GameLibrary::BOARDRADIUS = 5; //TODO: if boardsize > 12, throws on L81. fix
float GameLibrary::TILE_SEPARATION = 1.8; //1.7 is directly touching
bool GameLibrary::WATER_BORDER = true;
bool GameLibrary::ISLANDS = true;
bool GameLibrary::BRIDGES = true;
bool GameLibrary::BOATS = true;
bool GameLibrary::STARTING_RESOURCES = true;

GLuint GameLibrary::loadTexture(std::string filename) {
    filename = "Resources/" + filename;
    GLuint tex;

    glGenTextures(1, &tex); // Obtain an id for the texture
    glBindTexture(GL_TEXTURE_2D, tex); // Set as the current texture

    QImage im;
    assert(im.load(filename.c_str()));

    QImage texture = QGLWidget::convertToGLFormat(im);
    assert(!texture.isNull());

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return tex;
}

int GameLibrary::getdir(std::string dir, std::vector<std::string> &files) {
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(dir.c_str())) == NULL) {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(std::string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

void GameLibrary::cacheTextures() {
    textureCache = std::vector<Association <GLuint> > ();
    std::string ending = ".png";
    for (unsigned int i = 0; i < resourceDir.size(); i++) {
        if (resourceDir[i].length() > ending.length()) {
            if (0 == resourceDir[i].compare(resourceDir[i].length() - ending.length(), ending.length(), ending)) {
                GLuint tex = loadTexture(resourceDir[i]);
                Association<GLuint> a;
                a.key = resourceDir[i];
                a.value = tex;
                textureCache.push_back(a);
                //std::cout << "pushing texture:" << resourceDir[i] << ":" << tex << std::endl;
            }
        }
    }
}

GLuint GameLibrary::getCachedTexture(std::string name) {
    GLuint tex = -1;
    for (unsigned int i = 0; i < textureCache.size(); i++) {
        if (textureCache[i].key.compare(name) == 0) {
            for (unsigned int j = 0; j < resourceDir.size(); j++) {
                if (resourceDir[j].compare(name) == 0) {
                    tex = textureCache[i].value;
                    break;
                }
            }
        }
    }
    return tex;
}

void GameLibrary::cacheModels() {
    modelCache = std::vector<Association <Model*> > ();
    std::string ending = ".mdl";
    for (unsigned int i = 0; i < resourceDir.size(); i++) {
        if (resourceDir[i].length() > ending.length()) {
            if (0 == resourceDir[i].compare(resourceDir[i].length() - ending.length(), ending.length(), ending)) {
                Model* model = new Model(resourceDir[i]);
                Association<Model*> a;
                a.key = resourceDir[i];
                a.value = model;
                modelCache.push_back(a);
                //std::cout << "pushing model:" << resourceDir[i] << ":" << model << std::endl;
            }
        }
    }
}

Model* GameLibrary::getCachedModel(std::string name) {
    Model* model = NULL;
    for (unsigned int i = 0; i < modelCache.size(); i++) {
        if (modelCache[i].key.compare(name) == 0) {
            for (unsigned int j = 0; j < resourceDir.size(); j++) {
                if (resourceDir[j].compare(name) == 0) {
                    model = modelCache[i].value;
                    break;
                }
            }
        }
    }
    return model;
}

void GameLibrary::setPlayers(std::vector<Player*>* plrs) {
    players = plrs;
}

void GameLibrary::setCurrentPlayer(Player* player) {
    currentPlayer = player;
    for (int i = 0; i < getNumberOfPlayers(); i++) {
        if (player == players->at(i)) {
            currentPlayerIndex = i;
            player->beginTurn();
            break;
        }
    }
}

std::vector<Player*>* GameLibrary::getPlayers() {
    if (players == 0) {
        players = new std::vector<Player*>();
    }
    return players;
}

Player* GameLibrary::getPlayer(Vector3f color) {
    if (players) {
        for (int i = 0; i < players->size(); i++) {
            if (players->at(i)->getColor() == color) {
                return players->at(i);
            }
        }
    }
    return NULL;
}

void GameLibrary::addPlayer(Player* player) {
    if (players == 0) {
        players = new std::vector<Player*>();
    }
    players->push_back(player);
}

void GameLibrary::addStatusBarMessage(std::string message, int timeout) {
    statusBar->showMessage(QString::fromStdString(message), timeout);
}

int GameLibrary::getNumberOfPlayers() {
    if (players == 0) {
        players = new std::vector<Player*>();
    }
    return players->size();
}

void GameLibrary::nextPlayerTurn() {
    if (getCurrentLocalPlayer()->endTurn()) {
        Player* prev = getCurrentLocalPlayer();
        currentPlayerIndex = (currentPlayerIndex + 1) % getNumberOfPlayers();
        setCurrentPlayer(players->at(currentPlayerIndex));
        if (!prev->isRemote()) {
            getNetworkManager()->sendPackets("trn", getCurrentLocalPlayer()->serialize());
        }
    }
    getDockWidget()->updatePlayerInfo();
}

bool GameLibrary::checkLargestArmy(Player* player) {
    bool ret = false;
    bool someoneHas = false;
    if (player->getLargestArmy() >= 3) {
        for (int i = 0; i < numPlayers; i++) {
            Player* current = players->at(i);
            if (current->hasLargestArmy()) {
                someoneHas = true;
                if (player == current) {
                    ret = true;
                }
                if (player->getLargestArmy() > current->getLargestArmy()) {
                    player->setLargestArmy(true);
                    current->setLargestArmy(false);
                    ret = true;
                }
            }
        }
        if (!someoneHas) {
            player->setLargestArmy(true);
        }
    }
    return ret;
}

bool GameLibrary::checkLongestRoad(Player* player) {
    bool ret = false;
    bool someoneHas = false;
    if (player->getLongestRoad() >= 5) {
        for (int i = 0; i < numPlayers; i++) {
            Player* current = players->at(i);
            if (current->hasLongestRoad()) {
                someoneHas = true;
                if (player == current) {
                    ret = true;
                }
                if (player->getLongestRoad() > current->getLongestRoad()) {
                    player->setLongestRoad(true);
                    current->setLongestRoad(false);
                    GameLibrary::getDockWidget()->updatePlayerInfo();
                    ret = true;
                }
            }
        }
        if (!someoneHas) {
            player->setLongestRoad(true);
        }
    }
    return ret;
}


int GameLibrary::numPlayers = 0;
int GameLibrary::networkPlayers = 0;
int GameLibrary::currentPlayerIndex = 0;

bool GameLibrary::server = false;


GameLibrary* GameLibrary::instance = 0;
Player* GameLibrary::currentPlayer = 0;
std::vector<Player*>* GameLibrary::players = 0;
GameDockWidget* GameLibrary::currentDockWidget = 0;
GLFrame* GameLibrary::currentGLWidget = 0;
NetworkManager* GameLibrary::networkManager = 0;
MainGameWindow* GameLibrary::mainWindow = 0;
SettingsWidget* GameLibrary::settingsWidget = 0;

QStatusBar* GameLibrary::statusBar = 0;
Board* GameLibrary::currentBoard = 0;