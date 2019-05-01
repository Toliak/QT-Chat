#pragma once

#include <QIODevice>
#include <iostream>

#include "Core/HttpListener.h"

class StaticHttpListener: public HttpListener
{
Q_OBJECT
public:
    explicit StaticHttpListener(QFile *file, Config *config, QObject *parent = nullptr)
        : HttpListener(config, parent), file(file)
    {}

    ~StaticHttpListener() override
    {
        file->deleteLater();
    }

protected:
    using HttpListener::onConnection;

    QStringList onReady(QTcpSocket *socket) override
    {
        QStringList lines = HttpListener::onReady(socket);

        if (!file->open(QIODevice::ReadOnly)) {
            return lines;
        }

        socket->write(
            "HTTP/1.0 200 Ok\n"
            "Content-Type: text/html; charset=\"utf-8\"\n"
            "\n"
                + file->readAll()
        );
        file->close();

        socket->disconnectFromHost();

        return lines;
    }

/*
    QTcpSocket *onConnection() override
    {
        QTcpSocket *socket = HttpListener::onConnection();
        if (!socket) {
            return nullptr;
        }

        connect(
            socket,
            &QIODevice::readyRead,
            [socket]() {
                char webBrowerRXData[1000];
                int sv=socket->read(webBrowerRXData,1000);
                std::cout<<"\nreading web browser data\n";
                for(int i=0;i<sv;i++)std::cout<<webBrowerRXData[i];
                std::cout<<"\n";
            }
        );

        if (!file->open(QIODevice::ReadOnly)) {
            return socket;
        }

        socket->write(
            "HTTP/1.0 200 Ok\n"
            "Content-Type: text/html; charset=\"utf-8\"\n"
            "\n"
                + file->readAll()
        );
        file->close();

        socket->disconnectFromHost();

        return socket;
    }
*/

private:
    QFile *file;

};