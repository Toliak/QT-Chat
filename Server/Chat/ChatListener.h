#pragma once

#include "../Core/Listener.h"
#include "../Core/MessageHandler.h"

class ChatListener: public Listener
{
Q_OBJECT

public:
    explicit ChatListener(const Config *config, QObject *parent = nullptr);

protected:
    using RegisteredSlotType = void (ChatListener::*)(Client *client, const QJsonObject &);

    using Listener::onMessage;
    void onDisconnect() override;
    Client *onConnect() override;

private:
    void onMessageLogin(Client *client, const QJsonObject &data);;
    void onMessageMessage(Client *client, const QJsonObject &data);

private:
    void registerSlot(const QString &type, RegisteredSlotType slot)
    {
        registeredSlots.push_back({type, slot});
    }

    QList<QPair<QString, RegisteredSlotType>> registeredSlots;
};