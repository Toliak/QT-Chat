#pragma once

#include <QObject>
#include <QJsonObject>

#include "Core/Connection.h"

class ChatConnection: public Connection
{
Q_OBJECT

public:
    /**
     * @brief Connection constructor
     * @param urlString Websocket server address
     * @param loginData Auth information
     * @param parent QT parent
     */
    explicit ChatConnection(const QString &urlString, QObject *parent = nullptr)
        : Connection(urlString, parent)
    {}

    /**
     * @brief Opens websocket connection
     * @param loginData Auto information
     */
    void start(const QJsonObject &loginData);

    void sendMessage(const QString &text);

signals:
    void chatMessage(const QJsonObject &data);
    void errorMessage(const QJsonObject &data);
    void login();
    void fail(const QString &reason);

private:
    /**
     * @brief Processes message
     * @param message Message
     */
    void onMessage(const QString &message);

    using Connection::start;    // Remove parent method

    quint8 status = 0;

};