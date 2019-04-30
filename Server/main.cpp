#include <QCoreApplication>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

#include "Core/Config.h"
#include "Chat/ChatListener.h"

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);

    auto *config = new Config();

    auto *listener = new ChatListener(nullptr, nullptr);

    return QCoreApplication::exec();
}