#ifndef C_MYTIMER_H
#define C_MYTIMER_H

#include <QTimer>
#include <QObject>

class c_myTimer : public QTimer
{
    Q_OBJECT
public:
    explicit c_myTimer(int time, int interval = 1000, QObject *parent = nullptr);
    ~c_myTimer();

    int getInterval() const;
    void setInterval(int newInterval);

    int getTime() const;
    void setTime(int newTime);

    void start();
    void reStart();
    void pause();
    void reset();
    void stop();

    int getRemainingTime() const;
    void setRemainingTime(int newRemainingTime);

private:
    int time;
    int interval;
    int remainingTime;
    QTimer oneSecTimer;

private slots:
    void oneSecTimerTimeOut();

signals:
    void timerStarted(int time);
    void oneSecPassed(int remainingTime);
    void timeOut();
    void timerStopped(int remainingTime);
};

#endif // C_MYTIMER_H
