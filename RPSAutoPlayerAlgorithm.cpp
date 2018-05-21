

#include "RPSAutoPlayerAlgorithm.h"


RPSAutoPlayerAlgorithm::RPSAutoPlayerAlgorithm(int player) : RPSPlayerAlgorithm(player) {
    RPSPoint pointToAdd;
    for (int j = 0; j < N; j++) {
        pointToAdd.setY(j);
        for (int i = 0; i < M; i++) {
            pointToAdd.setX(i);
            emptyPositions.push_back(pointToAdd);
        }
    }
}


void RPSAutoPlayerAlgorithm::notifyOnInitialBoard(const Board &b, const std::vector<unique_ptr<FightInfo>> &fights) {

    RPSPoint curPoint;

    for (int j = 0; j < N; j++) {
        curPoint.setY(j);
        for (int i = 0; i < M; i++) {

            curPoint.setX(i);

            if (b.getPlayer(curPoint) != 0 && b.getPlayer(curPoint) != _player) {
                // if it's not an empty point and not occupied by one of the players tools so
                // it has an opponent tool
                eraseFromVector(emptyPositions,curPoint);
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
        } else if (fight->getWinner() != _player) { //opponent won this fight
            opponentTools[curPoint] = fight->getPiece(fight->getWinner());
            myTools.erase(curPoint);
        }
    }
}


void RPSAutoPlayerAlgorithm::notifyOnOpponentMove(const Move &move) {

    RPSPoint fromPoint(move.getFrom().getX(), move.getFrom().getY());
    RPSPoint toPoint(move.getTo().getX(), move.getTo().getY());
    auto it=opponentTools.find(fromPoint);
    char movedTool = it->second;

    opponentTools.erase(fromPoint);
    opponentTools[toPoint] = movedTool;

    emptyPositions.push_back(fromPoint);
    eraseFromVector(emptyPositions,toPoint);
}


void RPSAutoPlayerAlgorithm::notifyFightResult(const FightInfo &fightInfo) {

    RPSPoint curPoint(fightInfo.getPosition().getX(), fightInfo.getPosition().getY());
    if(curPoint.getX()==-1) //if there was no fight in the last move, there's nothing to update
        return;

    if (fightInfo.getWinner() == 0) {//tie
        myTools.erase(curPoint);
        opponentTools.erase(curPoint);
        emptyPositions.push_back(curPoint);
    } else if (fightInfo.getWinner() != _player) { //opponent won this fight
        opponentTools[curPoint] = fightInfo.getPiece(fightInfo.getWinner());
        myTools.erase(curPoint);
    } else
        eraseFromMap(opponentTools,curPoint);
}


unique_ptr<JokerChange> RPSAutoPlayerAlgorithm::getJokerChange() {

    if (playerToolCounters['J'] == J) //all the players jokers been eaten
        return nullptr;

    for (auto &&pair : opponentTools) {

        if (pair.second == 'R') { //if one of the known opponents tools is 'R'

            if (playerToolCounters['P'] == P) { //player doesn't have 'P' tool on board
                if (checkIfHasThisJokerRep('P').getX() != -1) //already has a joker that is 'P'
                    return nullptr;
                return make_unique<RPSJokerChange>(*(playerJokers.begin()->get()), 'P');
                // change one of the jokers to 'P'
            }
        } else if (pair.second == 'P') {

            if (playerToolCounters['S'] == S) {
                if (checkIfHasThisJokerRep('S').getX() != -1)
                    return nullptr;
                return make_unique<RPSJokerChange>(*(playerJokers.begin()->get()), 'S');
            }
        } else if (pair.second == 'S') {

            if (playerToolCounters['R'] == R) {
                if (checkIfHasThisJokerRep('R').getX() != -1)
                    return nullptr;
                return make_unique<RPSJokerChange>(*(playerJokers.begin()->get()), 'R');
            }
        }
    }
    return nullptr;
}


RPSPoint RPSAutoPlayerAlgorithm::checkIfHasThisJokerRep(char c)const {

    RPSPoint curPoint;

    for (auto &point : playerJokers) {
        // go over the positions of the jokers
        curPoint.setX(point->getX());
        curPoint.setY(point->getY());

        if (curPoint.getX() != -2){ //if this joker still exists
            auto it = myTools.find(curPoint);
            if (it!=myTools.end()) {
                if (it->second == c)
                    return curPoint;
            }
        }
    }

    return RPSPoint(-1, -1);
}


unique_ptr<Move> RPSAutoPlayerAlgorithm::getMove() {

    unique_ptr<Move> resMove;
    RPSPoint from;
    RPSPoint to;
    bool found=false;

    for (auto pair : opponentTools) {
        to=pair.first; //the point of opponents tool
        if (pair.second == 'R') { //we know that opponent has 'R' tool somewhere
            if (playerToolCounters['P'] < P) { //if we have 'P' tool
                from = findKeyOfValueInMyTools('P'); //get the position of our 'P'
                resMove = make_unique<RPSMove>(from, to, 'P', _player);
                found=true;
                break;
            }
            from = checkIfHasThisJokerRep('P'); //if we don't have 'P' lets check if we have a joker 'P'
            if (from.getX() != -1) { //if we do have this kind of joker

                resMove= make_unique<RPSMove>(from, to, 'P', _player);
                found=true;
                break;
            }

        } else if (pair.second == 'P') {
            if (playerToolCounters['S'] < S) {
                from = findKeyOfValueInMyTools('S');
                resMove= make_unique<RPSMove>(from, to, 'S', _player);
                found=true;
                break;
            }
            from = checkIfHasThisJokerRep('S');
            if (from.getX() != -1) {
                resMove= make_unique<RPSMove>(from, to, 'S', _player);
                found=true;
                break;
            }
        } else if (pair.second == 'S') {
            if (playerToolCounters['R'] < R) {
                from = findKeyOfValueInMyTools('R');
                resMove= make_unique<RPSMove>(from, to, 'R', _player);
                found=true;
                break;
            }
            from = checkIfHasThisJokerRep('R');
            if (from.getX() != -1) {
                resMove= make_unique<RPSMove>(from, to, 'R', _player);
                found=true;
                break;
            }
        }
    }
    if(!found) { //if we didn't find a tool of ours to win one of opponents tools

        while (true) {
            from = getRandomPoint(myTools);
            if (myTools.at(from) != 'F' && myTools.at(from) != 'B') //don't want to move unmoving tool
                break;
        }
        char piece=myTools[from];
        to = getRandomPoint(opponentTools);
        resMove= make_unique<RPSMove>(from, to, piece, _player);
    }

    //update vector and map
    myTools[to]=myTools[from];
    myTools.erase(from);

    emptyPositions.push_back(from);
    return resMove;
}


RPSPoint RPSAutoPlayerAlgorithm::findKeyOfValueInMyTools(char value) {

    for (auto pair : myTools) {
        if (pair.second == value)
            return RPSPoint(pair.first.getX(), pair.first.getY());
    }

    return RPSPoint(-1, -1);
}


void RPSAutoPlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill) {
    if(player!=1 && player!=2 )
        return;
    RPSPoint pushedPoint;
    for(int i=0;i<R;i++){
        pushedPoint=getRandomPoint(emptyPositions);
        vectorToFill.push_back(make_unique<RPSPiecePosition>(pushedPoint,'R'));
        eraseFromVector(emptyPositions,pushedPoint);
        myTools[pushedPoint]='R';
    }
    for(int i=0;i<P;i++){
        pushedPoint=getRandomPoint(emptyPositions);
        vectorToFill.push_back(make_unique<RPSPiecePosition>(pushedPoint,'P'));
        eraseFromVector(emptyPositions,pushedPoint);
        myTools[pushedPoint]='P';
    }
    for(int i=0;i<S;i++){
        pushedPoint=getRandomPoint(emptyPositions);
        vectorToFill.push_back(make_unique<RPSPiecePosition>(pushedPoint,'S'));
        eraseFromVector(emptyPositions,pushedPoint);
        myTools[pushedPoint]='S';
    }
    for(int i=0;i<B;i++){
        pushedPoint=getRandomPoint(emptyPositions);
        vectorToFill.push_back(make_unique<RPSPiecePosition>(pushedPoint,'B'));
        eraseFromVector(emptyPositions,pushedPoint);
        myTools[pushedPoint]='B';
    }
    for(int i=0;i<F;i++){
        pushedPoint=getRandomPoint(emptyPositions);
        vectorToFill.push_back(make_unique<RPSPiecePosition>(pushedPoint,'F'));
        eraseFromVector(emptyPositions,pushedPoint);
        myTools[pushedPoint]='F';
    }
    for(int i=0;i<J;i++){
        pushedPoint=getRandomPoint(emptyPositions);
        vectorToFill.push_back(make_unique<RPSPiecePosition>(pushedPoint,'J','R'));
        eraseFromVector(emptyPositions,pushedPoint);
        myTools[pushedPoint]='R';
    }
}


void RPSAutoPlayerAlgorithm::eraseFromMap(map<RPSPoint, char> &m, const RPSPoint &p) {
    for(auto it=m.begin();it!=m.end();++it){
        if(it->first==p) {
            m.erase(it);
            break;
        }
    }

}


void RPSAutoPlayerAlgorithm::eraseFromVector(vector<RPSPoint> &v, RPSPoint p) {

    for(auto it=v.begin();it!=v.end();++it){
        if(*it==p) {
            v.erase(it);
            return;
        }
    }

}


RPSPoint RPSAutoPlayerAlgorithm::getRandomPoint(vector<RPSPoint> v)const {

    auto it = v.begin();
    std::advance(it, rand() % v.size());
    return RPSPoint(it->getX(), it->getY());
}


RPSPoint RPSAutoPlayerAlgorithm::getRandomPoint(map<RPSPoint, char> m)const {
    if(m.empty())
        return RPSPoint(-1,-1);
    auto it = m.begin();
    std::advance(it, rand() % m.size());
    return RPSPoint(it->first.getX(), it->first.getY());
}





