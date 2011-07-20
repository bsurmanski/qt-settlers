/*
 * File:   NetworkManager.cpp
 * Author: brandon
 *
 * Created on July 3, 2011, 5:26 PM
 */

#include "NetworkManager.h"
#include "GameLibrary.h"
#include "GameDockWidget.h"
#include "Board.h"
#include "MainGameWindow.h"
#include "SettingsWidget.h"
#include <QtGui/QMessageBox>
#include <string>

NetworkManager::NetworkManager(QObject* parent) : QObject::QObject(parent) {
    tcpClient = NULL;
    tcpServer = NULL;
    server_clients = NULL;
    connected = false;
}

void NetworkManager::setConnection(const QString ip, const int port, const bool server) {
    connected = server;
    this->server = server;
    this->ip = ip;
    this->port = port;
    if (server) {
        tcpServer = new QTcpServer();
        if (!tcpServer->listen(QHostAddress::Any, port)) {
            this->port = -1;
        }
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(server_newConnect()));
        server_clients = new QList<QTcpSocket*>();
        //connect(this, SIGNAL(server_createMessage(std::string)), this, SLOT(server_sendMsg(std::string)));
    } else {
        tcpClient = new QTcpSocket();
        client_getConnect();
    }
}

NetworkManager::~NetworkManager() {
    if (tcpClient) {
        delete tcpClient;
    }
    if (tcpServer) {
        delete tcpServer;
    }
    if (server_clients) {
        delete server_clients;
    }
}

bool NetworkManager::isServer() {
    return server;
}

bool NetworkManager::isConnected() {
    return connected;
}

void NetworkManager::sendPackets(std::string code, std::string msg) {
    msg += ";";
    if (server) {
        for (int i = 0; i < server_clients->size(); i++) {
            msg = code + msg;
            server_clients->at(i)->write(msg.c_str(), msg.length());
        }
    } else if (tcpClient->isOpen()) {
        msg = code + msg;
        tcpClient->write(msg.c_str(), msg.length());
    }
}

void NetworkManager::sendPackets(std::string concat, QTcpSocket* client) {
    concat += ";";
    client->write(concat.c_str(), concat.length());
}

void NetworkManager::server_newConnect() {
    QTcpSocket* client = tcpServer->nextPendingConnection();
    std::vector<Player*>* players = GameLibrary::getPlayers();
    server_clients->append(client);
    sendPackets("adv" + GameLibrary::getSettingsWidget()->getAdvancedOptionsSerial(), client);
    sendPackets("brd" + GameLibrary::getCurrentBoard()->serialize(), client);
    for (unsigned int i = 0; i < players->size(); i++) {
        sendPackets("plr" + players->at(i)->serialize(), client);
    }
    sendPackets("trn", GameLibrary::getCurrentLocalPlayer()->serialize());
    sendPackets("bgn", client);
    connect(client, SIGNAL(readyRead()), this, SLOT(server_parsePackets()));
}

void NetworkManager::client_getConnect() {
    tcpClient->connectToHost(ip, port);
    std::vector<Player*>* players = GameLibrary::getPlayers();
    for (unsigned int i = 0; i < players->size(); i++) {
        sendPackets("plr", players->at(i)->serialize());
    }
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(client_parsePackets()));
}

void NetworkManager::server_parsePackets() {
    std::string msg;

    QTcpSocket* sender;
    for (int i = 0; i < server_clients->size(); i++) {
        if (server_clients->at(i)->bytesAvailable() > 0) {
            sender = server_clients->at(i);
            msg = readPackets(sender);
            break;
        }
    }
    for (int i = 0; i < server_clients->size(); i++) {
        if (server_clients->at(i) != sender) {
            sendPackets(msg, server_clients->at(i));
        }
    }
}

void NetworkManager::parsePackets(std::string msg) {
    int n;
    char* nextmsg = 0;
    if ((n = msg.find(';')) < msg.npos) { // handle multiple messages
        int nextMsgSize = msg.size() - n;
        nextmsg = new char[nextMsgSize];
        memcpy(nextmsg, (msg.data() + n + 1), nextMsgSize);
    }

    msg = msg.substr(0, n);

    std::string code = msg.substr(0, 3);
    msg.erase(0, 3);

    if (code.compare("msg") == 0) { // chat message
        GameLibrary::getDockWidget()->newChatMessage(msg);

    } else if (code.compare("rbr") == 0) { //move robber
        int tile = atoi(msg.c_str());
        GameLibrary::getCurrentBoard()->moveRobber(tile);

    } else if (code.compare("set") == 0) { // settlement
        const char* dataptr = msg.data();
        Vector3f color = Vector3f();
        color.r = atof(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        color.g = atof(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        color.b = atof(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        int tile = atoi(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        int corner = atoi(dataptr);
        Player* player = GameLibrary::getPlayer(color);
        if (player) {
            player->placeSettlement(tile, corner);
        }
    } else if (code.compare("upg") == 0) { //upgrade settlement
        const char* dataptr = msg.data();
        Vector3f color = Vector3f();
        color.r = atof(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        color.g = atof(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        color.b = atof(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        int tile = atoi(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        int corner = atoi(dataptr);
        Player* player = GameLibrary::getPlayer(color);
        if (player) {
            player->upgradeSettlement(tile, corner);
        }
    } else if (code.compare("rol") == 0) { //dice roll
        std::vector<Player*>* players = GameLibrary::getPlayers();
        for (int i = 0; i < GameLibrary::getNumberOfPlayers(); i++) {//LOCAL PLAYERS
            players->at(i)->CollectResourcesOn(atoi(msg.substr(0, 1).c_str()) + atoi(msg.substr(2, 1).c_str()));
        }
    } else if (code.compare("brd") == 0) { // board
        delete GameLibrary::getCurrentBoard();
        GameLibrary::setCurrentBoard(new Board(msg.c_str(), msg.length()));
        GameLibrary::getCurrentBoard()->buildLists();
        std::vector<Player*>* players = GameLibrary::getPlayers();
        if (players != NULL) {
            for (unsigned int i = 0; i < players->size(); i++) {
                players->at(i)->reset();
            }
        }
    } else if (code.compare("rd ") == 0) { // road
        const char* dataptr = msg.data();
        Vector3f color = Vector3f();
        color.r = atof(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        color.g = atof(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        color.b = atof(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        int tile = atoi(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        int corner = atoi(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        int direction = atoi(dataptr);
        Player* player = GameLibrary::getPlayer(color);
        if (player) {
            player->placeRoad(tile, corner, direction);
        }
    } else if (code.compare("mkt") == 0) { // make trade
    } else if (code.compare("rmt") == 0) { //remove trade
    } else if (code.compare("acp") == 0) { //accept trade
    } else if (code.compare("plr") == 0) { //define player
        const char* dataptr = msg.data();
        Vector3f color = Vector3f();
        color.r = atof(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        color.g = atof(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        color.b = atof(dataptr);
        dataptr = (strchr(dataptr, ':') + 1);
        std::string name = std::string(dataptr);
        Player* p = new Player(color, name);
        p->reset();
        p->setCurrentBoard(GameLibrary::getCurrentBoard());
        p->setRemote(true);
        GameLibrary::addPlayer(p);

    } else if (code.compare("win") == 0) { //player declares victory
    } else if (code.compare("die") == 0) { //player quits
    } else if (code.compare("trn") == 0) { //start player's turn
        const char* dataptr = msg.data();
        Vector3f color = Vector3f();
        color.r = atof(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        color.g = atof(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        color.b = atof(dataptr);
        dataptr = (strchr(dataptr, ':') + 1);
        std::string name = std::string(dataptr);
        Player* player = GameLibrary::getPlayer(color);
        //if player->getName()
        if (player) {
            if (GameLibrary::getCurrentLocalPlayer()) {
                GameLibrary::getCurrentLocalPlayer()->endTurn();
            }
            GameLibrary::setCurrentPlayer(player);
            player->beginTurn();
        }
    } else if (code.compare("bgn") == 0) { //start game
        connected = true;
        GameLibrary::getCurrentMainWindow()->client_start();
    } else if (code.compare("rme") == 0) { //army

    } else if (code.compare("adv") == 0) { //advanced options
        const char* dataptr = msg.data();
        GameLibrary::BOARDRADIUS = atoi(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        GameLibrary::BOATS = atoi(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        GameLibrary::BRIDGES = atoi(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        GameLibrary::ISLANDS = atoi(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        GameLibrary::STARTING_RESOURCES = atoi(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        GameLibrary::TILE_SEPARATION = atoi(dataptr);
        dataptr = (strchr(dataptr, ' ') + 1);
        GameLibrary::WATER_BORDER = atoi(dataptr);
    }

    if (nextmsg != 0) {
        parsePackets(std::string(nextmsg));
        delete nextmsg;
    }
}

std::string NetworkManager::readPackets(QTcpSocket* socket) {
    int bytes;
    std::string msg = "";
    if ((bytes = socket->bytesAvailable()) > 0) {
        int read = 0;
        char buffer [bytes + 1];
        memset(buffer, 0, sizeof (buffer));
        while (read < bytes) {
            read += socket->read(buffer + read, bytes - read);
        }
        msg = std::string(buffer);
        parsePackets(msg);
    }
    return msg;
}

void NetworkManager::client_parsePackets() {
    readPackets(tcpClient);
}

