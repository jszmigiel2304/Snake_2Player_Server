#include "c_snake.h"
#include "c_player.h"

#define _PARENT_ dynamic_cast<c_player *>(parent())

c_snake::c_snake(QObject *parent)
    : QObject{parent}
{
    direction = snake::MOVE_UP;
    snake.append(QPoint(10,10));
    snake.append(QPoint(10,11));
    snake.append(QPoint(10,12));
    snake.append(QPoint(10,13));
    snake.append(QPoint(10,14));
    snake.append(QPoint(10,15));
    snake.append(QPoint(10,16));
    snake.append(QPoint(10,17));
    snake.append(QPoint(10,18));
    snake.append(QPoint(10,19));
}

const QList<QPoint> &c_snake::getSnake() const
{
    return snake;
}

snake::MoveDirection c_snake::getDirection() const
{
    return direction;
}

void c_snake::changeDirection(snake::MoveDirection direction)
{
    setDirection(direction);
    emit snakeChanged();
}

void c_snake::setDirection(snake::MoveDirection newDirection)
{
    direction = newDirection;
}

quint8 c_snake::getSpeed() const
{
    return speed;
}

void c_snake::setSpeed(quint8 newSpeed)
{
    speed = newSpeed;
}

void c_snake::move(snake::MoveDirection direction)
{
    switch(direction) {
    case snake::MOVE_UP: {
        int newY = snake[0].y() - 1;
        if (newY < 0 || newY > board::boardSize-1)
            return;
        moveBody();
        snake[0].setY( newY );
        break;
    }
    case snake::MOVE_DOWN: {
        int newY = snake[0].y() + 1;
        if (newY < 0 || newY > board::boardSize-1)
            return;
        moveBody();
        snake[0].setY( newY );
        break;
    }
    case snake::MOVE_LEFT: {
        int newX = snake[0].x() - 1;
        if (newX < 0 || newX > board::boardSize-1)
            return;
        moveBody();
        snake[0].setX( newX );
        break;
    }
    case snake::MOVE_RIGHT: {
        int newX = snake[0].x() + 1;
        if (newX < 0 || newX > board::boardSize-1)
            return;
        moveBody();
        snake[0].setX( newX );
        break;}
    default: {
        return;
    }
    }
    setDirection(direction);
    emit snakeChanged();
}

void c_snake::extend(quint8 value)
{
    for(int iii=0; iii<value; iii++)
        extend();
}

void c_snake::extend()
{
    QPoint p1 = snake[ snake.size()-1 ];
    QPoint p2 = snake[ snake.size()-2 ];
    quint8 x,y;

    if( p1.x() > p2.x() ) {
        x = p1.x() + 1;
        y = p1.y();
        if(_PARENT_->getBoard()->isFieldNotWall(QPoint(x,y))) {
            snake.append(QPoint(x,y));
            return;
        }
        x = p1.x();
        y = p1.y() - 1;
        if(_PARENT_->getBoard()->isFieldNotWall(QPoint(x,y))) {
            snake.append(QPoint(x,y));
            return;
        }
        x = p1.x();
        y = p1.y() + 1;
        if(_PARENT_->getBoard()->isFieldNotWall(QPoint(x,y))) {
            snake.append(QPoint(x,y));
            return;
        }
    } else if( p1.x() < p2.x() ){
        x = p1.x() - 1;
        y = p1.y();
        if(_PARENT_->getBoard()->isFieldNotWall(QPoint(x,y))) {
            snake.append(QPoint(x,y));
            return;
        }
        x = p1.x();
        y = p1.y() - 1;
        if(_PARENT_->getBoard()->isFieldNotWall(QPoint(x,y))) {
            snake.append(QPoint(x,y));
            return;
        }
        x = p1.x();
        y = p1.y() + 1;
        if(_PARENT_->getBoard()->isFieldNotWall(QPoint(x,y))) {
            snake.append(QPoint(x,y));
            return;
        }
    } else {
        if( p1.y() > p2.y() ) {
            x = p1.x();
            y = p1.y() + 1;
            if(_PARENT_->getBoard()->isFieldNotWall(QPoint(x,y))) {
                snake.append(QPoint(x,y));
                return;
            }
            x = p1.x() - 1;
            y = p1.y();
            if(_PARENT_->getBoard()->isFieldNotWall(QPoint(x,y))) {
                snake.append(QPoint(x,y));
                return;
            }
            x = p1.x() + 1;
            y = p1.y();
            if(_PARENT_->getBoard()->isFieldNotWall(QPoint(x,y))) {
                snake.append(QPoint(x,y));
                return;
            }
        } else if( p1.y() < p2.y() ) {
            x = p1.x();
            y = p1.y() - 1;
            if(_PARENT_->getBoard()->isFieldNotWall(QPoint(x,y))) {
                snake.append(QPoint(x,y));
                return;
            }
            x = p1.x() - 1;
            y = p1.y();
            if(_PARENT_->getBoard()->isFieldNotWall(QPoint(x,y))) {
                snake.append(QPoint(x,y));
                return;
            }
            x = p1.x() + 1;
            y = p1.y();
            if(_PARENT_->getBoard()->isFieldNotWall(QPoint(x,y))) {
                snake.append(QPoint(x,y));
                return;
            }
        }
    }
}

void c_snake::moveBody()
{
    for(int iii = snake.size()-1; iii > 0; iii--) {
        snake[iii].setX( snake[iii-1].x() );
        snake[iii].setY( snake[iii-1].y() );
    }
}

