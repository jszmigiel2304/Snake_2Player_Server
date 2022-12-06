#include "c_server.h"
#include "c_gamescontroller.h"
#include "c_lobbiescontroller.h"
#include "c_game.h"
#include "c_player.h"
#include "c_lobby.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    c_server server;
    c_gamesController gamesCtrlr;
    c_lobbiesController lobbiesCtrlr;

    server.setGamesControllerConnector( dynamic_cast<QObject *>(&gamesCtrlr) );
    server.setLobbiesControllerConnector( dynamic_cast<QObject *>(&lobbiesCtrlr) );

    QObject::connect(&gamesCtrlr, SIGNAL(sendAnswerToPeer(qintptr,QByteArray)), &server, SLOT(sendAnswerToPeer(qintptr,QByteArray)));
    QObject::connect(&lobbiesCtrlr, SIGNAL(sendAnswerToPeer(qintptr,QByteArray)), &server, SLOT(sendAnswerToPeer(qintptr,QByteArray)));

    QObject::connect(&a, SIGNAL(aboutToQuit()), &server, SLOT(deleteLater()));
    QObject::connect(&a, SIGNAL(aboutToQuit()), &gamesCtrlr, SLOT(deleteLater()));

    c_player * player = new c_player();
    for(int i = 0; i < 100; i++) {
        c_lobby * lobby = c_lobby::newLobby(player);
        lobbiesCtrlr.newLobby(lobby);
    }

    server.runServer();

    return a.exec();
}
