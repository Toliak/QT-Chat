#pragma once

#include <QObject>
#include <QString>
#include <QArgument>
#include <QJsonObject>
#include <QJsonDocument>

class MessageHandler: public QObject
{
Q_OBJECT

public:
    explicit MessageHandler(QObject *parent = nullptr);

    void giveMessage(const QString &message);


signals:
    void onMessage(const QString &type, const QJsonObject &json);

private:
};