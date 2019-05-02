#pragma once

#include <QObject>
#include <QString>
#include <QArgument>
#include <QJsonObject>
#include <QJsonDocument>

/**
 * @brief Get and prepare message. Can be connected
 */
class MessageHandler: public QObject
{
Q_OBJECT

public:
    /**
     * @brief Default constructor
     * @param parent QT parent
     */
    explicit MessageHandler(QObject *parent = nullptr);

    /**
     * @brief Send message from handler
     * @param message String message
     */
    void giveMessage(const QString &message);

signals:
    /**
     * @brief Triggers on message got
     * @param type Type of message
     * @param json Message data
     */
    void onMessage(const QString &type, const QJsonObject &json);

private:

};