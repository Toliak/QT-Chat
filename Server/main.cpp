#include <QCoreApplication>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <QResource>

#include "Core/Config.h"
#include "Chat/ChatWebSocketListener.h"

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);

    auto *config = new Config("server.json");

    auto *listener = new ChatWebSocketListener(config);
    listener->start();

    return QCoreApplication::exec();
}