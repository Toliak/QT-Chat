#pragma once

#include "../Core/Listener.h"
#include "../Core/MessageHandler.h"

class ChatListener: public Listener
{
public:
    explicit ChatListener(QObject *parent = nullptr)
        : Listener(parent)
    {
        registerSlot("login", &::ChatListener::onMessageLogin);
        registerSlot("message", &::ChatListener::onMessageMessage);
    }

protected:
    using RegisteredSlotType = void (ChatListener::*)(Client *client, const QJsonObject &);

    using Listener::onMessage;

    void onDisconnect() override
    {
        auto *socket = qobject_cast<QWebSocket *>(sender());

        if (!isSocketAvailable(socket)) {
            return;
        }

        Client *client = Listener::clients[socket];
        const QString& name = client->getName();

        Listener::sendDataToAll(
            {
                {"type", 2},
                {"name", name},
            },
            "message"
        );

        Listener::onDisconnect();
    }

    Client *onConnect() override
    {
        Client *client = Listener::onConnect();

        for (const auto &registered: ChatListener::registeredSlots) {
            QString expectedType = registered.first;
            RegisteredSlotType slot = registered.second;

            connect(
                client->getMessageHandler(),
                &MessageHandler::onMessage,
                [expectedType, slot, client, this](const QString &type, const QJsonObject &json) {
                    if (type == expectedType) {
                        (this->*slot)(client, json);
                    }
                }
            );
        }

        return client;
    }

private slots:
    void onMessageLogin(Client *client, const QJsonObject &data)
    {
        QString name = data["name"].toString();

        auto isClientExists = std::find_if(
            Listener::clients.cbegin(),
            Listener::clients.cend(),
            [&name](Client *client) {
                return client->getName() == name;
            }
        ) != Listener::clients.cend();

        if (isClientExists) {
            client->sendData(
                {
                    {"id", 1},
                    {"message", "Name is already exists"}
                },
                "error"
            );

            Listener::removeClient(client);
        } else {
            client->setName(name);
            client->setLogin(true);

            client->sendData(
                {},
                "login"
            );

            Listener::sendDataToAll(
                {
                    {"type", 1},
                    {"name", name},
                },
                "message"
            );
        }
    };

    void onMessageMessage(Client *client, const QJsonObject &data)
    {
        if (!client->isLogin()) {
            return;
        }

        QString message = data["message"].toString();

        Listener::sendDataToAll(
            {
                {"type", 0},
                {"name", client->getName()},
                {"text", message},
            },
            "message"
        );
    }

private:
    void registerSlot(const QString &type, RegisteredSlotType slot)
    {
        registeredSlots.push_back({type, slot});
    }

    QList<QPair<QString, RegisteredSlotType>> registeredSlots;
};