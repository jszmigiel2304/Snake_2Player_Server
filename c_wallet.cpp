#include "c_wallet.h"

c_wallet::c_wallet(QObject *parent)
    : QObject{parent}
{
    coins = 0;
}

qint32 c_wallet::getCoins() const
{
    return coins;
}

void c_wallet::setCoins(qint32 newCoins)
{
    coins = newCoins;
}

void c_wallet::addCoins(quint32 coins)
{
    this->coins += coins;
    walletValueChanged(getCoins());
}

void c_wallet::takeCoins(quint32 coins)
{
    this->coins -= coins;
    walletValueChanged(getCoins());
}
