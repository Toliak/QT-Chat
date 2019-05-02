#pragma once

#include <QString>
#include <QtWebSockets/QWebSocket>

#include "MessageHandler.h"

/**
 * @brief Basic client class
 */
struct Client: public QObject
{
Q_OBJECT

public:
    /**
     * @brief Basic constructor from WS connection
     * @param socket Connection
     */
    explicit Client(QWebSocket *socket, QObject *parent = nullptr);

    /**
     * @brief Sends data to client
     * @param type String type of sending data
     * @param data Sending data
     */
    void sendData(const QString &type, const QJsonObject &data);

    QWebSocket *getSocket() const
    {
        return Client::socket;
    }

    const QString &getName() const
    {
        return Client::name;
    }

    MessageHandler *getMessageHandler() const
    {
        return Client::messageHandler;
    }

    bool isLogin() const
    {
        return Client::loginStatus;
    }

    void setName(const QString &clientName)
    {
        Client::name = clientName;
    }

    void setLogin(bool login)
    {
        Client::loginStatus = login;
    }

    ~Client() override
    {
        delete Client::messageHandler;
    }

private:
    QWebSocket *socket;             ///< Connection
    QString name;                   ///< Client name
    MessageHandler *messageHandler; ///< Message transport

    bool loginStatus = false;       ///< Auth status
};