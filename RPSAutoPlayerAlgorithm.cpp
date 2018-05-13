

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
                eraseFromVector(emptyPositions,curPoint);
                opponentTools[curPoint] = 'O';
            }
        }
    }

    for (auto &&fight : fights) {

        curPoint.setX(fight->getPosition().getX());
        curPoint.setY(fight->getPosition().getY());

        if (fight->getWinner() == 0) {//tie
            eraseFromMap(myTools,curPoint);
            emptyPositions.push_back(curPoint);
        } else if (fight->getWinner() != _player) { //opponent won this fight
            opponentTools[curPoint] = fight->getPiece(fight->getWinner());
            eraseFromMap(myTools,curPoint);
        }
    }
}

void RPSAutoPlayerAlgorithm::notifyOnOpponentMove(const Move &move) {

    RPSPoint fromPoint(move.getFrom().getX(), move.getFrom().getY());
    RPSPoint toPoint(move.getTo().getX(), move.getTo().getY());
    auto it=opponentTools.find(fromPoint);
    char movedTool = it->second;

    eraseFromMap(opponentTools,fromPoint);
    opponentTools[toPoint] = movedTool;

    emptyPositions.push_back(fromPoint);
    eraseFromVector(emptyPositions,toPoint);

}


void RPSAutoPlayerAlgorithm::notifyFightResult(const FightInfo &fightInfo) {

    RPSPoint curPoint(fightInfo.getPosition().getX(), fightInfo.getPosition().getY());
    if(curPoint.getX()==-1)
        return;


    if (fightInfo.getWinner() == 0) {//tie
        eraseFromMap(myTools,curPoint);
        eraseFromMap(opponentTools,curPoint);
        emptyPositions.push_back(curPoint);
    } else if (fightInfo.getWinner() != _player) { //opponent won this fight
        opponentTools[curPoint] = fightInfo.getPiece(fightInfo.getWinner());
        eraseFromMap(myTools,curPoint);
    } else
        eraseFromMap(opponentTools,curPoint);

}


unique_ptr<JokerChange> RPSAutoPlayerAlgorithm::getJokerChange() {

    if (playerJokers.empty())
        return nullptr;

    for (auto &&pair : opponentTools) {

        if (pair.second == 'R') {

            if (playerToolCounters['P'] == P) { //player doesn't have 'P' tool on board
                if (checkIfHasThisJokerRep('P').getX() != -1)
                    return nullptr;
                return make_unique<RPSJokerChange>(*(playerJokers.begin()->get()), 'P');
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

    for (auto &&point : playerJokers) {

        curPoint.setX(point->getX());
        curPoint.setY(point->getY());

        if (myTools.at(curPoint) == c)
            return curPoint;

    }

    return RPSPoint(-1, -1);
}


unique_ptr<Move> RPSAutoPlayerAlgorithm::getMove() {

    unique_ptr<Move> resMove;
    RPSPoint from;
    RPSPoint to;
    bool found=false;

    for (auto pair : opponentTools) {
        to=pair.first;
        if (pair.second == 'R') {
            if (playerToolCounters['P'] < P) {

                resMove = make_unique<RPSMove>(findKeyOfValueInMyTools('P'), to, 'P', _player);
                found=true;
                break;
            }
            from = checkIfHasThisJokerRep('P');
            if (from.getX() != -1) {

                resMove= make_unique<RPSMove>(from, to, 'P', _player);
                found=true;
                break;
            }

        } else if (pair.second == 'P') {
            if (playerToolCounters['S'] < S) {
                resMove= make_unique<RPSMove>(findKeyOfValueInMyTools('S'), to, 'S', _player);
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
                resMove= make_unique<RPSMove>(findKeyOfValueInMyTools('R'), to, 'R', _player);
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
    if(!found) {

        while (true) {
            from = getRandomPoint(myTools);
            if (myTools.at(from) != 'F' && myTools.at(from) != 'B')
                break;
        }
        char piece=myTools[from];
        to = getRandomPoint(opponentTools);
        resMove= make_unique<RPSMove>(from, to, piece, _player);
    }
//    if(auto&& it=find(playerJokers.begin(),playerJokers.end(),make_unique<RPSPoint>(from))!=playerJokers.end())
//    {
//        eraseFromVector(playerJokers,make_unique<RPSPoint>(from));
//        playerJokers.push_back(make_unique<RPSPoint>(to));
//    }
    myTools[to]=myTools[from];
    if(myTools.erase(from)==1)
        cout<<"Success"<<endl;
    emptyPositions.push_back(from);
    //eraseFromVector(emptyPositions,to);
    return resMove;

}


RPSPoint RPSAutoPlayerAlgorithm::findKeyOfValueInMyTools(char value) {

    for (auto &&pair : myTools) {
        if (pair.second == value)
            return pair.first;
    }

    return RPSPoint(-1, -1);
}

void RPSAutoPlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill) {
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
template<typename T>
void RPSAutoPlayerAlgorithm::eraseFromVector(vector<T> &v, const T &p) {

    for(auto it=v.begin();it!=v.end();++it){
        if(*it==p) {
            v.erase(it);
            return;
        }
    }

}

RPSPoint RPSAutoPlayerAlgorithm::getRandomPoint(vector<RPSPoint> v)const {
     random_shuffle(v.begin(),v.end());
     return *(v.begin().base());

}

RPSPoint RPSAutoPlayerAlgorithm::getRandomPoint(map<RPSPoint, char> m)const {
    if(m.empty())
        return RPSPoint(-1,-1);
    auto it = m.begin();
    std::advance(it, rand() % m.size());
    return it->first;

}





