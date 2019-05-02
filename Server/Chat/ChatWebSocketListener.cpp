#include <QJsonArray>

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
            {"type", ChatWebSocketListener::MessageDisconnect},
            {"name", name},
        }
    );

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

QStringList ChatWebSocketListener::getAvailableClientNames() const
{
    QStringList names;
    for (Client *client : WebSocketListener::clients) {
        if (client->isLogin()) {
            names.push_back(client->getName());
        }
    }
    return names;
}

void ChatWebSocketListener::sendError(Client *client, qint64 errorId, const QString &errorText, bool disconnect)
{
    client->sendData(
        "error",
        {
            {"id", errorId},
            {"text", "errorText"},
        }
    );

    if (disconnect) {
        WebSocketListener::removeClient(client);
    }
}

void ChatWebSocketListener::onMessageLogin(Client *client, const QJsonObject &data)
{
    QString name = data["name"].toString();     ///< Client name
    if (!ChatWebSocketListener::checkName(name)) {                     // Name is incorrect
        return ChatWebSocketListener::sendError(
            client,
            ChatWebSocketListener::ErrorNameIncorrect,
            "Name size must be between 1 and 16. Name can contain only A-Z, a-z, 0-9 and _"
        );
    }

    auto isClientExists = std::find_if(
        WebSocketListener::clients.cbegin(),
        WebSocketListener::clients.cend(),
        [&name](Client *client) {
            return client->getName() == name;
        }
    ) != WebSocketListener::clients.cend();

    if (isClientExists) {
        return ChatWebSocketListener::sendError(
            client,
            ChatWebSocketListener::ErrorClientExists,
            "Client with this name is already exists"
        );
    } else {
        // Setup client
        client->setName(name);
        client->setLogin(true);

        // Successful login
        client->sendData(
            "login",
            {}
        );

        QJsonArray availableNames;
        QStringList availableNamesList = ChatWebSocketListener::getAvailableClientNames();
        std::transform(
            availableNamesList.cbegin(),
            availableNamesList.cend(),
            std::back_inserter(availableNames),
            [](auto input) {
                return input;
            }
        );

        client->sendData(
            "message",
            {
                {"type", ChatWebSocketListener::MessageAvailable},
                {"data", availableNames}
            }
        );

        WebSocketListener::sendDataToAll(
            "message",
            {
                {"type", ChatWebSocketListener::MessageConnect},
                {"name", name},
            }
        );
    }
}

void ChatWebSocketListener::onMessageMessage(Client *client, const QJsonObject &data)
{
    if (!client->isLogin()) {
        return;
    }

    QString message = data["text"].toString();
    if (!ChatWebSocketListener::checkMessage(message)) {
        return ChatWebSocketListener::sendError(
            client,
            ChatWebSocketListener::ErrorMessageIncorrect,
            "Message size must be between 1 and 64",
            false
        );
    }

    WebSocketListener::sendDataToAll(
        "message",
        {
            {"type", ChatWebSocketListener::MessageDefault},
            {"name", client->getName()},
            {"text", message},
        });
}
