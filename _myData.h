#ifndef _MYDATA_H
#define _MYDATA_H

//#include "c_player.h"
//#include "c_socket.h"

#include <QObject>
#include <QMap>
#include <QList>
#include <QPoint>
#include <QHostAddress>
#include <QPair>
#include <QString>


namespace server {
    static const QString addres{"127.0.0.1"};
    constexpr quint16 port = 65432;

//    struct playerConnection {
//        c_socket *connection;
//        c_player *player;
//        ~playerConnection() {
//            qDebug() << "deleted playerConnection";
//            connection->deleteLater();
//            player->deleteLater();
//        }
//        QString toString() {
//            return QString("%1 [ %2 ]").arg(player->toString(), connection->toString());
//        }
//    };
}

namespace game {
    constexpr quint32 speedUpNormalTime = 3000;
    constexpr quint32 speedUpReducedTime = 7500;
    constexpr quint32 speedUpEnlargedTime = 12500;

    constexpr quint32 beginningMoveTime = 410;

    constexpr quint32 speedUpValue = 20;
    constexpr quint32 maxSpeedUpLevel = 20;

    constexpr quint32 newBlockOnBoardInterval = 1000;
    constexpr quint32 newFoodOnBoardInterval = 500;
    constexpr quint32 newCoinOnBoardInterval = 200;

    constexpr quint32 prizeLvl1 = 100;
    constexpr quint32 prizeLvl2 = 200;
    constexpr quint32 prizeLvl3 = 300;

    constexpr quint8 feedLvl1 = 1;
    constexpr quint8 feedLvl2 = 5;

    enum ColisionResult : quint8 {
        NO_COLLISION = 0,
        BLOCK_COLLISION = 10,
        SNAKE_BODY_COLLISION = 11,
        FOOD_LVL_1_COLLISION = 21,
        FOOD_LVL_2_COLLISION = 22,
        COIN_LVL_1_COLLISION = 30,
        COIN_LVL_2_COLLISION = 31,
        COIN_LVL_3_COLLISION = 32,
        UNDEFINED_COLLISION = 255
    };

    enum State : quint8 {
        GAME_CREATED_NOT_INITIALIZED,
        GAME_INITIALIZED,
        GAME_WAITING_FOR_PLAYERS_READY_CHECK,
        GAME_READY_TO_START,
        GAME_STARTED,
        GAME_FINISHED
    };

//    typedef QPair<c_player *, c_player *> Players;

//    struct gameInformations {
//        QString gameName;
//        game::State state;
//        QPair<QString, QString> playersNames;
//        QPair<bool, bool> playersReadyCheck;

//        QMap<QString, QVariant> toMap() {
//            QMap<QString, QVariant> map;

//            map["game_name"] = gameName;
//            map["game_state"] = state;
//            map["player_one_name"] = playersNames.first;
//            map["player_two_name"] = playersNames.second;
//            map["player_one_ready_check"] = playersReadyCheck.first;
//            map["player_two_ready_check"] = playersReadyCheck.second;

//            return map;
//        };
//        static gameInformations fromMap(QMap<QString, QVariant> map) {
//            gameInformations gameInfos;

//            gameInfos.gameName = map["game_name"].toString();
//            gameInfos.state = static_cast<game::State>(map["game_state"].toUInt());
//            gameInfos.playersNames.first = map["player_one_name"].toString();
//            gameInfos.playersNames.second = map["player_two_name"].toString();
//            gameInfos.playersReadyCheck.first = map["player_one_ready_check"].toBool();
//            gameInfos.playersReadyCheck.second = map["player_two_ready_check"].toBool();

//            return gameInfos;
//        };
//    };

}

namespace board {
    constexpr quint8 boardSize = 50;

    enum BoardField : quint8 {
        EMPTY = 0,
        WALL = 1,
        BLOCK = 2,
        SNAKE_HEAD_DOWN = 10,
        SNAKE_HEAD_UP = 11,
        SNAKE_HEAD_LEFT = 12,
        SNAKE_HEAD_RIGHT = 13,
        SNAKE_BODY_HORIZONTAL = 14,
        SNAKE_BODY_VERTICAL = 15,
        SNAKE_BODY_BEND_LT = 16,
        SNAKE_BODY_BEND_LB = 17,
        SNAKE_BODY_BEND_RT = 18,
        SNAKE_BODY_BEND_RB = 19,
        SNAKE_TAIL_DOWN = 20,
        SNAKE_TAIL_UP = 21,
        SNAKE_TAIL_LEFT = 22,
        SNAKE_TAIL_RIGHT = 23,
        FOOD_LVL_1 = 50,
        FOOD_LVL_2 = 51,
        COIN_LVL_1= 52,
        COIN_LVL_2 = 53,
        COIN_LVL_3 = 54,
    };

    typedef BoardField boardArray[board::boardSize][board::boardSize];

} // namespace board

namespace snake {

    enum MoveDirection : quint8 {
        MOVE_UP,
        MOVE_RIGHT,
        MOVE_DOWN,
        MOVE_LEFT
    };

    struct snake {
        QList<QPoint> snake;
        MoveDirection direction;
    };

}

namespace parser {
    enum PacketContent : quint8 {
        EMPTY = 0,
        ERROR_READING_JSON = 1,
        SET_PLAYER_NAME = 10,
        SET_PLAYER_NAME_ANSWER = 11,
        CREATE_NEW_GAME = 20,
        GAME_CREATED = 21,
        REMOVE_GAME = 22,
        GAME_INFOS_CHANGED = 23,
        GET_GAMES_LIST = 24,
        GAMES_LIST = 25,
        GAME_REMOVED = 29,
        GAME_CREATED_STARTING_BOARD = 30,
        GAME_START = 31,
        GAME_STARTED = 32,
        GAME_STATE_CHANGED = 33,
        GAME_BOARD_STATE_CHANGED = 34,
        GAME_SNAKE_MOVE_DIRECTION_CHANGED = 35,
        GAME_PLAYER_COINS_NUMBER_CHANGED = 36,
        GAME_PLAYER_SHOP_CHANGED = 37
    };

    struct Packet {
        qintptr socketDescriptor;
        parser::PacketContent content;
        QList<QMap<QString, QVariant>> data;
    };
}

#endif // _MYDATA_H
