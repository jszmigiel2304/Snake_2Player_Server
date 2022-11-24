#include "c_board.h"

c_board::c_board(QObject *parent)
    : QObject{parent}
{
    for(int iii = 0; iii < board::boardSize; iii++) {
        for(int jjj = 0; jjj < board::boardSize; jjj++) {
            if (iii == 0 || jjj == 0 || iii == board::boardSize-1 || jjj == board::boardSize-1) {
                board[iii][jjj] = board::WALL;
                continue;
            } else {
                board[iii][jjj] = board::EMPTY;
                continue;
            }
        }
    }
}

const board::boardArray &c_board::getBoard()
{
    return board;
}

void c_board::newBlock(QPoint snakeHeadPosition)
{
    QPoint point;

    do {
        point.setX( QRandomGenerator( QTime::currentTime().msecsSinceStartOfDay() ).bounded(1,49) );
        point.setY( QRandomGenerator( QDateTime::currentMSecsSinceEpoch() ).bounded(1,49) );
    } while(!testDistanceFromSnake(point, snakeHeadPosition));

    if(!isFieldAvailable(point))
        point = seekAvailableFieldInNeighbours(point);

    if(point.isNull())
        return;
    else
        board[point.x()][point.y()] = board::BLOCK;

    emit boardChanged();
}

void c_board::newFood(QPoint snakeHeadPosition)
{
    QPoint point;

    do {
        point.setX( QRandomGenerator( QTime::currentTime().msecsSinceStartOfDay() ).bounded(1,49) );
        point.setY( QRandomGenerator( QDateTime::currentMSecsSinceEpoch() ).bounded(1,49) );
    } while(!testDistanceFromSnake(point, snakeHeadPosition));

    if(!isFieldAvailable(point))
        point = seekAvailableFieldInNeighbours(point);

    if(point.isNull())
        return;
    else
        board[point.x()][point.y()] = board::FOOD_LVL_2;

    emit boardChanged();
}

void c_board::newCoin(QPoint snakeHeadPosition)
{
    QPoint point;

    do {
        point.setX( QRandomGenerator( QTime::currentTime().msecsSinceStartOfDay() ).bounded(1,49) );
        point.setY( QRandomGenerator( QDateTime::currentMSecsSinceEpoch() ).bounded(1,49) );
    } while(!testDistanceFromSnake(point, snakeHeadPosition));

    if(!isFieldAvailable(point))
        point = seekAvailableFieldInNeighbours(point);

    if(point.isNull())
        return;
    else
        board[point.x()][point.y()] = board::COIN_LVL_3;

    emit boardChanged();
}

bool c_board::testDistanceFromSnake(QPoint point, QPoint snakeHeadPoint, int minDistance)
{
    if(qPow(point.x()-snakeHeadPoint.x(), 2) + qPow(point.y()-snakeHeadPoint.y(), 2) < qPow(minDistance, 2)) return false;
    return true;
}

bool c_board::isFieldAvailable(QPoint point)
{
    if(board[point.x()][point.y()] == board::EMPTY) return true;
    return false;
}

bool c_board::isFieldNotWall(QPoint point)
{
    if(board[point.x()][point.y()] == board::WALL || board[point.x()][point.y()] == board::BLOCK ) return false;
    return true;
}

void c_board::clearField(QPoint point)
{
    if( point.x() > 0 && point.x() < board::boardSize - 1 && point.y() > 0 && point.y() < board::boardSize - 1 )
        board[point.x()][point.y()] = board::BoardField::EMPTY;
}


QPoint c_board::seekAvailableFieldInNeighbours(QPoint point, quint8 neighbourousLevel)
{
    if(neighbourousLevel > 2) return getFirstAvailable();

    for(int iii=point.x()-neighbourousLevel; iii<=point.x()+neighbourousLevel; iii++) {
        for(int jjj=point.y()-neighbourousLevel; jjj<=point.y()+neighbourousLevel; jjj++) {
            if( iii > 0 && iii < board::boardSize - 1 && jjj > 0 && jjj < board::boardSize - 1 ) {
                QPoint tempPoint(iii,jjj);
                if( isFieldAvailable( tempPoint ) )
                    return tempPoint;
            }
        }
    }

    return seekAvailableFieldInNeighbours(point, neighbourousLevel+1);
}

QPoint c_board::getFirstAvailable()
{
    for(int iii = 1; iii < board::boardSize - 1; iii++) {
        for(int jjj = 1; jjj < board::boardSize - 1; jjj++) {
            if ( board[iii][jjj] == board::EMPTY)
                return QPoint(iii,jjj);
        }
    }

    return QPoint(0,0);
}
