#include "c_server.h"

c_server::c_server(QObject *parent)
    : QTcpServer(parent)
{
    address = QHostAddress(server::addres);
    port = server::port;
}

c_server::~c_server()
{
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

void c_server::removePeer(c_socket *socket)
{
    QMutableListIterator<c_socket *> i(connectedPeers);
    while (i.hasNext()) {
        if (i.next() == socket) {
            i.remove();
        }
    }
}

void c_server::removePeer(qintptr socketDescriptor)
{
    QMutableListIterator<c_socket *> i(connectedPeers);
    while (i.hasNext()) {
        if (i.next()->getSocketDescriptor() == socketDescriptor){
            i.remove();
        }
    }
}

void c_server::newPeer(c_socket *peer)
{
    connectedPeers.append(peer);
}

void c_server::incomingConnection(qintptr socketDescriptor)
{
    c_socket *peer = new c_socket(socketDescriptor);

    connect(peer, SIGNAL(connectionFinished(c_socket *)), this, SLOT(removePeer(c_socket *)));

    addPendingConnection( const_cast<QTcpSocket *>(&peer->getSocket()) );

    peer->writeToPeer("Fromserv: hhhhh");
}
