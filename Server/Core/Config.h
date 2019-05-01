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

    quint16 getWebSocketPort() const
    {
        return Config::webSockerPort;
    }

    bool isHttp() const
    {
        return http;
    }

    quint16 getHttpPort() const
    {
        return Config::httpPort;
    }

    const QString &getIp() const
    {
        return Config::ip;
    }

    void fromJson(const QJsonObject &json);

    QJsonObject toJson() const;

private:
    const QString filename = "config.json";
    bool http = false;
    quint16 httpPort = 8888;
    quint16 webSockerPort = 8880;
    QString ip = "0.0.0.0";

    void load();
    void save() const;
};