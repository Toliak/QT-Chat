#include "Client.h"

Client::Client(QWebSocket *socket, QObject *parent)
    : socket(socket), messageHandler(new MessageHandler), QObject(parent)
{}

void Client::sendData(const QString &type, const QJsonObject &data)
{
    QJsonObject json;
    json["type"] = type;
    json["data"] = data;

    Client::socket->sendTextMessage(QJsonDocument(json).toJson());
}
