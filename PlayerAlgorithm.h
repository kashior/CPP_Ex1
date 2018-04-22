//
// Created by Or Kashi on 22/04/2018.
//
#include <vector>
#include <memory>
#include "PiecePosition.h"
#include "Board.h"

#ifndef EX1_PLAYERALGORITHM_H
#define EX1_PLAYERALGORITHM_H

using namespace std;

class PlayerAlgorithm {
public:
    virtual void getInitialPositions(int player, vector<unique_ptr<PiecePosition>> & vectorToFill )=0;
    virtual void notifyOnInitialBoard(const Board& b, const vector<unique_ptr<FightInfo>> & fights)=0;
    virtual void notifyOnOpponentMove(const Move & move)=0;
    virtual void notifyFightResult(const FightInfo & fightInfo)=0;
    virtual unique_ptr<Move> getMove()=0;
    virtual unique_ptr<JokerChange> getJokerChange()=0;
};


#endif //EX1_PLAYERALGORITHM_H
