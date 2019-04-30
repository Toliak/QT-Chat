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
    explicit Listener(const Config *config, QObject *parent = nullptr);

protected:
    virtual Client *onConnect();
    virtual void onMessage(const QString &message);
    virtual void onDisconnect();

protected:
    void sendDataToAll(const QString &type, const QJsonObject &data);

    void removeClient(Client *client)
    {
        Listener::clients.remove(client->getSocket());
        delete client;
    }

    bool isSocketAvailable(QWebSocket *socket)
    {
        return socket
            && Listener::clients.contains(socket)
            && Listener::clients[socket]->isLogin();
    }

    QHash<QWebSocket *, Client *> clients;      ///< Те, кому будет доставлено сообщение

private:
    const Config *config;
    QWebSocketServer *websocketServer = nullptr;

    void createWebSocketServer();
};



