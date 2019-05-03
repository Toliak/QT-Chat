#pragma once

#include <QSet>
#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

#include "Config.h"
#include "Client.h"

/**
 * @brief Basic websocket listener
 */
class WebSocketListener: public QObject
{
Q_OBJECT

public:
    /**
     * @brief Default constructor
     * @param config Configuration object
     * @param parent QT parent
     */
    explicit WebSocketListener(const Config *config, QObject *parent = nullptr)
        : config(config),
          websocketServer(new QWebSocketServer("Chat", QWebSocketServer::NonSecureMode)),
          QObject(parent)
    {}

    /**
     * @brief Start the websocker server
     */
    virtual void start();

    /**
     * @brief Checks if socket exists, socket registered and is login
     * @param socket Connection
     * @return true, if checks have been passed
     */
    bool isSocketAvailable(QWebSocket *socket) const
    {
        return socket
            && WebSocketListener::clients.contains(socket)
            && WebSocketListener::clients[socket]->isLogin();
    }

    ~WebSocketListener() override
    {
        delete WebSocketListener::websocketServer;
    }

protected:
    virtual Client *onConnect();
    virtual void onMessage(const QString &message);
    virtual void onDisconnect();

protected:
    void sendDataToAll(const QString &type, const QJsonObject &data);

    /**
     * @brief Removes client from registered
     * @param client Client object
     */
    void removeClient(Client *client)
    {
        client->getSocket()->close();
        WebSocketListener::clients.remove(client->getSocket());
        delete client;
    }

    QHash<QWebSocket *, Client *> clients;

private:
    const Config *config;
    QWebSocketServer *websocketServer = nullptr;
};



