#pragma once

#include <QSet>
#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

#include "Config.h"
#include "Client.h"

class Listener: public QObject
{
Q_OBJECT

public:
    explicit Listener(QObject *parent = nullptr);

protected slots:
    virtual Client *onConnect();
    virtual void onMessage(const QString &message);
    virtual void onDisconnect();

protected:
    void sendDataToAll(const QJsonObject &data, const QString &type);

    void removeClient(Client *client)
    {
        delete client;
        Listener::clients.remove(client->getSocket());
    }

//    void sendMessage(QWebSocket *socket, const QJsonObject &data, const QString &type = "data") const;

    bool isSocketAvailable(QWebSocket *socket)
    {
        return socket
            && Listener::clients.contains(socket)
            && Listener::clients[socket]->isLogin();
    }

    QHash<QWebSocket *, Client *> clients;      ///< Те, кому будет доставлено сообщение

private:
    QWebSocketServer *websocketServer = nullptr;

    void createWebSocketServer();
};



