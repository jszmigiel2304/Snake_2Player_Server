#ifndef C_SERVER_H
#define C_SERVER_H

#include "_myData.h"
#include "c_gamescontroller.h"
#include "c_game.h"
#include "c_socket.h"
#include "c_player.h"

#include <QTcpServer>
#include <QThreadPool>
#include <QObject>
#include <QTcpSocket>
#include <QList>
#include <QPair>
#include <QAbstractSocket>

namespace server {
    struct playerConnection {
        c_socket *connection;
        c_player *player;
        ~playerConnection() {
            qDebug() << "deleted playerConnection";
            connection->deleteLater();
            player->deleteLater();
        }
        QString toString() {
            return QString("%1 [ %2 ]").arg(player->toString(), connection->toString());
        }
    };
}

class c_server : public QTcpServer
{
    Q_OBJECT
public:
    c_server(QObject *parent = nullptr);
    ~c_server();

    bool runServer();
    bool closeServer();

    quint32 removePeers();  //zwraca liszbę usuniętych

    QObject *getGamesControllerConnector() const;
    void setGamesControllerConnector(QObject *newGamesControllerConnector);

public slots:
    void newPeer(server::playerConnection *peer);
    void removePeer(server::playerConnection *peer);
    void removePeer(qintptr socketDescriptor);
    void removePeer(c_socket * socket);

    void sendAnswerToPeer(qintptr socketDescriptor, const QByteArray & answerPacket);


protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QHostAddress address;
    quint16 port;

    QObject * gamesControllerConnector;

    QList<server::playerConnection *> connectedPeers;

private slots:

signals:

};

#endif // C_SERVER_H
