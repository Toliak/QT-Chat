#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include "Core/Connection.h"

class ChatConnection: public Connection
{
Q_OBJECT

public:
    ChatConnection(const QString &urlString, const QJsonObject &loginData, QObject *parent = nullptr)
        : Connection(urlString, parent)
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
            emit ChatConnection::fail();
        });
    }

    void sendMessage(const QString &text)
    {
        QJsonObject json{
            {"type", "message"},
            {"data", QJsonObject{
                {"text", text}
            }}
        };

        Connection::send(QJsonDocument(json).toJson());
    }

signals:
    void chatMessage(const QJsonObject &data);
    void login();
    void fail();

private:
    void onMessage(const QString &message)
    {
        QJsonObject json = QJsonDocument::fromJson(message.toUtf8()).object();

        if (ChatConnection::status == 0) {
            if (json["type"].toString() == "login") {
                ChatConnection::status = 1;
                emit ChatConnection::login();
            } else {
                ChatConnection::status = -1;
                emit ChatConnection::fail();
            }
        } else if (ChatConnection::status == 1) {
            if (json["type"].toString() == "message") {
                emit ChatConnection::chatMessage(json["data"].toObject());
            }
        }
    };

    quint8 status = 0;

};