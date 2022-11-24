#ifndef C_BOARD_H
#define C_BOARD_H

#include "_myData.h"

#include <QObject>
#include <QRandomGenerator>
#include <QtMath>
#include <QTime>
#include <QDateTime>

class c_board : public QObject
{
    Q_OBJECT
public:

    explicit c_board(QObject *parent = nullptr);
    const board::boardArray& getBoard();

    void newBlock(QPoint snakeHeadPosition);
    void newFood(QPoint snakeHeadPosition);
    void newCoin(QPoint snakeHeadPosition);

    bool testDistanceFromSnake(QPoint point, QPoint snakeHeadPoint, int minDistance = 5);
    bool isFieldAvailable(QPoint point);
    bool isFieldNotWall(QPoint point);

public slots:
    void clearField(QPoint point);

private:
    board::boardArray board;

    QPoint seekAvailableFieldInNeighbours(QPoint point, quint8 neighbourousLevel = 1);
    QPoint getFirstAvailable();

private slots:

signals:
    void boardChanged();

};

#endif // C_BOARD_H
