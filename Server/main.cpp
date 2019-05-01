#include <QCoreApplication>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <QResource>

#include "Core/Config.h"
#include "Chat/ChatWebSocketListener.h"
#include "Chat/StaticHttpListener.h"

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);

    auto *config = new Config();

    auto *listener = new ChatWebSocketListener(config);

    if (config->isHttp()) {
        auto *httpListener = new StaticHttpListener(new QFile(":/html/chat.html"), config);
    }

    return QCoreApplication::exec();
}