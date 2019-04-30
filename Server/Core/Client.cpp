#include "Client.h"

Client::Client(QWebSocket *socket)
    : socket(socket), messageHandler(new MessageHandler)
{}

void Client::sendData(const QString &type, const QJsonObject &data)
{
    QJsonObject json;
    json["type"] = type;
    json["data"] = data;

    Client::socket->sendTextMessage(QJsonDocument(json).toJson());
}
