#pragma once

#include "Core/WebSocketListener.h"
#include "Core/MessageHandler.h"

/**
 * @brief Chat websocket listener
 */
class ChatWebSocketListener: public WebSocketListener
{
Q_OBJECT

public:
    enum MessageType
    {
        MessageDefault = 0,
        MessageConnect,
        MessageDisconnect,
        MessageAvailable,
    };

    enum ErrorType
    {
        ErrorClientExists = 1,
        ErrorNameIncorrect,
        ErrorMessageIncorrect,
    };

    /**
     * @brief Default constructor
     * @param config Configuration object
     * @param parent QT parent
     */
    explicit ChatWebSocketListener(const Config *config, QObject *parent = nullptr);

    /**
     * @brief Checks char
     * @return true, if char is from [A-Za-z0-9_]
     */
    static bool checkNameChar(const QChar &c)
    {
        return (
            (c >= 'a' && c <= 'z')
                || (c >= 'A' && c <= 'Z')
                || (c >= L'а' && c <= L'я')
                || (c >= L'А' && c <= L'Я')
                || (c >= '0' && c <= '9')
                || (c == '_' || c == '-')
        );
    }

    /**
     * @brief Is name correct (size between 1 and 16, some char checks)
     * @param name
     * @return true, if name is correct
     */
    static bool checkName(const QString &name)
    {
        if (name.size() > 24) {
            return false;
        } else if (name.isEmpty()) {
            return false;
        } else if (std::count_if(name.cbegin(), name.cend(), ChatWebSocketListener::checkNameChar) != name.size()) {
            return false;
        }

        return true;
    }

    /**
     * @brief Checks message
     * @param message Message string
     * @return true, if message size is between 0 and 1024
     */
    static bool checkMessage(const QString &message)
    {
        if (message.size() > 1024) {
            return false;
        } else if (message.isEmpty()) {
            return false;
        }

        return true;
    }

    /**
     * @brief get logged in client names
     * @return List of names
     */
    QStringList getAvailableClientNames() const;

protected:
    using RegisteredSlotType = void (ChatWebSocketListener::*)(Client *client, const QJsonObject &);
    ///< Slot method signature

    using WebSocketListener::onMessage;
    void onDisconnect() override;
    Client *onConnect() override;

    void onMessageLogin(Client *client, const QJsonObject &data);
    void onMessageMessage(Client *client, const QJsonObject &data);

    void sendError(Client *client, qint64 errorId, const QString &errorText, bool disconnect = true);

private:
    /**
     * @brief Connects message with type to slot
     * @param type Message type
     * @param slot Slot
     */
    void registerSlot(const QString &type, RegisteredSlotType slot)
    {
        registeredSlots.push_back({type, slot});
    }

    QList<QPair<QString, RegisteredSlotType>> registeredSlots;
};