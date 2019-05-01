#include "ChatWebSocketListener.h"

ChatWebSocketListener::ChatWebSocketListener(const Config *config, QObject *parent)
    : WebSocketListener(config, parent)
{
    registerSlot("login", &::ChatWebSocketListener::onMessageLogin);
    registerSlot("message", &::ChatWebSocketListener::onMessageMessage);
}

void ChatWebSocketListener::onDisconnect()
{
    auto *socket = qobject_cast<QWebSocket *>(sender());

    if (!isSocketAvailable(socket)) {
        return;
    }

    Client *client = WebSocketListener::clients[socket];
    const QString &name = client->getName();

    WebSocketListener::sendDataToAll(
        "message",
        {
            {"type", 2},
            {"name", name},
        });

    WebSocketListener::onDisconnect();
}

Client *ChatWebSocketListener::onConnect()
{
    Client *client = WebSocketListener::onConnect();

    for (const auto &registered: ChatWebSocketListener::registeredSlots) {
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

void ChatWebSocketListener::onMessageLogin(Client *client, const QJsonObject &data)
{
    QString name = data["name"].toString();

    auto isClientExists = std::find_if(
        WebSocketListener::clients.cbegin(),
        WebSocketListener::clients.cend(),
        [&name](Client *client) {
            return client->getName() == name;
        }
    ) != WebSocketListener::clients.cend();

    if (isClientExists) {
        client->sendData(
            "error",
            {
                {"id", 1},
                {"text", "Name is already exists"}
            });

        WebSocketListener::removeClient(client);
    } else {
        client->setName(name);
        client->setLogin(true);

        client->sendData(
            "login",
            {});

        WebSocketListener::sendDataToAll(
            "message",
            {
                {"type", 1},
                {"name", name},
            });
    }
}

void ChatWebSocketListener::onMessageMessage(Client *client, const QJsonObject &data)
{
    if (!client->isLogin()) {
        return;
    }

    QString message = data["text"].toString();

    WebSocketListener::sendDataToAll(
        "message",
        {
            {"type", 0},
            {"name", client->getName()},
            {"text", message},
        });
}
