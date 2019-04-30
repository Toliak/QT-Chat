#include "ChatListener.h"

ChatListener::ChatListener(const Config *config, QObject *parent)
    : Listener(config, parent)
{
    registerSlot("login", &::ChatListener::onMessageLogin);
    registerSlot("message", &::ChatListener::onMessageMessage);
}

void ChatListener::onDisconnect()
{
    auto *socket = qobject_cast<QWebSocket *>(sender());

    if (!isSocketAvailable(socket)) {
        return;
    }

    Client *client = Listener::clients[socket];
    const QString &name = client->getName();

    Listener::sendDataToAll(
        "message",
        {
            {"type", 2},
            {"name", name},
        });

    Listener::onDisconnect();
}

Client *ChatListener::onConnect()
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

void ChatListener::onMessageLogin(Client *client, const QJsonObject &data)
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
            "error",
            {
                {"id", 1},
                {"text", "Name is already exists"}
            });

        Listener::removeClient(client);
    } else {
        client->setName(name);
        client->setLogin(true);

        client->sendData(
            "login",
            {});

        Listener::sendDataToAll(
            "message",
            {
                {"type", 1},
                {"name", name},
            });
    }
}

void ChatListener::onMessageMessage(Client *client, const QJsonObject &data)
{
    if (!client->isLogin()) {
        return;
    }

    QString message = data["text"].toString();

    Listener::sendDataToAll(
        "message",
        {
            {"type", 0},
            {"name", client->getName()},
            {"text", message},
        });
}
