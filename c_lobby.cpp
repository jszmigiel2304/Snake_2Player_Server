#include "c_lobby.h"

c_lobby::c_lobby(c_player *gameOwner, QObject *parent)
    : QObject{parent}, owner(gameOwner)
{
    players.first = gameOwner;
    players.second = nullptr;
    owner = gameOwner;

    identifier = QUuid::createUuid();
    name = identifier.toString(QUuid::WithoutBraces);
    players.first = gameOwner;
    setState(lobby::LOBBY_CREATED_NOT_INITIALIZED);
}

c_lobby::~c_lobby()
{
    qDebug() << "Lobby removed " << toString();
}

QString c_lobby::toString()
{
    return QString("%1 [ %2 ]").arg(name, owner->toString());
}

c_lobby *c_lobby::newLobby(c_player *gameOwner, QObject *parent)
{
    return new c_lobby(gameOwner, parent);
}

const QUuid &c_lobby::getIdentifier() const
{
    return identifier;
}

const QPair<c_player *, c_player *> &c_lobby::getPlayers() const
{
    return players;
}

c_player *c_lobby::getOwner() const
{
    return owner;
}

const QString &c_lobby::getName() const
{
    return name;
}

void c_lobby::setName(const QString &newName)
{
    name = newName;
}

lobby::State c_lobby::getState() const
{
    return state;
}

void c_lobby::setState(lobby::State newState)
{
    state = newState;
}

lobby::State c_lobby::defineState()
{
    if(players.first != nullptr && players.second != nullptr) {
        if(players.first->getReadyCheck() && players.second->getReadyCheck()) {
            return lobby::LOBBY_READY_TO_START;
        } else {
            return lobby::LOBBY_WAITING_FOR_PLAYERS_READY_CHECK;
        }
    }

    return lobby::LOBBY_INITIALIZED;
}

lobby::lobbyInformations c_lobby::getLobbyInfo()
{
    lobby::lobbyInformations lobbyInfo;

    lobbyInfo.lobbyName = getName();
    lobbyInfo.ownerName = owner->getName();
    lobbyInfo.state = getState();

    if(players.first != nullptr) {
        lobbyInfo.playersNames.first = getPlayers().first->getName();
        lobbyInfo.playersReadyCheck.first = getPlayers().first->getReadyCheck();
    } else {
        lobbyInfo.playersNames.first = QString();
        lobbyInfo.playersReadyCheck.first = false;
    }

    if(players.second != nullptr) {
        lobbyInfo.playersNames.second = getPlayers().second->getName();
        lobbyInfo.playersReadyCheck.second = getPlayers().second->getReadyCheck();
    } else {
        lobbyInfo.playersNames.second = QString();
        lobbyInfo.playersReadyCheck.second = false;
    }

    return lobbyInfo;
}

void c_lobby::setOwner(const QString &ownerName)
{
    if(players.first->getName() == ownerName) {owner = players.first; return; }
    if(players.second->getName() == ownerName) {owner = players.second; return; }

    qDebug() << "Nie udało się ustawić włąściciela lobby !!!!";
}
