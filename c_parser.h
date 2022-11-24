#ifndef C_PARSER_H
#define C_PARSER_H

#include "_myData.h"
#include "c_game.h"

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

    QByteArray prepareSetPlayerNamePacket(const QString &playerName);
    QByteArray prepareSetPlayerNameAnswer(const QString &playerName);
    QByteArray prepareGamesListRequest();
    QByteArray prepareGamesListPacket(const QList<game::gameInformations> &gamesData);
    QByteArray prepareNewGameRequest();
    QByteArray prepareNewGameRequestAnswer(const game::gameInformations &game);
    QByteArray prepareModifyGameRequestAnswer(const game::gameInformations &game);
    QByteArray prepareRemoveGameRequestAnswer(const QString &gameName);

    QJsonDocument prepareJSON(parser::PacketContent content, const QList<QMap<QString, QVariant>> &packet_data);
    parser::Packet ParseReceivedPacket(QByteArray data, qintptr socketDescriptor);


signals:

};

#endif // C_PARSER_H
