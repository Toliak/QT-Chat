#include "WebSocketListener.h"

WebSocketListener::WebSocketListener(const Config *config, QObject *parent)
    : config(config),
      websocketServer(new QWebSocketServer("Chat", QWebSocketServer::NonSecureMode)),
      QObject(parent)
{
    WebSocketListener::createWebSocketServer();
}

void WebSocketListener::createWebSocketServer()
{
    QHostAddress address(WebSocketListener::config->getIp());
    if (WebSocketListener::websocketServer->listen(address, WebSocketListener::config->getWebSocketPort())) {
        connect(
            WebSocketListener::websocketServer,
            &QWebSocketServer::newConnection,
            this,
            &WebSocketListener::onConnect
        );
        //TODO: should i connect close signal?
    } else {
        qFatal("Cannot start listener");
    }
}

Client *WebSocketListener::onConnect()
{
    QWebSocket *socket = WebSocketListener::websocketServer->nextPendingConnection();

    if (!socket) {
        return nullptr;
    }

    connect(socket, &QWebSocket::textMessageReceived, this, &WebSocketListener::onMessage);
    connect(socket, &QWebSocket::disconnected, this, &WebSocketListener::onDisconnect);

    qDebug("Connection");

    auto client = new Client(socket);
    WebSocketListener::clients.insert(socket, client);

    return client;
}

void WebSocketListener::onMessage(const QString &message)
{
    auto *socket = qobject_cast<QWebSocket *>(sender());

    if (!socket || !WebSocketListener::clients.contains(socket)) {
        return;
    }

    qDebug("Basic message from %s: %s",
           WebSocketListener::clients[socket]->getName().toStdString().c_str(),
           message.toStdString().c_str()
    );

    WebSocketListener::clients[socket]->getMessageHandler()->giveMessage(message);
}

void WebSocketListener::onDisconnect()
{
    auto *socket = qobject_cast<QWebSocket *>(sender());

    if (!isSocketAvailable(socket)) {
        return;
    }

    Client *client = WebSocketListener::clients[socket];
    QString name = client->getName();

    qDebug(
        "Disconnected %s",
        name.toStdString().c_str()
    );

    WebSocketListener::removeClient(client);
}

void WebSocketListener::sendDataToAll(const QString &type, const QJsonObject &data)
{
    for (Client *client : WebSocketListener::clients) {
        client->sendData(type, data);
    }
}
