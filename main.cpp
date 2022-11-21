#include "c_server.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    c_server server;
    server.runServer();

    return a.exec();
}
