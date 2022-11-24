#ifndef C_EVENTCONTROLLER_H
#define C_EVENTCONTROLLER_H

#include "_myData.h"
#include "c_mytimer.h"

#include <QObject>
#include <QTimer>
#include <QPair>

class c_eventController : public QObject
{
    Q_OBJECT
public:
    explicit c_eventController(QObject *parent = nullptr);

    const QTimer &getNewBlockTimer() const;
    const QTimer &getNewFoodTimer() const;
    const QTimer &getNewCoinTimer() const;
    const QTimer &getMovingTimer() const;
    const c_myTimer &getSpeedUpTimer() const;

    int getNextSpeedUpTime() const;
    void setNextSpeedUpTime(int newNextSpeedUpTime);
    int getNextMoveTime() const;
    void setNextMoveTime(int newNextMoveTime);

public slots:
    void stopTimers();

private:
    QTimer newBlockTimer;
    QTimer newFoodTimer;
    QTimer newCoinTimer;
    QTimer movingTimer;
    c_myTimer speedUpTimer{game::speedUpNormalTime};

    int nextSpeedUpTime;
    int nextMoveTime;

private slots:
    void newBlockTimerTimeOut();
    void newFoodTimerTimeOut();
    void newCoinTimerTimeOut();
    void movingTimerTimeOut();
    void speedUpTimerTimeOut();
    void speedUpTimerStarted(int time);
    void speedUpTimerOneSecPassed(int time);

signals:
    void newBlockEvent(QPair<bool, bool> player); //których graczy dotyczsy newBlockEvent: first - player1, second - player2
    void newFoodEvent(QPair<bool, bool> player); //których graczy dotyczsy newFoodEvent: first - player1, second - player2
    void newCoinEvent(QPair<bool, bool> player); //których graczy dotyczsy newCoinEvent: first - player1, second - player2
    void moveEvent(QPair<bool, bool> player); //których graczy dotyczsy moveEvent: first - player1, second - player2
    void speedUpEvent(QPair<bool, bool> player); //których graczy dotyczsy speedUpEvent: first - player1, second - player2
    void speedUpTimerStartedSignal(QPair<bool, bool> player, int time);
    void speedUpTimerOneSecPassedSignal(QPair<bool, bool> player, int time);
};

#endif // C_EVENTCONTROLLER_H
