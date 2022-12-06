#ifndef C_LOBBY_H
#define C_LOBBY_H

#include "_myData.h"
#include "c_player.h"

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QUuid>
#include <QPair>
#include <QDebug>

namespace lobby {

    struct lobbyInformations {
        QString lobbyName;
        lobby::State state;
        QString ownerName;
        QPair<QString, QString> playersNames;
        QPair<bool, bool> playersReadyCheck;

        QMap<QString, QVariant> toMap() {
            QMap<QString, QVariant> map;

            map["lobby_name"] = lobbyName;
            map["lobby_state"] = state;
            map["player_one_name"] = playersNames.first;
            map["player_two_name"] = playersNames.second;
            map["player_one_ready_check"] = playersReadyCheck.first;
            map["player_two_ready_check"] = playersReadyCheck.second;
            map["owner_name"] = ownerName;

            return map;
        };
        static lobbyInformations fromMap(QMap<QString, QVariant> map) {
            lobbyInformations lobbyInfos;

            lobbyInfos.lobbyName = map["lobby_name"].toString();
            lobbyInfos.state = static_cast<lobby::State>(map["lobby_state"].toUInt());
            lobbyInfos.playersNames.first = map["player_one_name"].toString();
            lobbyInfos.playersNames.second = map["player_two_name"].toString();
            lobbyInfos.playersReadyCheck.first = map["player_one_ready_check"].toBool();
            lobbyInfos.playersReadyCheck.second = map["player_two_ready_check"].toBool();
            lobbyInfos.ownerName = map["owner_name"].toString();

            return lobbyInfos;
        };
    };
}

class c_lobby : public QObject
{
    Q_OBJECT
public:
    explicit c_lobby(c_player * gameOwner, QObject *parent = nullptr);
    ~c_lobby();

    QString toString();
    static c_lobby * newLobby(c_player * gameOwner, QObject *parent = nullptr);

    const QUuid &getIdentifier() const;
    const QPair<c_player *, c_player *> &getPlayers() const;
    c_player *getOwner() const;

    const QString &getName() const;

    void setName(const QString &newName);

    lobby::State getState() const;
    void setState(lobby::State newState);
    lobby::State defineState();

    lobby::lobbyInformations getLobbyInfo();

    void setOwner(const QString &ownerName);

public slots:

private:
    QUuid identifier;
    QString name;
    QPair<c_player *, c_player *> players;
    c_player * owner;
    lobby::State state;

private slots:

signals:

};

#endif // C_LOBBY_H
