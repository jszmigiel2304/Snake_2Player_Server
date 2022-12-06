#include "c_lobbiescontroller.h"
#include "c_lobby.h"

c_lobbiesController::c_lobbiesController(QObject *parent)
    : QObject{parent}
{

}

c_lobbiesController::~c_lobbiesController()
{

}

const QList<c_lobby *> &c_lobbiesController::getLobbies() const
{
    return lobbies;
}

void c_lobbiesController::newLobby(qintptr socketDescriptor, c_player *owner)
{
    c_lobby * lobby = c_lobby::newLobby(owner);
    lobby->setState(lobby::LOBBY_INITIALIZED);
    lobbies.append( lobby );

    QByteArray answerPacket = c_parser().prepareNewLobbyRequestAnswer(lobby->getLobbyInfo());

    qDebug() << "New lobby created: " << lobby->toString();

    emit sendAnswerToPeer(socketDescriptor, answerPacket);
}

void c_lobbiesController::newLobby(c_lobby *lobby)
{
    lobbies.append(lobby);
    connect(lobby, SIGNAL(destroyed(QObject *)), this, SLOT(removeLobby(QObject *)));

    qDebug() << "Lobby loaded " << lobby->toString();
}

void c_lobbiesController::removeLobby(qintptr socketDescriptor, const QString &owner)
{
    QString on = owner;
    auto modifiedLobby = std::find_if(lobbies.begin(), lobbies.end(),
                                     [&on](const c_lobby * lobby) {return lobby->getOwner()->getName() == on;} );

    QByteArray answerPacket = c_parser().prepareRemoveLobbyRequestAnswer((*modifiedLobby)->getName());

    lobbies.removeAll(*modifiedLobby);

    emit sendAnswerToPeer(socketDescriptor, answerPacket);
}

void c_lobbiesController::removeLobby(c_player *owner)
{
    QMutableListIterator<c_lobby *> i(lobbies);
    while (i.hasNext()) {
        if (i.next()->getOwner() == owner){
            i.remove();
            qDebug() <<  owner->toString() << "Lobby deleted";
        }
    }
}

void c_lobbiesController::removeLobby(const QString &lobbyName)
{
    QMutableListIterator<c_lobby *> i(lobbies);
    while (i.hasNext()) {
        if (i.next()->getName() == lobbyName){
            i.remove();
            qDebug() <<  lobbyName << " Lobby deleted";
        }
    }
}

void c_lobbiesController::modifyLobby(qintptr socketDescriptor, const QMap<QString, QVariant> &lobbyInfos)
{
    QString lm = lobbyInfos["lobby_name"].toString();
    auto modifiedLobby = std::find_if(lobbies.begin(), lobbies.end(),
                                     [&lm](const c_lobby * lobby) {return lobby->getName() == lm;} );
    modifyLobby( (*modifiedLobby), lobbyInfos );

    QByteArray answerPacket = c_parser().prepareModifyLobbyRequestAnswer((*modifiedLobby)->getLobbyInfo());

    emit sendAnswerToPeer(socketDescriptor, answerPacket);
}

void c_lobbiesController::lobbiesListRequest(qintptr socketDescriptor)
{
    QList<lobby::lobbyInformations> lobbiesData;

    foreach (c_lobby * lobby, lobbies)
        lobbiesData.append( lobby->getLobbyInfo() );

    QByteArray answerPacket = c_parser().prepareLobbiesListPacket(lobbiesData);

    emit sendAnswerToPeer(socketDescriptor, answerPacket);
}

void c_lobbiesController::removeLobby(QObject *lobby)
{
    for(int i = 0; i<lobbies.size(); i++) {
        if(lobbies[i] == lobby) {
            lobbies.takeAt(i)->deleteLater();
        }
    }
}

void c_lobbiesController::modifyLobby(c_lobby *lobby, const QMap<QString, QVariant> &lobbyInfos)
{
    lobby->setName( lobbyInfos["lobby_name"].toString() );
    lobby->setOwner( lobbyInfos["owner_name"].toString() );
    lobby->getPlayers().first->setName( lobbyInfos["player_one_name"].toString() );
    lobby->getPlayers().first->setReadyCheck( lobbyInfos["player_one_ready_check"].toBool() );
    lobby->getPlayers().second->setName( lobbyInfos["player_two_name"].toString() );
    lobby->getPlayers().second->setReadyCheck( lobbyInfos["player_two_ready_check"].toBool() );

    lobby->defineState();
}

void c_lobbiesController::modifyLobby(quint32 index, const QMap<QString, QVariant> &lobbyInfos)
{
    lobbies.at(index)->setName( lobbyInfos["lobby_name"].toString() );
    lobbies.at(index)->setOwner( lobbyInfos["owner_name"].toString() );
    lobbies.at(index)->getPlayers().first->setName( lobbyInfos["player_one_name"].toString() );
    lobbies.at(index)->getPlayers().first->setReadyCheck( lobbyInfos["player_one_ready_check"].toBool() );
    lobbies.at(index)->getPlayers().second->setName( lobbyInfos["player_two_name"].toString() );
    lobbies.at(index)->getPlayers().second->setReadyCheck( lobbyInfos["player_two_ready_check"].toBool() );

    lobbies.at(index)->defineState();
}
