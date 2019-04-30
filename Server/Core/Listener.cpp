#include "Listener.h"

Listener::Listener(const Config *config, QObject *parent)
    : config(config), QObject(parent)
{
    Listener::createWebSocketServer();
}

void Listener::createWebSocketServer()
{
    Listener::websocketServer = new QWebSocketServer("Chat", QWebSocketServer::NonSecureMode);

    if (Listener::websocketServer->listen(QHostAddress(Listener::config->getIp()), Listener::config->getPort())) {
        connect(Listener::websocketServer, &QWebSocketServer::newConnection, this, &Listener::onConnect);
        //TODO: should i connect close signal?
    } else {
        qFatal("Cannot start listener");
    }
}

Client *Listener::onConnect()
{
    QWebSocket *socket = Listener::websocketServer->nextPendingConnection();

    if (!socket) {
        return nullptr;
    }

    connect(socket, &QWebSocket::textMessageReceived, this, &Listener::onMessage);
    connect(socket, &QWebSocket::disconnected, this, &Listener::onDisconnect);

    qDebug("Connection");

    auto client = new Client(socket);
    Listener::clients.insert(socket, client);

    return client;
}

void Listener::onMessage(const QString &message)
{
    auto *socket = qobject_cast<QWebSocket *>(sender());

    if (!socket || !Listener::clients.contains(socket)) {
        return;
    }

    qDebug("Basic message from %s: %s",
           Listener::clients[socket]->getName().toStdString().c_str(),
           message.toStdString().c_str()
    );

    Listener::clients[socket]->getMessageHandler()->giveMessage(message);
}

void Listener::onDisconnect()
{
    auto *socket = qobject_cast<QWebSocket *>(sender());

    if (!isSocketAvailable(socket)) {
        return;
    }

    Client *client = Listener::clients[socket];
    QString name = client->getName();

    qDebug(
        "Disconnected %s",
        name.toStdString().c_str()
    );

    Listener::removeClient(client);
}

void Listener::sendDataToAll(const QString &type, const QJsonObject &data)
{
    for (Client *client : Listener::clients) {
        client->sendData(type, data);
    }
}
