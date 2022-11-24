#include "c_parser.h"

c_parser::c_parser(QObject *parent)
    : QObject{parent}
{

}

QByteArray c_parser::prepareSetPlayerNamePacket(const QString &playerName)
{
    QList<QMap<QString, QVariant>> list;
    QMap<QString, QVariant> map;
    map["player_name"] = playerName;
    list.append(map);

    return prepareJSON(parser::SET_PLAYER_NAME, list).toJson(QJsonDocument::Compact);
}

QByteArray c_parser::prepareSetPlayerNameAnswer(const QString &playerName)
{
    QList<QMap<QString, QVariant>> list;
    QMap<QString, QVariant> map;
    map["player_name"] = playerName;
    list.append(map);

    return prepareJSON(parser::SET_PLAYER_NAME_ANSWER, list).toJson(QJsonDocument::Compact);
}

QByteArray c_parser::prepareGamesListRequest()
{
    return prepareJSON(parser::GET_GAMES_LIST, QList<QMap<QString, QVariant>>()).toJson(QJsonDocument::Compact);
}

QByteArray c_parser::prepareGamesListPacket(const QList<game::gameInformations> &gamesData)
{
    QList<QMap<QString, QVariant>> list;
    foreach (game::gameInformations game, gamesData) {
        list.append(game.toMap());
    }
    return prepareJSON(parser::GAMES_LIST, list).toJson(QJsonDocument::Compact);
}

QByteArray c_parser::prepareNewGameRequest()
{
    return prepareJSON(parser::CREATE_NEW_GAME, QList<QMap<QString, QVariant>>()).toJson(QJsonDocument::Compact);
}

QByteArray c_parser::prepareNewGameRequestAnswer(const game::gameInformations &game)
{
    QList<QMap<QString, QVariant>> list;
    list.append( const_cast<game::gameInformations &>(game).toMap() );

    return prepareJSON(parser::GAME_CREATED, list).toJson(QJsonDocument::Compact);
}

QByteArray c_parser::prepareModifyGameRequestAnswer(const game::gameInformations &game)
{
    QList<QMap<QString, QVariant>> list;
    list.append( const_cast<game::gameInformations &>(game).toMap() );

    return prepareJSON(parser::GAME_INFOS_CHANGED, list).toJson(QJsonDocument::Compact);
}

QByteArray c_parser::prepareRemoveGameRequestAnswer(const QString &gameName)
{
    QList<QMap<QString, QVariant>> list;
    QMap<QString, QVariant> map;
    map["game_name"] = gameName;
    list.append(map);

    return prepareJSON(parser::GAME_REMOVED, list).toJson(QJsonDocument::Compact);
}

QJsonDocument c_parser::prepareJSON(parser::PacketContent content, const QList<QMap<QString, QVariant>> &packet_data)
{
    QJsonDocument jsonDoc;
    QJsonObject mainobject;

    QJsonArray packetDataArray;

    mainobject["content"] = content;

    for(int i = 0; i < packet_data.size(); i++) {
        QJsonObject data_obj;

        foreach(QString key, packet_data[i].keys()) {
            data_obj[key] = QJsonValue::fromVariant(packet_data[i][key]);
        }

        packetDataArray.append(data_obj);
    }

    mainobject["data"] = packetDataArray;
    jsonDoc.setObject(mainobject);
    return jsonDoc;
}

parser::Packet c_parser::ParseReceivedPacket(QByteArray data, qintptr socketDescriptor)
{
    parser::Packet packetData;

    packetData.socketDescriptor = socketDescriptor;

    if (data.isEmpty()) {
        packetData.content = parser::EMPTY;
        return packetData;
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError) {
        packetData.content = parser::ERROR_READING_JSON;
        return packetData;
    }

    QJsonObject mainObject = jsonDoc.object();

    packetData.content = static_cast<parser::PacketContent>( mainObject["content"].toInteger() );


    QJsonArray dataArray = mainObject["data"].toArray();

    for(int i = 0; i < dataArray.size(); i++)
    {
        QJsonObject object = dataArray[i].toObject();

        QMap<QString, QVariant> map;
        foreach(QString key, object.keys()) {
            map[key] = object[key].toVariant();
        }
        packetData.data.append(map);
    }

    return packetData;
}
