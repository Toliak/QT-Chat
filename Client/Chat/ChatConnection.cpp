#include "ChatConnection.h"

void ChatConnection::start(const QJsonObject &loginData)
{
    connect(this, &Connection::connected, [this, loginData]() {
        connect(this, &Connection::message, this, &ChatConnection::onMessage);

        Connection::send(
            QJsonDocument(
                QJsonObject{
                    {"type", "login"},
                    {"data", loginData},
                }
            ).toJson()
        );
    });

    connect(this, &Connection::disconnected, [this]() {
        if (ChatConnection::status == 0) {
            emit ChatConnection::fail("Disconnected");
        } else if (ChatConnection::status == 1) {
            emit ChatConnection::errorMessage({
                                                  {"text", "Disconnected"}
                                              });
        }
    });

    connect(this, &Connection::error, [this](const QString &text) {
        if (ChatConnection::status == 0) {
            emit ChatConnection::fail(text);
        } else if (ChatConnection::status == 1) {
            emit ChatConnection::errorMessage({
                                                  {"text", text}
                                              });
        }
    });

    Connection::start();
}

void ChatConnection::sendMessage(const QString &text)
{
    QJsonObject json{
        {"type", "message"},
        {"data", QJsonObject{
            {"text", text}
        }}
    };

    Connection::send(QJsonDocument(json).toJson());
}
void ChatConnection::onMessage(const QString &message)
{
    QJsonObject json = QJsonDocument::fromJson(message.toUtf8()).object();

    if (ChatConnection::status == 0) {
        if (json["type"].toString() == "login") {
            ChatConnection::status = 1;
            emit ChatConnection::login();
        } else if (json["type"].toString() == "error") {
            emit ChatConnection::fail(
                json["data"].toObject()["text"].toString()
            );
        }
    } else if (ChatConnection::status == 1) {
        if (json["type"].toString() == "message") {
            emit ChatConnection::chatMessage(json["data"].toObject());
        } else if (json["type"].toString() == "error") {
            emit ChatConnection::errorMessage(json["data"].toObject());
        }
    }
}
