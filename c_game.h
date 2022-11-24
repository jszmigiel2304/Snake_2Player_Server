#ifndef C_GAME_H
#define C_GAME_H

#include "_myData.h"
#include "c_eventcontroller.h"
#include "c_player.h"

#include <QObject>
#include <QPair>
#include <QSet>
#include <QUuid>
#include <QVariant>

namespace game {
typedef QPair<c_player *, c_player *> Players;

struct gameInformations {
    QString gameName;
    game::State state;
    QPair<QString, QString> playersNames;
    QPair<bool, bool> playersReadyCheck;

    QMap<QString, QVariant> toMap() {
        QMap<QString, QVariant> map;

        map["game_name"] = gameName;
        map["game_state"] = state;
        map["player_one_name"] = playersNames.first;
        map["player_two_name"] = playersNames.second;
        map["player_one_ready_check"] = playersReadyCheck.first;
        map["player_two_ready_check"] = playersReadyCheck.second;

        return map;
    };
    static gameInformations fromMap(QMap<QString, QVariant> map) {
        gameInformations gameInfos;

        gameInfos.gameName = map["game_name"].toString();
        gameInfos.state = static_cast<game::State>(map["game_state"].toUInt());
        gameInfos.playersNames.first = map["player_one_name"].toString();
        gameInfos.playersNames.second = map["player_two_name"].toString();
        gameInfos.playersReadyCheck.first = map["player_one_ready_check"].toBool();
        gameInfos.playersReadyCheck.second = map["player_two_ready_check"].toBool();

        return gameInfos;
    };
};
}

class c_game : public QObject
{
    Q_OBJECT
public:
    explicit c_game(c_player * gameOwner, QObject *parent = nullptr);
    ~c_game();

    QString toString();

    static c_game * newGame(c_player * gameOwner, QObject *parent = nullptr);

    const QUuid &getIdentifier() const;
    const game::Players &getPlayers() const;
    c_player *getOwner() const;

    const QString &getName() const;

    void setName(const QString &newName);

    game::State getState() const;
    void setState(game::State newState);
    game::State defineState();

    game::gameInformations getGameInfo();

    bool getIsStarted() const;
    void setIsStarted(bool newIsStarted);


    c_eventController *getEventCtrlr() const;

public slots:
    void changeMoveDirection(snake::MoveDirection direction);

private:
    QUuid identifier;
    QString name;
    game::Players players;
    c_player * owner;
    game::State state;
    bool isStarted;

    quint8 speedLevel;

    c_eventController * eventCtrlr;

    game::ColisionResult testCollision(QList<QPoint> &snake, const board::boardArray & board);

    void stopGame(QObject * loserPlayer);


private slots:
    void addNewBlock(QPair<bool, bool> player);
    void addNewFood(QPair<bool, bool> player);
    void addNewCoin(QPair<bool, bool> player);
    void movePlayer(QPair<bool, bool> player);
    void speedUpPlayer(QPair<bool, bool> player);
    void testPotentialCollision(QList<QPoint> & snake, const board::boardArray & board);


signals:
    void speedUpSignal(QPair<bool, bool> player, quint8 speedLevel);
    void notificationMessage(QPair<bool, bool> player, QString msg);
    void stopGameSignal();

};

#endif // C_GAME_H
