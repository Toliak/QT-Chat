#include "Listener.h"

Listener::Listener(QObject *parent)
    : QObject(parent)
{
    Listener::createWebSocketServer();
}

void Listener::createWebSocketServer()
{
    auto *config = Config::getConfig();

    Listener::websocketServer = new QWebSocketServer("Chat", QWebSocketServer::NonSecureMode);

    if (Listener::websocketServer->listen(QHostAddress(config->getIp()), config->getPort())) {
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
    clients.insert(socket, client);

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

void Listener::sendDataToAll(const QJsonObject &data, const QString &type)
{
    for (Client *client : clients) {
        client->sendData(data, type);
    }
}

/*
void Listener::sendMessage(QWebSocket *socket, const QJsonObject &data, const QString &type) const
{
    QJsonObject json;
    json["type"] = type;
    json["data"] = data;

    if (socket) {
        socket->sendTextMessage(QJsonDocument(json).toJson());
    } else {
        for (auto it = Listener::clients.begin(); it != Listener::clients.end(); it++) {
            it.key()->sendTextMessage(QJsonDocument(json).toJson());
        }
    }
}


void Listener::executeMessage(QWebSocket *socket, const QJsonObject &json)
{
    if (!json.contains("type")) {
        return;
    }

    if (json["type"].toString() == "loginStatus") {
        if (Listener::clients[socket]->isLogin()) {
            return;
        }
        Listener::clients[socket]->setLogin(true);

        QString name = json["name"].toString();
        Listener::clients[socket]->setName(name);
        Listener::sendMessage(nullptr, name, "connect");

        qDebug("Login %s", name.toStdString().c_str());
    } else if (json["type"].toString() == "message") {
        QString message = json["message"].toString();
        sendMessage(nullptr, message);

        qDebug(
            "Message from %s: %s",
            Listener::clients[socket]->getName().toStdString().c_str(),
            message.toStdString().c_str()
        );
    }
}
*/
