#include "MessageHandler.h"

MessageHandler::MessageHandler(QObject *parent)
    : QObject(parent)
{}

void MessageHandler::giveMessage(const QString &message)
{
    QJsonObject json = QJsonDocument::fromJson(message.toUtf8()).object();

    if (!(json.contains("type") && json.contains("data"))) {
        return;
    }

    emit onMessage(json["type"].toString(), json["data"].toObject());
}
