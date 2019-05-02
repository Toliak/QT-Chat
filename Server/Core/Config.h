#pragma once

#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>

/**
 * @brief Config class
 */
class Config: public QObject
{
Q_OBJECT

public:
    /**
     * @brief Default constructor
     * @param name JSON config filename
     */
    explicit Config(QString name, QObject *parent = nullptr);

    const QString &getFilename() const
    {
        return Config::filename;
    }

    quint16 getWebSocketPort() const
    {
        return Config::webSocketPort;
    }

    const QString &getIp() const
    {
        return Config::ip;
    }

private:
    QString filename;
    quint16 webSocketPort = 8880;
    QString ip = "0.0.0.0";

    /**
     * @brief Reader
     */
    void load();

    /**
     * @brief Writer
     */
    void save() const;

    /**
     * @brief Exporter to JSON
     * @return JSON config data
     */
    QJsonObject toJson() const;

    /**
     * @brief Imported from JSON
     * @param json JSON object with config data
     */
    void fromJson(const QJsonObject &json);
};