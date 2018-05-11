

#ifndef EX1_RPSAUTOPLAYERALGORITHM_H
#define EX1_RPSAUTOPLAYERALGORITHM_H


#include <unordered_map>
#include "RPSPlayerAlgorithm.h"

#define MN M*N



class RPSAutoPlayerAlgorithm : public RPSPlayerAlgorithm{

    vector<RPSPoint> emptyPositions;
    unordered_map<RPSPoint, char> myTools;
    unordered_map<RPSPoint, char> opponentTools;

public:

    RPSAutoPlayerAlgorithm(int player);

    virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill);
    virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights);
    virtual void notifyOnOpponentMove(const Move& move); // called only on opponents move
    virtual void notifyFightResult(const FightInfo& fightInfo); // called only if there was a fight
    virtual unique_ptr<Move> getMove();

    RPSPoint findKeyOfValueInmyTools(char value);

    virtual unique_ptr<JokerChange> getJokerChange(); // nullptr if no change is requested

    RPSPoint checkIfHasThisJokerRep(char c);

    virtual ~RPSAutoPlayerAlgorithm(){}

};




#endif //EX1_RPSAUTOPLAYERALGORITHM_H
