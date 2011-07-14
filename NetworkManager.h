/* 
 * File:   NetworkManager.h
 * Author: brandon
 *
 * Created on July 3, 2011, 5:26 PM
 */

#ifndef NETWORKMANAGER_H
#define	NETWORKMANAGER_H
#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

class NetworkManager : public QObject {
    Q_OBJECT
public:
    NetworkManager(const bool server, QObject* parent = 0);
    virtual ~NetworkManager();

    bool isServer();
    bool isConnected();

    void parsePackets(std::string);
    std::string readPackets(QTcpSocket* socket);
    void sendPackets(std::string code, std::string msg);

protected slots:
    void server_newConnect();
    void server_parsePackets();

    void client_parsePackets();
    void client_getConnect();
private:
    void sendPackets(std::string concat, QTcpSocket* client);
    //SERVER
    bool server;
    int port;
    std::string message;
    QTcpServer* tcpServer;
    QList<QTcpSocket*>* server_clients;
    //CLIENT
    bool connected;
    QTcpSocket* tcpClient;
};

#endif	/* NETWORKMANAGER_H */

