#include "c_mytimer.h"


c_myTimer::c_myTimer(int time, int interval, QObject *parent) :
    QTimer(parent), time{time}, interval{interval}
{
    oneSecTimer.setInterval(getInterval());
    oneSecTimer.setSingleShot(false);
    connect(&oneSecTimer, SIGNAL(timeout()), this, SLOT(oneSecTimerTimeOut()));
}

c_myTimer::~c_myTimer()
{

}

int c_myTimer::getInterval() const
{
    return interval;
}

void c_myTimer::setInterval(int newInterval)
{
    interval = newInterval;
}

int c_myTimer::getTime() const
{
    return time;
}

void c_myTimer::setTime(int newTime)
{
    time = newTime;
}

void c_myTimer::start()
{
    remainingTime = getTime();
    oneSecTimer.start();

    emit timerStarted(time);
}

void c_myTimer::reStart()
{
    oneSecTimer.start();
}

void c_myTimer::pause()
{
    oneSecTimer.stop();
}

void c_myTimer::reset()
{
    oneSecTimer.stop();
    remainingTime = getTime();

    emit timerStarted(time);
}

void c_myTimer::stop()
{
    oneSecTimer.stop();

    emit timerStopped(getRemainingTime());
}

int c_myTimer::getRemainingTime() const
{
    return remainingTime;
}

void c_myTimer::setRemainingTime(int newRemainingTime)
{
    remainingTime = newRemainingTime;
}

void c_myTimer::oneSecTimerTimeOut()
{
    remainingTime -= 1000;
    emit oneSecPassed(remainingTime);

    if(remainingTime <= 0)
        emit timeOut();
}
