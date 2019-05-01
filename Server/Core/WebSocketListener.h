#pragma once

#include <QSet>
#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

#include "Config.h"
#include "Client.h"

class WebSocketListener: public QObject
{
Q_OBJECT

public:
    explicit WebSocketListener(const Config *config, QObject *parent = nullptr);

protected:
    virtual Client *onConnect();
    virtual void onMessage(const QString &message);
    virtual void onDisconnect();

protected:
    void sendDataToAll(const QString &type, const QJsonObject &data);

    void removeClient(Client *client)
    {
        WebSocketListener::clients.remove(client->getSocket());
        delete client;
    }

    bool isSocketAvailable(QWebSocket *socket)
    {
        return socket
            && WebSocketListener::clients.contains(socket)
            && WebSocketListener::clients[socket]->isLogin();
    }

    QHash<QWebSocket *, Client *> clients;      ///< Те, кому будет доставлено сообщение

private:
    const Config *config;
    QWebSocketServer *websocketServer = nullptr;

    void createWebSocketServer();
};



