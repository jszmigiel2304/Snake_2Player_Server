#ifndef C_SOCKET_H
#define C_SOCKET_H

#include <QTcpSocket>
#include <QObject>

class c_socket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit c_socket(qintptr socketDescriptor, QObject *parent = nullptr);

    qintptr getSocketDescriptor() const;
    const QTcpSocket &getSocket() const;

    bool operator==(const c_socket& socket) const;
    bool operator!=(const c_socket& socket) const;

    void writeToPeer(const char * msg);

public slots:

protected:

private:
    qintptr socketDescriptor;
    QTcpSocket socket;

private slots:
    void peerConnected();
    void peerDisconnected();
    void peerErrorOccurred(QAbstractSocket::SocketError error);
    void peerStateChanged(QAbstractSocket::SocketState state);

    void peerAboutToClose();
    void peerBytesWritten(qint64 bytes);
    void peerReadyRead();

signals:
    void newPeer(c_socket *socket);
    void connectionFinished(c_socket *client);

};

#endif // C_SOCKET_H
