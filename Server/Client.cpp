#include "Client.h"

Client::Client(QWebSocket *socket)
    : socket(socket), messageHandler(new MessageHandler(this))
{}

void Client::sendData(const QJsonObject &data, const QString &type)
{
    QJsonObject json;
    json["type"] = type;
    json["data"] = data;

    Client::socket->sendTextMessage(QJsonDocument(json).toJson());
}
