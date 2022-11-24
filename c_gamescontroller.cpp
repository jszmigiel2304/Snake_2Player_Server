#include "c_gamescontroller.h"

c_gamesController::c_gamesController(QObject *parent)
    : QObject{parent}
{

}

c_gamesController::~c_gamesController()
{
    qDeleteAll(games.begin(), games.end());
    games.clear();
}

void c_gamesController::newGame(c_game *game)
{
    games.append(game);
    connect(game, SIGNAL(destroyed(QObject *)), this, SLOT(removeGame(QObject *)));

    qDebug() << "Game loaded " << game->toString();
}

void c_gamesController::removeGame(qintptr socketDescriptor, const QString &owner)
{
    QString on = owner;
    auto modifiedGame = std::find_if(games.begin(), games.end(),
                                     [&on](const c_game * game) {return game->getOwner()->getName() == on;} );

    QByteArray answerPacket = c_parser().prepareRemoveGameRequestAnswer((*modifiedGame)->getName());

    games.removeAll(*modifiedGame);

    emit sendAnswerToPeer(socketDescriptor, answerPacket);
}

void c_gamesController::removeGame(c_player *owner)
{
    QMutableListIterator<c_game *> i(games);
    while (i.hasNext()) {
        if (i.next()->getOwner() == owner){
            i.remove();
            qDebug() <<  owner->toString() << "Game deleted";
        }
    }
}

void c_gamesController::removeGame(const QString &gameName)
{
    QMutableListIterator<c_game *> i(games);
    while (i.hasNext()) {
        if (i.next()->getName() == gameName){
            i.remove();
            qDebug() <<  gameName << " Game deleted";
        }
    }
}

void c_gamesController::modifyGame(qintptr socketDescriptor, const QMap<QString, QVariant> &gameInfos)
{
    QString gm = gameInfos["game_name"].toString();
    auto modifiedGame = std::find_if(games.begin(), games.end(),
                                     [&gm](const c_game * game) {return game->getName() == gm;} );
    modifyGame( (*modifiedGame), gameInfos );    

    QByteArray answerPacket = c_parser().prepareModifyGameRequestAnswer((*modifiedGame)->getGameInfo());

    emit sendAnswerToPeer(socketDescriptor, answerPacket);
}

void c_gamesController::gamesListRequest(qintptr socketDescriptor)
{
    QList<game::gameInformations> gamesData;

    foreach (c_game * game, games)
        gamesData.append( game->getGameInfo() );

    QByteArray answerPacket = c_parser().prepareGamesListPacket(gamesData);

    emit sendAnswerToPeer(socketDescriptor, answerPacket);
}

struct gameInformations {
    QString gameName;
    game::State state;
    QPair<QString, QString> playersNames;
    QPair<bool, bool> playersReadyCheck;
};

void c_gamesController::modifyGame(c_game *game, const QMap<QString, QVariant> &gameInfos)
{
    game->setName( gameInfos["game_name"].toString() );
    game->getPlayers().first->setName( gameInfos["player_one_name"].toString() );
    game->getPlayers().first->setReadyCheck( gameInfos["player_one_ready_check"].toBool() );
    game->getPlayers().second->setName( gameInfos["player_two_name"].toString() );
    game->getPlayers().second->setReadyCheck( gameInfos["player_two_ready_check"].toBool() );

    game->defineState();
}

void c_gamesController::modifyGame(quint32 index, const QMap<QString, QVariant> &gameInfos)
{
    games.at(index)->setName( gameInfos["game_name"].toString() );
    games.at(index)->getPlayers().first->setName( gameInfos["player_one_name"].toString() );
    games.at(index)->getPlayers().first->setReadyCheck( gameInfos["player_one_ready_check"].toBool() );
    games.at(index)->getPlayers().second->setName( gameInfos["player_two_name"].toString() );
    games.at(index)->getPlayers().second->setReadyCheck( gameInfos["player_two_ready_check"].toBool() );

    games.at(index)->defineState();
}

const QList<c_game *> &c_gamesController::getGames() const
{
    return games;
}

void c_gamesController::newGame(qintptr socketDescriptor, c_player *owner)
{
    c_game * game = c_game::newGame(owner);
    game->setState(game::GAME_INITIALIZED);
    games.append( game );

    QByteArray answerPacket = c_parser().prepareNewGameRequestAnswer(game->getGameInfo());

    emit sendAnswerToPeer(socketDescriptor, answerPacket);
}

