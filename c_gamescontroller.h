#ifndef C_GAMESCONTROLLER_H
#define C_GAMESCONTROLLER_H

#include "_myData.h"
#include "c_game.h"
#include "c_player.h"
#include "c_parser.h"

#include <QObject>
#include <QList>

class c_gamesController : public QObject
{
    Q_OBJECT
public:
    explicit c_gamesController(QObject *parent = nullptr);
    ~c_gamesController();



    const QList<c_game *> &getGames() const;

public slots:
    void newGame(qintptr socketDescriptor, c_player *owner);
    void newGame(c_game * game);
    void removeGame(qintptr socketDescriptor, const QString &owner);
    void removeGame(c_player * owner);
    void removeGame(const QString &gameName);
    void modifyGame(qintptr socketDescriptor, const QMap<QString, QVariant> &gameInfos);
    void gamesListRequest(qintptr socketDescriptor);

private:
    QList<c_game *> games;

private slots:
    void modifyGame(c_game * game, const QMap<QString, QVariant> &gameInfos);
    void modifyGame(quint32 index, const QMap<QString, QVariant> &gameInfos);

signals:
    void sendAnswerToPeer(qintptr socketDescriptor, const QByteArray &answerPacket);

};

#endif // C_GAMESCONTROLLER_H
