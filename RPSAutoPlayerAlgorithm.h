

#ifndef EX1_RPSAUTOPLAYERALGORITHM_H
#define EX1_RPSAUTOPLAYERALGORITHM_H


#include "RPSPlayerAlgorithm.h"

class RPSAutoPlayerAlgorithm : public RPSPlayerAlgorithm{

    vector<RPSPoint> emptyPositions;
    map<RPSPoint, char> myTools;
    map<RPSPoint, char> opponentTools;

public:

   explicit  RPSAutoPlayerAlgorithm(int player);

    virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill);
    virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights);
    virtual void notifyOnOpponentMove(const Move& move); // called only on opponents move
    virtual void notifyFightResult(const FightInfo& fightInfo); // called only if there was a fight
    virtual unique_ptr<Move> getMove();

    RPSPoint findKeyOfValueInMyTools(char value);

    virtual unique_ptr<JokerChange> getJokerChange(); // nullptr if no change is requested

    RPSPoint checkIfHasThisJokerRep(char c);

    void eraseFromMap(map<RPSPoint, char> &m, const RPSPoint &p);
    void eraseFromVector(vector<RPSPoint> &v, RPSPoint p);
    RPSPoint getRandomPoint(vector<RPSPoint> v);
    RPSPoint getRandomPoint(map<RPSPoint,char> v);

    virtual ~RPSAutoPlayerAlgorithm(){}

};




#endif //EX1_RPSAUTOPLAYERALGORITHM_H
