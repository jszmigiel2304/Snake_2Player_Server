#ifndef C_SERVER_H
#define C_SERVER_H

#include "c_socket.h"
#include "_myData.h"

#include <QTcpServer>
#include <QThreadPool>
#include <QObject>
#include <QTcpSocket>
#include <QList>
#include <QPair>

class c_server : public QTcpServer
{
    Q_OBJECT
public:
    explicit c_server(QObject *parent = nullptr);
    ~c_server();

    bool runServer();
    bool closeServer();

    quint32 removePeers();  //zwraca liszbę usuniętych

public slots:
    void newPeer(c_socket * peer);
    void removePeer(c_socket *socket);
    void removePeer(qintptr socketDescriptor);


protected:
    virtual void incomingConnection(qintptr socketDescriptor);

private:
    QHostAddress address;
    quint16 port;

    QList<c_socket *> connectedPeers;

private slots:

signals:

};

#endif // C_SERVER_H
