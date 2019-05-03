#include "Connection.h"

void Connection::start()
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

    connect(
        Connection::socket,
        QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
        [this]() {
            emit Connection::error(Connection::socket->errorString());
        }
    );

    Connection::socket->open(QUrl(urlString));
}
