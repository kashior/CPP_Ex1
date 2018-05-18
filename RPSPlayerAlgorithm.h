

#ifndef EX1_RPSPLAYERALGORITHM_H
#define EX1_RPSPLAYERALGORITHM_H


#include "PlayerAlgorithm.h"
#include "RPSPiecePosition.h"
#include <map>
#include <algorithm>
#include "RPSParser.h"

using namespace std;

class RPSPlayerAlgorithm : public PlayerAlgorithm {

public:

    int _player;
    int lineNum=0;
    vector<unique_ptr<RPSPoint>> playerJokers;
    map<char, int> playerToolCounters;


    RPSPlayerAlgorithm(int player);

    friend class RPSManager;
    friend class RPSGame;

    virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill) = 0;

    virtual void notifyOnInitialBoard(const Board &b, const std::vector<unique_ptr<FightInfo>> &fights)=0;

    virtual void notifyOnOpponentMove(const Move &move) = 0; // called only on opponents move

    virtual void notifyFightResult(const FightInfo &fightInfo) = 0; // called only if there was a fight

    virtual unique_ptr<Move> getMove() = 0;

    virtual unique_ptr<JokerChange> getJokerChange() = 0; // nullptr if no change is requested

    void addJokerPositionToVector(unique_ptr<PiecePosition> &pos);

    int getPlayer();

    void updateToolCounter(char tool);

    virtual ~RPSPlayerAlgorithm() {}
};


#endif //EX1_RPSPLAYERALGORITHM_H
