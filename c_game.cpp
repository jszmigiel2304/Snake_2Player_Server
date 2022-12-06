#include "c_game.h"

c_game::c_game(c_player * gameOwner, QObject *parent)
    : QObject{parent}, owner(gameOwner)
{
    isStarted = false;
    players.first = gameOwner;
    players.second = nullptr;
    owner = gameOwner;

    identifier = QUuid::createUuid();
    name = identifier.toString(QUuid::WithoutBraces);
    players.first = gameOwner;
    setState(game::GAME_CREATED_NOT_INITIALIZED);

    eventCtrlr = new c_eventController(this);
    speedLevel = 1;

    connect(eventCtrlr, SIGNAL(newBlockEvent(QPair<bool, bool> )), this, SLOT(addNewBlock(QPair<bool, bool> )));
    connect(eventCtrlr, SIGNAL(newFoodEvent(QPair<bool, bool> )), this, SLOT(addNewFood(QPair<bool, bool> )));
    connect(eventCtrlr, SIGNAL(newCoinEvent(QPair<bool, bool> )), this, SLOT(addNewCoin(QPair<bool, bool> )));
    connect(eventCtrlr, SIGNAL(moveEvent(QPair<bool, bool> )), this, SLOT(movePlayer(QPair<bool, bool> )));
    connect(eventCtrlr, SIGNAL(speedUpEvent(QPair<bool, bool>)), this, SLOT(speedUpPlayer(QPair<bool, bool>)), Qt::DirectConnection);

//    connect(players.first, SIGNAL(potentialCollisionSignal(QList<QPoint> &, const board::boardArray &)), this, SLOT(testPotentialCollision(QList<QPoint> &, const board::boardArray &)), Qt::DirectConnection);
//    connect(players.second, SIGNAL(potentialCollisionSignal(QList<QPoint> &, const board::boardArray &)), this, SLOT(testPotentialCollision(QList<QPoint> &, const board::boardArray &)), Qt::DirectConnection);

    connect(this, SIGNAL(stopGameSignal()), eventCtrlr, SLOT(stopTimers()), Qt::DirectConnection);
}

c_game::~c_game()
{
    qDebug() << "Game removed " << toString();
}

QString c_game::toString()
{
    return QString("%1 [ %2 ]").arg(name, owner->toString());
}

c_game *c_game::newGame(c_player *gameOwner, QObject *parent)
{
    return new c_game(gameOwner, parent);
}

const QUuid &c_game::getIdentifier() const
{
    return identifier;
}

const game::Players &c_game::getPlayers() const
{
    return players;
}

c_player *c_game::getOwner() const
{
    return owner;
}

const QString &c_game::getName() const
{
    return name;
}

void c_game::setName(const QString &newName)
{
    name = newName;
}

game::State c_game::getState() const
{
    return state;
}

void c_game::setState(game::State newState)
{
    state = newState;
}

game::State c_game::defineState()
{
    if(isStarted) return game::GAME_STARTED;

    if(players.first != nullptr && players.second != nullptr) {
        if(players.first->getReadyCheck() && players.second->getReadyCheck()) {
            return game::GAME_READY_TO_START;
        } else {
            return game::GAME_WAITING_FOR_PLAYERS_READY_CHECK;
        }
    }

    return game::GAME_INITIALIZED;
}

game::gameInformations c_game::getGameInfo()
{
    game::gameInformations gameInfo;

    gameInfo.gameName = getName();
    gameInfo.ownerName = owner->getName();
    gameInfo.state = getState();

    if(players.first != nullptr) {
        gameInfo.playersNames.first = getPlayers().first->getName();
        gameInfo.playersReadyCheck.first = getPlayers().first->getReadyCheck();
    } else {
        gameInfo.playersNames.first = QString();
        gameInfo.playersReadyCheck.first = false;
    }

    if(players.second != nullptr) {
        gameInfo.playersNames.second = getPlayers().second->getName();
        gameInfo.playersReadyCheck.second = getPlayers().second->getReadyCheck();
    } else {
        gameInfo.playersNames.second = QString();
        gameInfo.playersReadyCheck.second = false;
    }

    return gameInfo;
}

bool c_game::getIsStarted() const
{
    return isStarted;
}

void c_game::setIsStarted(bool newIsStarted)
{
    isStarted = newIsStarted;
}

void c_game::setOwner(const QString &ownerName)
{
    if(players.first->getName() == ownerName) {owner = players.first; return; }
    if(players.second->getName() == ownerName) {owner = players.second; return; }

    qDebug() << "Nie udało się ustawić włąściciela gry !!!!";
}

void c_game::changeMoveDirection(snake::MoveDirection direction)
{

}

game::ColisionResult c_game::testCollision(QList<QPoint> &snake, const board::boardArray &board)
{
    int x = snake.first().x();
    int y = snake.first().y();

    for(int iii = 1; iii<snake.size(); iii++)
        if(snake[iii].x() == x && snake[iii].y() == y)
            return game::SNAKE_BODY_COLLISION;

    board::BoardField field = board[x][y];

    if(static_cast<quint8>(field) == 0) return game::NO_COLLISION;
    else if(static_cast<quint8>(field) >= 1 && static_cast<quint8>(field) <= 2) return game::BLOCK_COLLISION;
    else if(static_cast<quint8>(field) >= 10 && static_cast<quint8>(field) <= 30) return game::SNAKE_BODY_COLLISION;
    else
        switch (field) {
        case board::FOOD_LVL_1: {
            return game::FOOD_LVL_1_COLLISION;
            break;
        }
        case board::FOOD_LVL_2: {
            return game::FOOD_LVL_2_COLLISION;
            break;
        }
        case board::COIN_LVL_1: {
            return game::COIN_LVL_1_COLLISION;
            break;
        }
        case board::COIN_LVL_2: {
            return game::COIN_LVL_2_COLLISION;
            break;
        }
        case board::COIN_LVL_3: {
            return game::COIN_LVL_3_COLLISION;
            break;
        }
        default:
            return game::UNDEFINED_COLLISION;
        }
}

void c_game::stopGame(QObject * loserPlayer)
{
//    QPair<bool,bool> pair;
//    if(loserPlayer == players.first)
//        pair.first = true;
//    if(loserPlayer == players.second)
//        pair.second = true;

    emit stopGameSignal();
//    emit notificationMessage(pair, "LOSS !!!");
}

c_eventController *c_game::getEventCtrlr() const
{
    return eventCtrlr;
}

void c_game::addNewBlock(QPair<bool, bool> player)
{
    if(player.first)
        players.first->getBoard()->newBlock( players.first->getSnake()->getSnake().first() );
    if(player.second)
        players.second->getBoard()->newBlock( players.second->getSnake()->getSnake().first() );
}

void c_game::addNewFood(QPair<bool, bool> player)
{
    if(player.first)
        players.first->getBoard()->newFood( players.first->getSnake()->getSnake().first() );
    if(player.second)
        players.second->getBoard()->newFood( players.second->getSnake()->getSnake().first() );
}

void c_game::addNewCoin(QPair<bool, bool> player)
{
    if(player.first)
        players.first->getBoard()->newCoin( players.first->getSnake()->getSnake().first() );
    if(player.second)
        players.second->getBoard()->newCoin( players.second->getSnake()->getSnake().first() );
}

void c_game::movePlayer(QPair<bool, bool> player)
{
    if(player.first)
        players.first->move();
    if(player.second)
        players.second->move();

}

void c_game::speedUpPlayer(QPair<bool, bool> player)
{
    if(speedLevel < game::maxSpeedUpLevel)
        ++speedLevel;

    int nextSpeed = game::beginningMoveTime - (game::speedUpValue * speedLevel );

    eventCtrlr->setNextMoveTime( nextSpeed );

    if(player.first)
        players.first->speedUp(speedLevel);
    if(player.second)
        players.second->speedUp(speedLevel);

    emit speedUpSignal(player, speedLevel);
}

void c_game::testPotentialCollision(QList<QPoint> &snake, const board::boardArray &board)
{
    game::ColisionResult result = testCollision(snake, board);

    switch (result) {
    case game::NO_COLLISION: {
        return;
    }
    case game::BLOCK_COLLISION: {
        stopGame(sender());
        return;
    }
    case game::SNAKE_BODY_COLLISION: {
        stopGame(sender());
        return;
    }
    case game::FOOD_LVL_1_COLLISION: {
        dynamic_cast<c_player *>(sender())->getBoard()->clearField(snake.first());
        dynamic_cast<c_player *>(sender())->feedSnake( game::feedLvl1 );
        return;
    }
    case game::FOOD_LVL_2_COLLISION: {
        dynamic_cast<c_player *>(sender())->getBoard()->clearField(snake.first());
        dynamic_cast<c_player *>(sender())->feedSnake( game::feedLvl2 );
        return;
    }
    case game::COIN_LVL_1_COLLISION: {
        dynamic_cast<c_player *>(sender())->getBoard()->clearField(snake.first());
        dynamic_cast<c_player *>(sender())->coinsToWallet( game::prizeLvl1 );
        return;
    }
    case game::COIN_LVL_2_COLLISION: {
        dynamic_cast<c_player *>(sender())->getBoard()->clearField(snake.first());
        dynamic_cast<c_player *>(sender())->coinsToWallet( game::prizeLvl2 );
        return;
    }
    case game::COIN_LVL_3_COLLISION: {
        dynamic_cast<c_player *>(sender())->getBoard()->clearField(snake.first());
        dynamic_cast<c_player *>(sender())->coinsToWallet( game::prizeLvl3 );
        return;
    }
    case game::UNDEFINED_COLLISION: {
        return;
    }
    default: {
        return;
    }
    }
}
