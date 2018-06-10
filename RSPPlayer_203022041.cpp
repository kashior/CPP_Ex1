

#include "RSPPlayer_203022041.h"
#include "AlgorithmRegistration.h"
#include <ctime>


REGISTER_ALGORITHM(203022041)


RSPPlayer_203022041::RSPPlayer_203022041(int player) : _player(player) {
    srand(time(NULL));
    RPSPoint pointToAdd;
    noFight = true;
    for (int j = 0; j < N; j++) {
        pointToAdd.setY(j);
        for (int i = 0; i < M; i++) {
            pointToAdd.setX(i);
            emptyPositions.push_back(pointToAdd);
        }
    }
}

void RSPPlayer_203022041::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill) {
    if (player != 1 && player != 2)
        return;
    RPSPoint pushedPoint;
    for (int i = 0; i < R; i++) {
        pushedPoint = getRandomPoint(emptyPositions);
        vectorToFill.push_back(make_unique<RPSPiecePosition>(pushedPoint, 'R'));
        eraseFromVector(emptyPositions, pushedPoint);
        myTools[pushedPoint] = 'R';
    }
    for (int i = 0; i < P; i++) {
        pushedPoint = getRandomPoint(emptyPositions);
        vectorToFill.push_back(make_unique<RPSPiecePosition>(pushedPoint, 'P'));
        eraseFromVector(emptyPositions, pushedPoint);
        myTools[pushedPoint] = 'P';
    }
    for (int i = 0; i < S; i++) {
        pushedPoint = getRandomPoint(emptyPositions);
        vectorToFill.push_back(make_unique<RPSPiecePosition>(pushedPoint, 'S'));
        eraseFromVector(emptyPositions, pushedPoint);
        myTools[pushedPoint] = 'S';
    }
    for (int i = 0; i < B; i++) {
        pushedPoint = getRandomPoint(emptyPositions);
        vectorToFill.push_back(make_unique<RPSPiecePosition>(pushedPoint, 'B'));
        eraseFromVector(emptyPositions, pushedPoint);
        myTools[pushedPoint] = 'B';
    }
    for (int i = 0; i < F; i++) {
        pushedPoint = getRandomPoint(emptyPositions);
        vectorToFill.push_back(make_unique<RPSPiecePosition>(pushedPoint, 'F'));
        eraseFromVector(emptyPositions, pushedPoint);
        myTools[pushedPoint] = 'F';
    }
    for (int i = 0; i < J; i++) {
        pushedPoint = getRandomPoint(emptyPositions);
        vectorToFill.push_back(make_unique<RPSPiecePosition>(pushedPoint, 'J', 'R'));
        eraseFromVector(emptyPositions, pushedPoint);
        myTools[pushedPoint] = 'R';
        playerJokers.push_back(make_unique<RPSPoint>(pushedPoint));
    }
}


void RSPPlayer_203022041::notifyOnInitialBoard(const Board &b, const std::vector<unique_ptr<FightInfo>> &fights) {

    RPSPoint curPoint;

    for (int j = 0; j < N; j++) {
        curPoint.setY(j);
        for (int i = 0; i < M; i++) {

            curPoint.setX(i);

            if (b.getPlayer(curPoint) != 0 && b.getPlayer(curPoint) != _player) {
                // if it's not an empty point and not occupied by one of the player tools so
                // it has an opponent tool
                eraseFromVector(emptyPositions, curPoint);
                opponentTools[curPoint] = 'O';
            }
        }
    }

    for (auto &fight : fights) {

        curPoint.setX(fight->getPosition().getX());
        curPoint.setY(fight->getPosition().getY());

        if (fight->getWinner() == 0) {//tie
            if (checkIfPlayerJokersHasPoint(curPoint))
                eraseFromJokers(curPoint,playerJokers);
            myTools.erase(curPoint);
            emptyPositions.push_back(curPoint);
        } else if (fight->getWinner() != _player) { //opponent won this fight
            opponentTools[curPoint] = (char)toupper(fight->getPiece(fight->getWinner()));
            if (checkIfPlayerJokersHasPoint(curPoint))
                eraseFromJokers(curPoint,playerJokers);
            myTools.erase(curPoint);
        }
    }
}


unique_ptr<Move> RSPPlayer_203022041::getMove() {

    unique_ptr<Move> resMove;
    RPSPoint from;
    RPSPoint to;
    bool found = false;

    for (auto pair : opponentTools) {
        to = pair.first; //the point of opponents tool
        if (pair.second == 'R') { //we know that opponent has 'R' tool somewhere
            from = findKeyOfValueInMyTools('P');//get the position of our 'P'
            if (from.getX() != -1) {
                resMove = make_unique<RPSMove>(from, to, 'P', _player);
                found = true;
                break;
            }
            from = checkIfHasThisJokerRep('P'); //if we don't have 'P' lets check if we have a joker 'P'
            if (from.getX() != -1) { //if we do have this kind of joker

                resMove = make_unique<RPSMove>(from, to, 'P', _player);
                found = true;
                break;
            }

        }
        if (pair.second == 'P') {
            from = findKeyOfValueInMyTools('S');
            if (from.getX() != -1) {
                resMove = make_unique<RPSMove>(from, to, 'S', _player);
                found = true;
                break;
            }
            from = checkIfHasThisJokerRep('S');
            if (from.getX() != -1) {
                resMove = make_unique<RPSMove>(from, to, 'S', _player);
                found = true;
                break;
            }
        }
        if (pair.second == 'S') {
            from = findKeyOfValueInMyTools('R');
            if (from.getX() != -1) {
                resMove = make_unique<RPSMove>(from, to, 'R', _player);
                found = true;
                break;
            }
            from = checkIfHasThisJokerRep('R');
            if (from.getX() != -1) {
                resMove = make_unique<RPSMove>(from, to, 'R', _player);
                found = true;
                break;
            }
        }
    }
    if (!found) { //if we didn't find a tool of ours to win one of opponents tools
        int i=M*N;

        while (--i>0) {
            from = getRandomPoint(myTools);
            if (myTools.at(from) != 'F' && myTools.at(from) != 'B') //don't want to move unmoving tool
                break;
        }
        char piece = myTools.at(from);
        to = getRandomPoint(opponentTools);
        resMove = make_unique<RPSMove>(from, to, piece, _player);
    }

    //update vector and map
    if (checkIfPlayerJokersHasPoint(from)) {
        eraseFromJokers(from,playerJokers);
        playerJokers.push_back(move(make_unique<RPSPoint>(to)));
    }
    myTools[to] = myTools[from];
    myTools.erase(from);
    emptyPositions.push_back(from);

    return resMove;
}


void RSPPlayer_203022041::notifyFightResult(const FightInfo &fightInfo) {

    RPSPoint curPoint(fightInfo.getPosition().getX(), fightInfo.getPosition().getY());
    if (curPoint.getX() == -1) { //if there was no fight in the last move, there's nothing to update
        noFight = true;
        return;
    }

    if (fightInfo.getWinner() == 0) {//tie
        if (checkIfPlayerJokersHasPoint(curPoint))
            eraseFromJokers(curPoint,playerJokers);
        myTools.erase(curPoint);
        opponentTools.erase(curPoint);
        emptyPositions.push_back(curPoint);
    } else if (fightInfo.getWinner() != _player) { //opponent won this fight
        opponentTools[curPoint] = (char)toupper(fightInfo.getPiece(fightInfo.getWinner()));
        if (checkIfPlayerJokersHasPoint(curPoint))
            eraseFromJokers(curPoint,playerJokers);
        myTools.erase(curPoint);
    } else
        opponentTools.erase(curPoint);

    noFight = false;

}

void RSPPlayer_203022041::notifyOnOpponentMove(const Move &move) {
    if (!noFight)
        return;

    RPSPoint fromPoint(move.getFrom().getX(), move.getFrom().getY());
    RPSPoint toPoint(move.getTo().getX(), move.getTo().getY());
    opponentTools[toPoint] = opponentTools[fromPoint];
    opponentTools.erase(fromPoint);
    emptyPositions.push_back(fromPoint);
    eraseFromVector(emptyPositions, toPoint);

}


unique_ptr<JokerChange> RSPPlayer_203022041::getJokerChange() {

    if (playerJokers.empty()) //all the players jokers been eaten
        return nullptr;
    RPSPoint jokerPoint;

    for (auto &pair : opponentTools) {

        if (pair.second == 'R') { //if one of the known opponents tools is 'R'
            if ((jokerPoint = checkIfHasThisJokerRep('P')).getX() == -1) {
                jokerPoint.setX(playerJokers[0]->getX());
                jokerPoint.setY(playerJokers[0]->getY());
                return make_unique<RPSJokerChange>(jokerPoint, 'P');
            }
            continue;
        }
        if (pair.second == 'P') {
            if ((jokerPoint = checkIfHasThisJokerRep('S')).getX() == -1) {
                jokerPoint.setX(playerJokers[0]->getX());
                jokerPoint.setY(playerJokers[0]->getY());
                return make_unique<RPSJokerChange>(jokerPoint, 'S');
            }
            continue;
        }
        if (pair.second == 'S') {
            if ((jokerPoint = checkIfHasThisJokerRep('R')).getX() == -1) {
                jokerPoint.setX(playerJokers[0]->getX());
                jokerPoint.setY(playerJokers[0]->getY());
                return make_unique<RPSJokerChange>(jokerPoint, 'R');
            }
            continue;
        }
    }

    return nullptr;
}


RPSPoint RSPPlayer_203022041::checkIfHasThisJokerRep(char c) {
    RPSPoint curPoint;
    for (auto &point : playerJokers) {
        // go over the positions of the jokers
        curPoint.setX(point->getX());
        curPoint.setY(point->getY());
        if (c == myTools.at(curPoint))
            return curPoint;
    }
    return RPSPoint(-1, -1);
}


RPSPoint RSPPlayer_203022041::findKeyOfValueInMyTools(char value) {

    for (auto pair : myTools) {
        if (pair.second == value)
            return RPSPoint(pair.first.getX(), pair.first.getY());
    }

    return RPSPoint(-1, -1);
}


template<class T>
void RSPPlayer_203022041::eraseFromVector(vector<T> &v, T p) {

    for (auto it = v.begin(); it != v.end(); ++it) {
        if (*it == p) {
            v.erase(it);
            return;
        }
    }

}


RPSPoint RSPPlayer_203022041::getRandomPoint(vector<RPSPoint> v) const {

    auto it = v.begin();
    std::advance(it, rand() % v.size());
    return RPSPoint(it->getX(), it->getY());
}


RPSPoint RSPPlayer_203022041::getRandomPoint(map<RPSPoint, char> m) const {
    if (m.empty())
        return RPSPoint(-1, -1);
    auto it = m.begin();
    std::advance(it, rand() % m.size());
    return RPSPoint(it->first.getX(), it->first.getY());
}

int RSPPlayer_203022041::getPlayer() { return _player; }

bool RSPPlayer_203022041::checkIfPlayerJokersHasPoint(const RPSPoint &p) {
    for(auto& point:playerJokers){
        if(p.getX()==point->getX() && p.getY()==point->getY())
            return true;
    }
    return false;
}

void RSPPlayer_203022041::eraseFromJokers(RPSPoint p, vector<unique_ptr<RPSPoint>> &v) {
    for (auto it = v.begin(); it != v.end(); ++it) {
        if ((*it)->getX()== p.getX() && (*it)->getY()==p.getY()) {
            playerJokers.erase(it);
            return;
        }
    }

}

//void RSPPlayer_203022041::eraseFromJokers(unique_ptr<RPSPoint> p, vector<unique_ptr<RPSPoint>> v) {
//    for (auto it = v.begin(); it != v.end(); ++it) {
//        if ((*it)->getX() == p->getX() && (*it)->getY()==p->getY()) {
//            playerJokers.erase(it);
//            return;
//        }
//    }
//}







