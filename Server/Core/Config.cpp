#include "Config.h"

Config::Config()
{
    Config::load();
}

void Config::fromJson(const QJsonObject &json)
{
    Config::http = json["http"].toBool();
    Config::httpPort = json["httpPort"].toInt();
    Config::webSockerPort = json["webSockerPort"].toInt();
    Config::ip = json["ip"].toString();
}

QJsonObject Config::toJson() const
{
    QJsonObject json;
    json["httpPort"] = Config::httpPort;
    json["webSockerPort"] = Config::webSockerPort;
    json["ip"] = Config::ip;
    json["http"] = Config::http;

    return json;
}

void Config::load()
{
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open config file. Generating new...");
        Config::save();
        return;
    }

    QByteArray configBinary = loadFile.readAll();
    auto loadDocument = QJsonDocument::fromJson(configBinary);
    Config::fromJson(loadDocument.object());
}

void Config::save() const
{
    QFile saveFile(filename);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't save config.");
        return;
    }

    QJsonDocument saveDocument(Config::toJson());
    saveFile.write(saveDocument.toJson());
}


