#include "HttpListener.h"

HttpListener::HttpListener(Config *config, QObject *parent)
    : QObject(parent),
      tcpServer(new QTcpServer),
      config(config)
{
    HttpListener::createTcpServer();
}

void HttpListener::createTcpServer()
{
    QHostAddress address(HttpListener::config->getIp());

    if (HttpListener::tcpServer->listen(address, HttpListener::config->getHttpPort())) {
        connect(
            HttpListener::tcpServer,
            &QTcpServer::newConnection,
            this,
            &HttpListener::onConnection
        );
    }
}

QTcpSocket *HttpListener::onConnection()
{
    QTcpSocket *socket = HttpListener::tcpServer->nextPendingConnection();
    if (!socket) {
        return nullptr;
    }

    qDebug("Http connected!");

    connect(
        socket,
        &QAbstractSocket::readyRead,
        [this, socket]() {
            this->onReady(socket);
        }
    );

    return socket;
}

QStringList HttpListener::onReady(QTcpSocket *socket)
{
    QString data = socket->readAll();
    QStringList lines = data.split("\r\n");

    return lines;
}
