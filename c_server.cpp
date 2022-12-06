#include "c_server.h"
#include "c_player.h"
#include "c_socket.h"

#define _GAMES_CTRLR_ dynamic_cast<c_gamesController *>(getGamesControllerConnector())
#define _LOBBIES_CTRLR_ dynamic_cast<c_lobbiesController *>(getLobbiesControllerConnector())

c_server::c_server(QObject *parent)
    : QTcpServer(parent)
{
    address = QHostAddress(server::addres);
    port = server::port;

    gamesControllerConnector = nullptr;
    lobbiesControllerConnector = nullptr;
}

c_server::~c_server()
{
    if(isListening())
        closeServer();

    if(!connectedPeers.isEmpty())
        removePeers();
}

bool c_server::runServer()
{
    bool ret = listen(address, port);

    if(isListening())
        qDebug() << "Server listening: " << address.toString() << ":" << port << "...";
    else
        qDebug() << "Server NOT listening: " << address.toString() << ":" << port << "...";

    return ret;
}

bool c_server::closeServer()
{
    close();

    bool ret = isListening();

    if(!ret)
        qDebug() << "Server closed: " << address.toString() << ":" << port << "...";
    else
        qDebug() << "Server NOT closed: " << address.toString() << ":" << port << "...";

    return !ret;
}

quint32 c_server::removePeers()
{
    quint32 peersNumber = connectedPeers.size();
    qDeleteAll(connectedPeers.begin(), connectedPeers.end());
    connectedPeers.clear();
    return peersNumber;
}

void c_server::removePeer(server::playerConnection *peer)
{
    QMutableListIterator<server::playerConnection *> i(connectedPeers);
    while (i.hasNext()) {
        if (i.next()->connection == peer->connection) {
            i.value()->connection->deleteLater();
            i.value()->player->deleteLater();
            i.remove();
        }
    }
}

void c_server::removePeer(qintptr socketDescriptor)
{
    QMutableListIterator<server::playerConnection *> i(connectedPeers);
    while (i.hasNext()) {
        if (i.next()->connection->getSocketDescriptor() == socketDescriptor){
            i.value()->connection->deleteLater();
            i.value()->player->deleteLater();
            i.remove();
        }
    }
}

void c_server::removePeer(c_socket *socket)
{
    QMutableListIterator<server::playerConnection *> i(connectedPeers);
    while (i.hasNext()) {
        if (i.next()->connection->getSocketDescriptor() == socket->getSocketDescriptor()){
            i.value()->connection->deleteLater();
            dynamic_cast<c_gamesController *>(gamesControllerConnector)->removeGame(i.value()->player);
            dynamic_cast<c_lobbiesController *>(lobbiesControllerConnector)->removeLobby(i.value()->player);
            i.value()->player->deleteLater();
            i.remove();
            qDebug() << "Peer removed";
        }
    }    
}

void c_server::sendAnswerToPeer(qintptr socketDescriptor, const QByteArray &answerPacket)
{
    qintptr sd = socketDescriptor;
    auto peer = std::find_if(connectedPeers.begin(), connectedPeers.end(),
                                     [&sd](const server::playerConnection * peer) {return peer->connection->getSocketDescriptor() == sd;} );

    (*peer)->connection->write(answerPacket);
}

void c_server::newPeer(server::playerConnection *peer)
{
    connectedPeers.append(peer);
}

void c_server::incomingConnection(qintptr socketDescriptor)
{
    c_socket *socket = new c_socket(socketDescriptor);
    c_player *player = new c_player();
    server::playerConnection *peer = new server::playerConnection{socket, player};    

    connect(socket, SIGNAL(connectionFinished(c_socket*)), this, SLOT(removePeer(c_socket*)));

    connect(socket, SIGNAL(playersNameReceivedSignal(qintptr, QString)), player, SLOT(setName(qintptr, QString)));
    connect(socket, SIGNAL(newLobbyRequest(qintptr, const QString&)), player, SLOT(orderNewLobby(qintptr, const QString&)));
    connect(player, SIGNAL(orderNewLobbySignal(qintptr, c_player*)), _LOBBIES_CTRLR_, SLOT(newLobby(qintptr, c_player*)));
    connect(player, SIGNAL(sendAnswerToPeer(qintptr,QByteArray)), this, SLOT(sendAnswerToPeer(qintptr,QByteArray)));
    connect(socket, SIGNAL(removeLobbyRequest(qintptr, const QString &)), _LOBBIES_CTRLR_, SLOT(removeLobby(qintptr, const QString&)));
    connect(socket, SIGNAL(lobbyInformationsChanged(qintptr, QMap<QString, QVariant>)), _LOBBIES_CTRLR_, SLOT(modifyLobby(qintptr, QMap<QString, QVariant>)));
    connect(socket, SIGNAL(lobbiesListRequest(qintptr)), _LOBBIES_CTRLR_, SLOT(lobbiesListRequest(qintptr)));

    addPendingConnection( const_cast<QTcpSocket *>(&socket->getSocket()) );

    connectedPeers.append(peer);
}

QObject *c_server::getLobbiesControllerConnector() const
{
    return lobbiesControllerConnector;
}

void c_server::setLobbiesControllerConnector(QObject *newLobbiesControllerConnector)
{
    lobbiesControllerConnector = newLobbiesControllerConnector;
}

QObject *c_server::getGamesControllerConnector() const
{
    return gamesControllerConnector;
}

void c_server::setGamesControllerConnector(QObject *newGamesControllerConnector)
{
    gamesControllerConnector = newGamesControllerConnector;
}
