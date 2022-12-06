#include "c_socket.h"
#include "c_parser.h"
#include "_myData.h"

c_socket::c_socket(qintptr socketDescriptor, QObject *parent)
    : QTcpSocket{parent}, socketDescriptor{socketDescriptor}
{
    socket.setParent(this);
    socket.setSocketDescriptor(socketDescriptor);

    connect(&socket, SIGNAL(connected()), this, SLOT(peerConnected()));
    connect(&socket, SIGNAL(disconnected()), this, SLOT(peerDisconnected()));
    connect(&socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(peerErrorOccurred(QAbstractSocket::SocketError)));
    connect(&socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(peerStateChanged(QAbstractSocket::SocketState)));
    connect(&socket, SIGNAL(aboutToClose()), this, SLOT(peerAboutToClose()));
    connect(&socket, SIGNAL(bytesWritten(qint64)), this, SLOT(peerBytesWritten(qint64)));
    connect(&socket, SIGNAL(readyRead()), this, SLOT(peerReadyRead()));
    connect(this, SIGNAL(dataReceived(QByteArray, qintptr)), this, SLOT(processReceivedData(QByteArray, qintptr)));

    connect(this, SIGNAL(readingPacketErrorSignal()), this, SLOT(readingPacketError()));

    socket.open(QIODevice::ReadWrite);
}

c_socket::~c_socket()
{
    if(socket.state() == QAbstractSocket::ConnectedState)
    {
        socket.disconnectFromHost();
    }
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

void c_socket::write(QByteArray data)
{
    socket.write(data);
    writeEndOfPacket();
    socket.waitForBytesWritten();
}


QString c_socket::toString()
{
    return QString("Peer: %1").arg(socketDescriptor);
}

void c_socket::writeEndOfPacket()
{
    const char *eop {"\n"};
    socket.write(eop);
    socket.flush();
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
    Q_UNUSED(error)
    //qDebug() << "Peer " << socketDescriptor << " error occurred: " << error;
}

void c_socket::peerStateChanged(SocketState state)
{
    Q_UNUSED(state)
    //qDebug() << "Peer " << socketDescriptor << " state changed: " << state;
}

void c_socket::peerAboutToClose()
{
    //qDebug() << "Closing Peer " << socketDescriptor << "...";
}

void c_socket::peerBytesWritten(qint64 bytes)
{
    Q_UNUSED(bytes)
    //qDebug() << "Peer " << socketDescriptor << " write data: " << bytes << " bytes.";
}

void c_socket::peerReadyRead()
{
    while(socket.canReadLine()) {
        QByteArray data = socket.readLine();
        qDebug() << "Peer " << socketDescriptor << "reading data... [ " << data.size() << " bytes]";

        emit dataReceived(data, socketDescriptor);
    }
}

void c_socket::processReceivedData(QByteArray data, qintptr socketDescriptor)
{
    parser::Packet packet;
    c_parser().ParseReceivedPacket(data, packet, socketDescriptor);

    switch (packet.content) {
    case parser::EMPTY: { return;}
    case parser::ERROR_READING_JSON: { emit readingPacketErrorSignal(); return; }
    case parser::SET_PLAYER_NAME: { emit playersNameReceivedSignal(socketDescriptor, packet.data.at(0)["player_name"].toString()); return; }
    case parser::CREATE_NEW_LOBBY: { emit newLobbyRequest(socketDescriptor, packet.data.at(0)["player_name"].toString()); return;}
    case parser::REMOVE_LOBBY: { emit removeLobbyRequest(socketDescriptor, packet.data.at(0)["lobby_name"].toString()); return;}
    case parser::LOBBY_INFOS_CHANGED: { emit lobbyInformationsChanged(socketDescriptor, packet.data.at(0)); return;}
    case parser::GET_LOBBIES_LIST: { emit lobbiesListRequest(socketDescriptor ); return;}
    default: {break;}
    }
}

void c_socket::readingPacketError()
{
    qDebug() << "Packet reading error.";
}
