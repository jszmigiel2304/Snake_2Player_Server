#include "c_eventcontroller.h"
#include "c_game.h"

#define _PARENT_ dynamic_cast<c_game *>(parent)

c_eventController::c_eventController(QObject *parent)
    : QObject{parent}
{
    nextSpeedUpTime = game::speedUpNormalTime;
    nextMoveTime = game::beginningMoveTime;

    newBlockTimer.setInterval( game::newBlockOnBoardInterval );
    newBlockTimer.setSingleShot(false);
    newFoodTimer.setInterval( game::newFoodOnBoardInterval );
    newFoodTimer.setSingleShot(false);
    newCoinTimer.setInterval( game::newCoinOnBoardInterval );
    newCoinTimer.setSingleShot(false);
    movingTimer.setInterval(getNextMoveTime());
    movingTimer.setSingleShot(false);


    connect(&newBlockTimer, SIGNAL(timeout()), this, SLOT(newBlockTimerTimeOut()));
    connect(&newFoodTimer, SIGNAL(timeout()), this, SLOT(newFoodTimerTimeOut()));
    connect(&newCoinTimer, SIGNAL(timeout()), this, SLOT(newCoinTimerTimeOut()));
    connect(&movingTimer, SIGNAL(timeout()), this, SLOT(movingTimerTimeOut()));
    connect(&speedUpTimer, SIGNAL(timeOut()), this, SLOT(speedUpTimerTimeOut()));    

    connect( &speedUpTimer, SIGNAL(timerStarted(int)), this, SLOT(speedUpTimerStarted(int)));
    connect( &speedUpTimer, SIGNAL(oneSecPassed(int)), this, SLOT(speedUpTimerOneSecPassed(int)));

//    newBlockTimer.start();
//    newFoodTimer.start();
//    newCoinTimer.start();
//    movingTimer.start();
//    speedUpTimer.start();
}

const QTimer &c_eventController::getNewBlockTimer() const
{
    return newBlockTimer;
}

const QTimer &c_eventController::getNewFoodTimer() const
{
    return newFoodTimer;
}

const QTimer &c_eventController::getNewCoinTimer() const
{
    return newCoinTimer;
}

const QTimer &c_eventController::getMovingTimer() const
{
    return movingTimer;
}

const c_myTimer &c_eventController::getSpeedUpTimer() const
{
    return speedUpTimer;
}

int c_eventController::getNextSpeedUpTime() const
{
    return nextSpeedUpTime;
}

void c_eventController::setNextSpeedUpTime(int newNextSpeedUpTime)
{
    nextSpeedUpTime = newNextSpeedUpTime;
}

int c_eventController::getNextMoveTime() const
{
    return nextMoveTime;
}

void c_eventController::setNextMoveTime(int newNextMoveTime)
{
    nextMoveTime = newNextMoveTime;
}

void c_eventController::stopTimers()
{
    newBlockTimer.stop();
    newFoodTimer.stop();
    newCoinTimer.stop();
    movingTimer.stop();
    speedUpTimer.stop();
}

void c_eventController::newBlockTimerTimeOut()
{
    emit newBlockEvent(QPair<bool,bool>(true,true));
}

void c_eventController::newFoodTimerTimeOut()
{
    emit newFoodEvent(QPair<bool,bool>(true,true));
}

void c_eventController::newCoinTimerTimeOut()
{
    emit newCoinEvent(QPair<bool,bool>(true,true));
}

void c_eventController::movingTimerTimeOut()
{
    emit moveEvent(QPair<bool,bool>(true,true));
}

void c_eventController::speedUpTimerTimeOut()
{
    movingTimer.stop();
    emit speedUpEvent(QPair<bool,bool>(true,true));

    movingTimer.start( getNextMoveTime() );
    speedUpTimer.reset();
    speedUpTimer.start();
}

void c_eventController::speedUpTimerStarted(int time)
{
    emit speedUpTimerStartedSignal(QPair<bool,bool>(true,true), time);
}

void c_eventController::speedUpTimerOneSecPassed(int time)
{
    emit speedUpTimerOneSecPassedSignal(QPair<bool,bool>(true,true), time);
}
