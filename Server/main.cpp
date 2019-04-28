#include <QCoreApplication>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

#include "Config.h"
#include "ChatListener.h"


int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);

    auto *config = Config::getConfig();

    auto *listener = new ChatListener();

    return QCoreApplication::exec();
}