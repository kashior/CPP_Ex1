
#ifndef EX1_RPSFILEPLAYERALGORITHM_H
#define EX1_RPSFILEPLAYERALGORITHM_H


#include "RPSPlayerAlgorithm.h"
#include <string>
#include <memory>
using namespace std;

class RPSFilePlayerAlgorithm: public RPSPlayerAlgorithm {
    string _directory;
    vector<unique_ptr<RPSMove>> playerMoves;
    int moveFileLineCounter;

public:
    RPSFilePlayerAlgorithm(int player, string dir);
    virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) ;
    virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights);
    virtual void notifyOnOpponentMove(const Move& move); // called only on opponents move
    virtual void notifyFightResult(const FightInfo& fightInfo) ; // called only if there was a fight
    virtual unique_ptr<Move> getMove();
    virtual unique_ptr<JokerChange> getJokerChange() ; // nullptr if no change is requested
    void setMovesFromMoveFile();
    virtual ~RPSFilePlayerAlgorithm() {}

};


#endif //EX1_RPSFILEPLAYERALGORITHM_H
