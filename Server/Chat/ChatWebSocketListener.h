#pragma once

#include "Core/WebSocketListener.h"
#include "Core/MessageHandler.h"

class ChatWebSocketListener: public WebSocketListener
{
Q_OBJECT

public:
    explicit ChatWebSocketListener(const Config *config, QObject *parent = nullptr);

protected:
    using RegisteredSlotType = void (ChatWebSocketListener::*)(Client *client, const QJsonObject &);

    using WebSocketListener::onMessage;
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