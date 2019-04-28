#pragma once

#include <QObject>
#include <QString>
#include <QArgument>
#include <QJsonObject>
#include <QJsonDocument>

class Client;

class MessageHandler: public QObject
{
Q_OBJECT

public:
    explicit MessageHandler(Client *client, QObject *parent = nullptr);

    void giveMessage(const QString &message);

    Client *getClient() const
    {
        return MessageHandler::client;
    }

signals:
    void onMessage(const QString &type, const QJsonObject &json);

private:
    Client *client;
};