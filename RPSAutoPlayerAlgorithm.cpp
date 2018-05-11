

#include "RPSAutoPlayerAlgorithm.h"

RPSAutoPlayerAlgorithm::RPSAutoPlayerAlgorithm(int player) : RPSPlayerAlgorithm(player) {}

void RPSAutoPlayerAlgorithm::notifyOnInitialBoard(const Board &b, const std::vector<unique_ptr<FightInfo>> &fights) {

    RPSPoint curPoint;

    for (int j = 0; j < N; j++) {
        curPoint.setY(j);
        for (int i = 0; i < M; i++) {

            curPoint.setX(i);

            if (b.getPlayer(curPoint) != 0 && b.getPlayer(curPoint) != _player) {
                emptyPositions.erase(remove(emptyPositions.begin(), emptyPositions.end(),curPoint),emptyPositions.end());
                opponentTools[curPoint] = 'O';
            }
        }
    }

    for (auto &&fight : fights) {

        curPoint.setX(fight->getPosition().getX());
        curPoint.setY(fight->getPosition().getY());

        if (fight->getWinner() == 0) {//tie
            myTools.erase(curPoint);
            emptyPositions.push_back(curPoint);
        }
        else if (fight->getWinner() != _player) { //opponent won this fight
            opponentTools[curPoint] = fight->getPiece(fight->getWinner());
            myTools.erase(curPoint);
        }
    }
}

void RPSAutoPlayerAlgorithm::notifyOnOpponentMove(const Move &move) {

    RPSPoint fromPoint(move.getFrom().getX(), move.getFrom().getY());
    RPSPoint toPoint(move.getTo().getX(), move.getTo().getY());
    char movedTool = opponentTools[fromPoint];

    opponentTools.erase(fromPoint);
    opponentTools[toPoint] = movedTool;

    emptyPositions.push_back(fromPoint);
    emptyPositions.erase(remove(emptyPositions.begin(), emptyPositions.end(),toPoint),emptyPositions.end());

}


void RPSAutoPlayerAlgorithm::notifyFightResult(const FightInfo &fightInfo) {

    RPSPoint curPoint(fightInfo.getPosition().getX(), fightInfo.getPosition().getY());


    if (fightInfo.getWinner() == 0) {//tie
        myTools.erase(curPoint);
        opponentTools.erase(curPoint);
        emptyPositions.push_back(curPoint);
    }

    else if (fightInfo.getWinner() != _player) { //opponent won this fight
        opponentTools[curPoint] = fightInfo.getPiece(fightInfo.getWinner());
        myTools.erase(curPoint);
    }

    else
        opponentTools.erase(curPoint);

}


unique_ptr<JokerChange> RPSAutoPlayerAlgorithm::getJokerChange() {

    if (playerJokers.size() == 0)
        return nullptr;

    for ( auto&& pair : opponentTools){

        if (pair.second == 'R'){

            if (playerToolCounters['P'] == P){ //player doesn't have 'P' tool on board
                if (checkIfHasThisJokerRep('P').getX() != -1)
                    return nullptr;
                return make_unique<RPSJokerChange>(*(playerJokers.begin()->get()), 'P');
            }
        }

        else if (pair.second == 'P'){

            if (playerToolCounters['S'] == S){
                if (checkIfHasThisJokerRep('S').getX() != -1)
                    return nullptr;
                return make_unique<RPSJokerChange>(*(playerJokers.begin()->get()), 'S');
            }
        }

        else if (pair.second == 'S'){

            if (playerToolCounters['R'] == R){
                if (checkIfHasThisJokerRep('R').getX() != -1)
                    return nullptr;
                return make_unique<RPSJokerChange>(*(playerJokers.begin()->get()), 'R');
            }
        }

    }

    return nullptr;
}


RPSPoint RPSAutoPlayerAlgorithm::checkIfHasThisJokerRep(char c) {

    RPSPoint curPoint;

    for (auto&& point : playerJokers){

        curPoint.setX(point->getX());
        curPoint.setY(point->getY());

        if (myTools[curPoint] == c)
            return curPoint;

    }

    return RPSPoint(-1,-1);
}


unique_ptr<Move> RPSAutoPlayerAlgorithm::getMove() {


    RPSPoint from;

    for (auto&& pair : opponentTools){

        if (pair.second == 'R'){
            if (playerToolCounters['P'] < P)
                return make_unique<RPSMove>(findKeyOfValueInmyTools('P'), pair.first, 'P', _player);
            from = checkIfHasThisJokerRep('P');
            if (from.getX() != -1){
                return make_unique<RPSMove>(from, pair.first, 'P', _player);
            }

        }

        else if (pair.second == 'P'){
            if (playerToolCounters['S'] < S)
                return make_unique<RPSMove>(findKeyOfValueInmyTools('S'), pair.first, 'S', _player);
            from = checkIfHasThisJokerRep('S');
            if (from.getX() != -1){
                return make_unique<RPSMove>(from, pair.first, 'S', _player);
            }
        }

        else if (pair.second == 'S'){
            if (playerToolCounters['R'] < R)
                return make_unique<RPSMove>(findKeyOfValueInmyTools('R'), pair.first, 'R', _player);
            from = checkIfHasThisJokerRep('R');
            if (from.getX() != -1){
                return make_unique<RPSMove>(from, pair.first, 'R', _player);
            }
        }
    }

    from = RPSPoint(-1,-1);//TODO random from my tool;
    RPSPoint to =RPSPoint(1,1) ;//TODO random from emptypositions;

    return make_unique<RPSMove>(from, to, myTools[from], _player);
}


RPSPoint RPSAutoPlayerAlgorithm::findKeyOfValueInmyTools(char value) {

    for (auto&& pair : myTools){
        if (pair.second == value)
            return pair.first;
    }

    return RPSPoint(-1,-1);
}

void RPSAutoPlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill) {

}



