#ifndef C_WALLET_H
#define C_WALLET_H

#include <QObject>

class c_wallet : public QObject
{
    Q_OBJECT
public:
    explicit c_wallet(QObject *parent = nullptr);

    qint32 getCoins() const;
    void setCoins(qint32 newCoins);

public slots:
    void addCoins(quint32 coins);
    void takeCoins(quint32 coins);

private:
    qint32 coins;

private slots:

signals:
    void walletValueChanged(qint32 coins);

};

#endif // C_WALLET_H
