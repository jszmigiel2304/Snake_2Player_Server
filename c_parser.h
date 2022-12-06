#ifndef C_PARSER_H
#define C_PARSER_H

#include "_myData.h"
#include "c_game.h"
#include "c_lobby.h"

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QPair>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>

class c_parser : public QObject
{
    Q_OBJECT
public:
    explicit c_parser(QObject *parent = nullptr);

    void prepareJSON(parser::PacketContent content, const QList<QMap<QString, QVariant>> &packet_data);
    void ParseReceivedPacket(const QByteArray &data, parser::Packet &outerPacket, qintptr socketDescriptor = -1);

    const QByteArray &prepareSetPlayerNamePacket(const QString &playerName);
    const QByteArray &prepareSetPlayerNameAnswer(const QString &playerName);

    const QByteArray &prepareGamesListRequest();
    const QByteArray &prepareGamesListPacket(const QList<game::gameInformations> &gamesData);

    const QByteArray &prepareLobbiesListRequest();
    const QByteArray &prepareLobbiesListPacket(const QList<lobby::lobbyInformations> &lobbiesData);

    const QByteArray &prepareNewGameRequest(const QString &playerName);
    const QByteArray &prepareNewGameRequestAnswer(const game::gameInformations &game);

    const QByteArray &prepareNewLobbyRequest(const QString &playerName);
    const QByteArray &prepareNewLobbyRequestAnswer(const lobby::lobbyInformations &lobby);

    const QByteArray &prepareModifyGameRequestAnswer(const game::gameInformations &game);
    const QByteArray &prepareModifyLobbyRequestAnswer(const lobby::lobbyInformations &lobby);

    const QByteArray &prepareRemoveGameRequestAnswer(const QString &gameName);
    const QByteArray &prepareRemoveLobbyRequestAnswer(const QString &lobbyName);

private:
    QByteArray answerPacket;

    void saveDocumentToByteArray(const QJsonDocument &doc, QByteArray *outerByteArray = nullptr);

signals:

};

#endif // C_PARSER_H
