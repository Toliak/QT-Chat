#pragma once

#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>

class Config
{
public:
    Config(const Config &) = delete;
    Config(Config &&) = delete;

    Config &operator=(const Config &) = delete;
    Config &operator=(Config &&) = delete;

    static Config * getConfig()
    {
        if (!Config::instance) {
            Config::instance = new Config;
        }
        return Config::instance;
    }

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

    explicit Config();

    void load();
    void save() const;

    static Config *instance;
};