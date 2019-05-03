#pragma once

#include <QtWebSockets/QWebSocket>

/**
 * @brief Basic connection class
 */
class Connection: public QObject
{
Q_OBJECT

public:
    explicit Connection(QString urlString, QObject *parent = nullptr)
        : urlString(std::move(urlString)), QObject(parent), socket(new QWebSocket)
    { }

    virtual void start();

    void send(const QString &data)
    {
        Connection::socket->sendTextMessage(data);
    }

    ~Connection() override
    {
        delete Connection::socket;
    }

signals:
    void message(const QString &message);
    void connected();
    void disconnected();
    void error(const QString &error);

private:
    QString urlString;
    QWebSocket *socket;
};