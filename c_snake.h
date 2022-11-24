#ifndef C_SNAKE_H
#define C_SNAKE_H

#include "_myData.h"

#include <QObject>
#include <QPoint>
#include <QList>

class c_snake : public QObject
{
    Q_OBJECT

public:
    explicit c_snake(QObject *parent = nullptr);

    const QList<QPoint> &getSnake() const;

    snake::MoveDirection getDirection() const;
    void changeDirection(snake::MoveDirection direction);
    void setDirection(snake::MoveDirection newDirection);

    quint8 getSpeed() const;
    void setSpeed(quint8 newSpeed);

public slots:
    void move(snake::MoveDirection direction);
    void extend(quint8 value);
    void extend();

private:
    QList<QPoint> snake;
    snake::MoveDirection direction;
    quint8 speed;

private slots:
    void moveBody();

signals:
    void snakeChanged();

};

#endif // C_SNAKE_H
