#ifndef C_LOBBIESCONTROLLER_H
#define C_LOBBIESCONTROLLER_H

#include "_myData.h"
#include "c_lobby.h"
#include "c_player.h"
#include "c_parser.h"

#include <QObject>
#include <QList>

class c_lobbiesController : public QObject
{
    Q_OBJECT
public:
    explicit c_lobbiesController(QObject *parent = nullptr);
    ~c_lobbiesController();

    const QList<c_lobby *> &getLobbies() const;

public slots:
    void newLobby(qintptr socketDescriptor, c_player *owner);
    void newLobby(c_lobby * lobby);
    void removeLobby(qintptr socketDescriptor, const QString &owner);
    void removeLobby(c_player * owner);
    void removeLobby(const QString &lobbyName);
    void modifyLobby(qintptr socketDescriptor, const QMap<QString, QVariant> &lobbyInfos);
    void lobbiesListRequest(qintptr socketDescriptor);

private:
    QList<c_lobby *> lobbies;

private slots:
    void removeLobby(QObject * lobby);
    void modifyLobby(c_lobby * lobby, const QMap<QString, QVariant> &lobbyInfos);
    void modifyLobby(quint32 index, const QMap<QString, QVariant> &lobbyInfos);

signals:
    void sendAnswerToPeer(qintptr socketDescriptor, const QByteArray &answerPacket);

};

#endif // C_LOBBIESCONTROLLER_H
