

#ifndef EX1_RPSPLAYERALGORITHM_H
#define EX1_RPSPLAYERALGORITHM_H


#include "PlayerAlgorithm.h"
#include "RPSPiecePosition.h"
#include <map>
#include <algorithm>
#include "RPSParser.h"

using namespace std;

/**
 * a class that inherits from PlayerAlgorithm abstract class. Implementing this class but keeping some methods abstract
 * Because we have 2 kinds of players. this class uses as a base class for these 2 kinds.
 *  contains player number, the line number we are at during reading or during evaluating a move from auto alg.
 *  a vector of player jokers and a map of counters for each tool type on board
 */
class RPSPlayerAlgorithm : public PlayerAlgorithm {

public:

    int _player;
    int lineNum=0;
    vector<unique_ptr<RPSPoint>> playerJokers;
    map<char, int> playerToolCounters;

    //<constructors>
    RPSPlayerAlgorithm(int player);
    //</constructors>

    //friends decleration
    friend class RPSManager;
    friend class RPSGame;

    // abstract methods that we  use only at the classes that inherits from this class
    virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill) = 0;

    virtual void notifyOnInitialBoard(const Board &b, const std::vector<unique_ptr<FightInfo>> &fights)=0;

    virtual void notifyOnOpponentMove(const Move &move) = 0; // called only on opponents move

    virtual void notifyFightResult(const FightInfo &fightInfo) = 0; // called only if there was a fight

    virtual unique_ptr<Move> getMove() = 0;

    virtual unique_ptr<JokerChange> getJokerChange() = 0; // nullptr if no change is requested


    /**
     * This function add a specific unique pointer of piece position to the player jokers vector
     * @param pos - the position we want to add
     */
    void addJokerPositionToVector(unique_ptr<PiecePosition> &pos);
    /**
     *
     * @return player number
     */
    int getPlayer();
    /**
     * method that we don't use right now.. it suppose to update a specific place at the tool counter map of this player
     * @param tool
     */
    void updateToolCounter(char tool);

    //distructor
    virtual ~RPSPlayerAlgorithm() {}
};


#endif //EX1_RPSPLAYERALGORITHM_H
