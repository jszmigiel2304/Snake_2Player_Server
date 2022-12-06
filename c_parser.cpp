#include "c_parser.h"

c_parser::c_parser(QObject *parent)
    : QObject{parent}
{

}

const QByteArray &c_parser::prepareSetPlayerNamePacket(const QString &playerName)
{
    QList<QMap<QString, QVariant>> list;
    QMap<QString, QVariant> map;
    map["player_name"] = playerName;
    list.append(map);    

    prepareJSON(parser::SET_PLAYER_NAME, list);

    return answerPacket;
}

const QByteArray &c_parser::prepareSetPlayerNameAnswer(const QString &playerName)
{
    QList<QMap<QString, QVariant>> list;
    QMap<QString, QVariant> map;
    map["player_name"] = playerName;
    list.append(map);

    prepareJSON(parser::SET_PLAYER_NAME_ANSWER, list);

    return answerPacket;
}

const QByteArray &c_parser::prepareGamesListRequest()
{
    prepareJSON(parser::GET_GAMES_LIST, QList<QMap<QString, QVariant>>());

    return answerPacket;
}

const QByteArray &c_parser::prepareGamesListPacket(const QList<game::gameInformations> &gamesData)
{
    QList<QMap<QString, QVariant>> list;
    foreach (game::gameInformations game, gamesData) {
        list.append(game.toMap());
    }
    prepareJSON(parser::GAMES_LIST, list);

    return answerPacket;
}

const QByteArray &c_parser::prepareLobbiesListRequest()
{
    prepareJSON(parser::GET_LOBBIES_LIST, QList<QMap<QString, QVariant>>());

    return answerPacket;
}

const QByteArray &c_parser::prepareLobbiesListPacket(const QList<lobby::lobbyInformations> &lobbiesData)
{
    QList<QMap<QString, QVariant>> list;
    foreach (lobby::lobbyInformations lobby, lobbiesData) {
        list.append(lobby.toMap());
    }

    prepareJSON(parser::LOBBIES_LIST, list);

    return answerPacket;
}

const QByteArray &c_parser::prepareNewGameRequest(const QString &playerName)
{
    QList<QMap<QString, QVariant>> list;
    QMap<QString, QVariant> map;
    map["player_name"] = playerName;
    list.append(map);

    prepareJSON(parser::CREATE_NEW_GAME, list);

    return answerPacket;
}

const QByteArray &c_parser::prepareNewGameRequestAnswer(const game::gameInformations &game)
{
    QList<QMap<QString, QVariant>> list;
    list.append( const_cast<game::gameInformations &>(game).toMap() );

    prepareJSON(parser::GAME_CREATED, list);

    return answerPacket;
}

const QByteArray &c_parser::prepareNewLobbyRequest(const QString &playerName)
{
    QList<QMap<QString, QVariant>> list;
    QMap<QString, QVariant> map;
    map["player_name"] = playerName;
    list.append(map);

    prepareJSON(parser::CREATE_NEW_LOBBY, list);

    return answerPacket;
}

const QByteArray &c_parser::prepareNewLobbyRequestAnswer(const lobby::lobbyInformations &lobby)
{
    QList<QMap<QString, QVariant>> list;
    list.append( const_cast<lobby::lobbyInformations &>(lobby).toMap() );

    prepareJSON(parser::LOBBY_CREATED, list);

    return answerPacket;
}

const QByteArray &c_parser::prepareModifyGameRequestAnswer(const game::gameInformations &game)
{
    QList<QMap<QString, QVariant>> list;
    list.append( const_cast<game::gameInformations &>(game).toMap() );

    prepareJSON(parser::GAME_INFOS_CHANGED, list);

    return answerPacket;
}

const QByteArray &c_parser::prepareModifyLobbyRequestAnswer(const lobby::lobbyInformations &lobby)
{
    QList<QMap<QString, QVariant>> list;
    list.append( const_cast<lobby::lobbyInformations &>(lobby).toMap() );

    prepareJSON(parser::LOBBY_INFOS_CHANGED, list);

    return answerPacket;
}

const QByteArray &c_parser::prepareRemoveGameRequestAnswer(const QString &gameName)
{
    QList<QMap<QString, QVariant>> list;
    QMap<QString, QVariant> map;
    map["game_name"] = gameName;
    list.append(map);

    prepareJSON(parser::GAME_REMOVED, list);

    return answerPacket;
}

const QByteArray &c_parser::prepareRemoveLobbyRequestAnswer(const QString &lobbyName)
{
    QList<QMap<QString, QVariant>> list;
    QMap<QString, QVariant> map;
    map["lobby_name"] = lobbyName;
    list.append(map);

    prepareJSON(parser::LOBBY_REMOVED, list);

    return answerPacket;
}

void c_parser::prepareJSON(parser::PacketContent content, const QList<QMap<QString, QVariant>> &packet_data)
{
    QJsonDocument workingDocument;
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
    workingDocument.setObject(mainobject);

    saveDocumentToByteArray(workingDocument);
}

void c_parser::ParseReceivedPacket(const QByteArray &data, parser::Packet &outerPacket,  qintptr socketDescriptor)
{
    //parser::Packet packetData;

    outerPacket.socketDescriptor = socketDescriptor;

    if (data.isEmpty()) {
        outerPacket.content = parser::EMPTY;
        return;
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError) {
        outerPacket.content = parser::ERROR_READING_JSON;
        return;
    }

    QJsonObject mainObject = jsonDoc.object();

    outerPacket.content = static_cast<parser::PacketContent>( mainObject["content"].toInteger() );


    QJsonArray dataArray = mainObject["data"].toArray();

    for(int i = 0; i < dataArray.size(); i++)
    {
        QJsonObject object = dataArray[i].toObject();

        QMap<QString, QVariant> map;
        foreach(QString key, object.keys()) {
            map[key] = object[key].toVariant();
        }
        outerPacket.data.append(map);
    }
}

void c_parser::saveDocumentToByteArray(const QJsonDocument &doc, QByteArray *outerByteArray)
{
    if(outerByteArray == nullptr) {
        answerPacket = doc.toJson(QJsonDocument::Compact);
    } else {
        *(outerByteArray) = doc.toJson(QJsonDocument::Compact);
    }
}

//template<class T>
//QByteArray c_parser::getPacket(quint8 jsonContent, T data)
//{
//    switch(jsonContent) {
//    case parser::EMPTY: {return answerPacket;}
//    case parser::ERROR_READING_JSON: {return answerPacket;}
//    case parser::SET_PLAYER_NAME: {return prepareSetPlayerNamePacket(data);}
//    case parser::SET_PLAYER_NAME_ANSWER: {return prepareSetPlayerNameAnswer(data);}
//    case parser::CREATE_NEW_GAME: {return prepareNewGameRequest();}
//    case parser::GAME_CREATED: {return prepareNewGameRequestAnswer(data);}
//    case parser::REMOVE_GAME: {return answerPacket;}
//    case parser::GAME_INFOS_CHANGED: {return prepareModifyGameRequestAnswer(data);}
//    case parser::GET_GAMES_LIST: {return prepareGamesListRequest();}
//    case parser::GAMES_LIST: {return prepareGamesListPacket(data);}
//    case parser::GAME_REMOVED: {return prepareRemoveGameRequestAnswer(data);}
//    case parser::GAME_CREATED_STARTING_BOARD: {return answerPacket;}
//    case parser::GAME_START: {return answerPacket;}
//    case parser::GAME_STARTED: {return answerPacket;}
//    case parser::GAME_STATE_CHANGED: {return answerPacket;}
//    case parser::GAME_BOARD_STATE_CHANGED: {return answerPacket;}
//    case parser::GAME_SNAKE_MOVE_DIRECTION_CHANGED: {return answerPacket;}
//    case parser::GAME_PLAYER_COINS_NUMBER_CHANGED: {return answerPacket;}
//    case parser::GAME_PLAYER_SHOP_CHANGED: {return answerPacket;}
//    case parser::CREATE_NEW_LOBBY: {return prepareNewLobbyRequest();}
//    case parser::LOBBY_CREATED: {return prepareNewLobbyRequestAnswer(data);}
//    case parser::REMOVE_LOBBY: {return answerPacket;}
//    case parser::LOBBY_REMOVED: {return prepareRemoveLobbyRequestAnswer(data);}
//    case parser::LOBBY_INFOS_CHANGED: {return prepareModifyLobbyRequestAnswer(data);}
//    case parser::GET_LOBBIES_LIST: {return prepareLobbiesListRequest();}
//    case parser::LOBBIES_LIST: {return prepareLobbiesListPacket(data);}
//    default: {return answerPacket;}
//    }
//}
