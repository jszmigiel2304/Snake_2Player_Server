#include "c_socket.h"

c_socket::c_socket(qintptr socketDescriptor, QObject *parent)
    : QTcpSocket{parent}, socketDescriptor{socketDescriptor}
{
    connect(&socket, SIGNAL(connected()), this, SLOT(peerConnected()));
    connect(&socket, SIGNAL(disconnected()), this, SLOT(peerDisconnected()));
    connect(&socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(peerErrorOccurred(QAbstractSocket::SocketError)));
    connect(&socket, SIGNAL(stateChanged(QAbstractSocket::SocketState )), this, SLOT(peerStateChanged(QAbstractSocket::SocketState)));
    connect(&socket, SIGNAL(aboutToClose()), this, SLOT(peerAboutToClose()));
    connect(&socket, SIGNAL(bytesWritten(qint64)), this, SLOT(peerBytesWritten(qint64)));
    connect(&socket, SIGNAL(readyRead()), this, SLOT(peerReadyRead()));

    socket.open(QIODevice::ReadWrite);

    emit newPeer(this);
}

qintptr c_socket::getSocketDescriptor() const
{
    return socketDescriptor;
}


bool c_socket::operator==(const c_socket &socket) const
{
    return  socketDescriptor == socket.socketDescriptor;
}

bool c_socket::operator!=(const c_socket &socket) const
{
    return !operator==(socket);
}

void c_socket::writeToPeer(const char *msg)
{
    socket.write(msg);
    socket.flush();
    socket.waitForBytesWritten();
}

const QTcpSocket &c_socket::getSocket() const
{
    return socket;
}

void c_socket::peerConnected()
{
    qDebug() << "Peer " << socketDescriptor << " connected.";
}

void c_socket::peerDisconnected()
{
    emit connectionFinished(this);
    qDebug() << "Peer " << socketDescriptor << " disonnected.";
}

void c_socket::peerErrorOccurred(SocketError error)
{
    qDebug() << "Peer " << socketDescriptor << " error occurred: " << error;
}

void c_socket::peerStateChanged(SocketState state)
{
    qDebug() << "Peer " << socketDescriptor << " state changed: " << state;
}

void c_socket::peerAboutToClose()
{
    qDebug() << "Closing Peer " << socketDescriptor << "...";
}

void c_socket::peerBytesWritten(qint64 bytes)
{
    qDebug() << "Peer " << socketDescriptor << " write data: " << bytes << " bytes.";
}

void c_socket::peerReadyRead()
{
    qDebug() << "Peer " << socketDescriptor << "reading data... ";
}
