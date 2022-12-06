#ifndef C_SOCKET_H
#define C_SOCKET_H

#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>

class c_socket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit c_socket(qintptr socketDescriptor, QObject *parent = nullptr);
    ~c_socket();

    qintptr getSocketDescriptor() const;
    const QTcpSocket &getSocket() const;

    bool operator==(const c_socket& socket) const;
    bool operator!=(const c_socket& socket) const;

    void write(QByteArray data);

    QString toString();


public slots:

protected:

private:
    qintptr socketDescriptor;
    QTcpSocket socket;

private slots:
    void writeEndOfPacket();

    void peerConnected();
    void peerDisconnected();
    void peerErrorOccurred(QAbstractSocket::SocketError error);
    void peerStateChanged(QAbstractSocket::SocketState state);

    void peerAboutToClose();
    void peerBytesWritten(qint64 bytes);
    void peerReadyRead();

    void processReceivedData(QByteArray data, qintptr socketDescriptor);
    void readingPacketError();

signals:
    void connectionFinished(c_socket *client);
    void dataReceived(QByteArray data, qintptr socketDescriptor);

    void readingPacketErrorSignal();
    void playersNameReceivedSignal(qintptr socketDescriptor, const QString &name);
    void newLobbyRequest(qintptr socketDescriptor, const QString&playerName);
    void removeLobbyRequest(qintptr socketDescriptor, const QString &lobbyName);
    void lobbyInformationsChanged(qintptr socketDescriptor, const QMap<QString, QVariant> &lobbyInfos);
    void lobbiesListRequest(qintptr socketDescripto);
};

#endif // C_SOCKET_H
