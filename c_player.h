#ifndef C_PLAYER_H
#define C_PLAYER_H

#include "c_wallet.h"
#include "c_board.h"
#include "c_snake.h"

#include <QObject>
#include <QIODevice>
#include <QDebug>

class c_player : public QObject
{
    Q_OBJECT
public:
    explicit c_player(QObject *parent = nullptr);
    ~c_player();

    QString toString();

    const QString &getName() const;
    bool operator==(const c_player &player) const;
    bool operator!=(const c_player &player) const;

    bool getReadyCheck() const;
    void setReadyCheck(bool newReadyCheck);

    c_wallet *getWallet() const;

    c_board *getBoard() const;
    void setBoard(c_board *newBoard);

    c_snake *getSnake() const;
    void setSnake(c_snake *newSnake);

    snake::MoveDirection getNextMoveDirection() const;
    void setNextMoveDirection(snake::MoveDirection newNextMoveDirection);


public slots:
    void setName(const QString &newName);
    void setName(qintptr socketDescriptor, const QString &newName);
    void orderNewGame(qintptr socketDescriptor);

    void move();
    void speedUp(quint8 speedLevel);
    void feedSnake(quint8 foodAmount);
    void coinsToWallet(quint32 coins);
    void takeCoinsFromWallet(quint32 coins);

private:
    QString name;
    bool readyCheck;
    c_wallet * wallet;
    c_board * board;
    c_snake * snake;
    snake::MoveDirection nextMoveDirection;

private slots:
    void boardChanged();
    void snakeChanged();

signals:
    void sendAnswerToPeer(qintptr socketDescriptor, const QByteArray &answerPacket);
    void orderNewGameSignal(qintptr socketDescriptor, c_player * gameOwner);
    void boardChangedSignal(const board::boardArray &board, const c_snake &snake);
    void potentialCollisionSignal(QList<QPoint> & snake, const board::boardArray & board);

};

#endif // C_PLAYER_H
