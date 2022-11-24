#include "c_server.h"
#include "c_gamescontroller.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    c_server server;
    c_gamesController gamesCtrlr;

    server.setGamesControllerConnector( dynamic_cast<QObject *>(&gamesCtrlr) );

    QObject::connect(&gamesCtrlr, SIGNAL(sendAnswerToPeer(qintptr,QByteArray)), &server, SLOT(sendAnswerToPeer(qintptr,QByteArray)));

    QObject::connect(&a, SIGNAL(aboutToQuit()), &server, SLOT(deleteLater()));
    QObject::connect(&a, SIGNAL(aboutToQuit()), &gamesCtrlr, SLOT(deleteLater()));



    server.runServer();

    return a.exec();
}
