#pragma once

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

#include "Config.h"

class HttpListener: public QObject
{
Q_OBJECT
public:
    explicit HttpListener(Config *config, QObject *parent = nullptr);

protected:
    virtual QTcpSocket *onConnection();
    virtual QStringList onReady(QTcpSocket *socket);

private:
    Config *config;
    QTcpServer *tcpServer;

    void createTcpServer();
};