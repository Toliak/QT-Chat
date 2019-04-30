#include <utility>

#pragma once

#include <QString>
#include <QtWebSockets/QWebSocket>

class Connection: public QObject
{
Q_OBJECT

public:
    explicit Connection(QString urlString, QObject *parent = nullptr)
        : urlString(std::move(urlString)), QObject(parent), socket(new QWebSocket)
    {
    }

    void start()
    {
        connect(Connection::socket, &QWebSocket::connected, [this]() {
            connect(Connection::socket, &QWebSocket::textMessageReceived, [this](const QString &message) {
                emit Connection::message(message);
            });

            emit Connection::connected();
        });
        connect(Connection::socket, &QWebSocket::disconnected, [this]() {
            emit Connection::disconnected();
        });
        Connection::socket->open(QUrl(urlString));
    }

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

private:
    QString urlString;
    QWebSocket *socket;


};