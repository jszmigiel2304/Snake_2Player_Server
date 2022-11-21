#ifndef _MYDATA_H
#define _MYDATA_H


#include <QObject>
#include <QMap>
#include <QList>
#include <QPoint>
#include <QHostAddress>

namespace server {
    const QString addres = "127.0.0.1";
    constexpr quint16 port = 65432;
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

    enum colisionResult : quint8 {
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

#endif // _MYDATA_H
