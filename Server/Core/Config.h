#pragma once

#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>

class Config
{
public:
    explicit Config();

    const QString &getFilename() const
    {
        return Config::filename;
    }

    quint16 getPort() const
    {
        return Config::port;
    }

    const QString &getIp() const
    {
        return Config::ip;
    }

    void fromJson(const QJsonObject &json);

    QJsonObject toJson() const;

private:
    const QString filename = "config.json";
    quint16 port = 8880;
    QString ip = "0.0.0.0";

    void load();
    void save() const;
};