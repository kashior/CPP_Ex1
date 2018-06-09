

#ifndef EX1_RPSAUTOPLAYERALGORITHM_H
#define EX1_RPSAUTOPLAYERALGORITHM_H

#include "gameDefinitions.h"
#include "PlayerAlgorithm.h"
#include "RPSBoard.h"
#include "RPSMove.h"
#include "RPSJokerChange.h"
#include "RPSFightInfo.h"
#include "RPSPiecePosition.h"

/*
 * class RSPPlayer_203022041
 *
 * This class represents an automatic player in the game.
 * This player will do moves in the game according to the information it has about the opponents pieces
 * on the board.
 * This information is stored in three different data structures:
 * 1. vector<RPSPoint> emptyPositions - has inside of it all the empty points on the board at the moment.
 * 2. map<RPSPoint, char> myTools - holds inside pairs of all the points on board that are occupied by the players
 *    tools and the kind of the tool (represented by char {'B', 'F', 'R', 'P', 'S'}).
 * 3. map<RPSPoint, char> opponentTools - holds inside pairs of all the points on board that are occupied by the opponents
 *    tools and the kind of the tool, if known (represented by char {'O' - for unknown kind of tool, 'R', 'P', 'S'}).
 */

class RSPPlayer_203022041 : public PlayerAlgorithm{

    vector<RPSPoint> emptyPositions;
    map<RPSPoint, char> myTools;
    map<RPSPoint, char> opponentTools;
    int _player;
    vector<unique_ptr<RPSPoint>> playerJokers;
    bool noFight;

public:


/**
 * C'tor. Initializing the vector "emptyPositions" - adds to it all the possible points on board.
 *
 * @param player - players number - 1 or 2
 *
 */
   explicit  RSPPlayer_203022041(int player);

/**
 * This function is called from the game manager.
 * It notifys the player about the state of the board after the first positioning of the tools by both players.
 * The auto player updates its data structures according to the changes on the board.
 *
 * @param b - the current board
 * @param fights - all the fights that acccured after initial positioning.
 *
 */
    virtual void notifyOnInitialBoard(const Board &b, const std::vector<unique_ptr<FightInfo>> &fights) ;

/**
 * Gives information to the auto player about the last move the opponent done.
 * The player updates afterwards the right vector or/and map/s according to the information it got.
 *
 * @param move - the last move made by the opponent.
 *
 */
    virtual void notifyOnOpponentMove(const Move& move); // called only on opponents move

/**
 * The auto player decides about its initial positioning.
 * The positioning is done randomly.
 * For each kind of tool, the auto player picks a random point from "emptyPositions" and puts the tool there.
 * This is done for each kind of tool, many times as the max possible number of pieces for this kind of the
 * tool.
 * The jokers are all set at the begining to be 'R'.
 *
 * @param player - 1 or 2
 * @param vectorToFill - for each tool position on the board it is also pushes this position to this vector.
 *
 */
    virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill);

/**
 * Informs the auto player about the last fight that accured in the game.
 * The auto player then updates its vector and/or map/s.
 *
 * @param fightInfo - the last fight accured.
 */
    virtual void notifyFightResult(const FightInfo& fightInfo); // called only if there was a fight

/**
 * The auto player decides the next move to make.
 * First we go over all the known tools of the opponent, and check if for a known opponent tool, we
 * have a tool that can eat it. If we don't have this tool, we will check if maybe we have a joker
 * that represents a tool that can eat the opponents tool. If we also don't have a fitting joker we will
 * continue checking for other known tools of the opponent.
 * If a the end we didn't find any fitting tool, the auto player will choose a random moving tool from
 * "myTools", and a random tool from "opponentsTools", move to this point its randomly chosen tool and
 * update the data structures.
 *
 * @return unique_ptr<Move> - the move that auto player wants to make.
 */
    virtual unique_ptr<Move> getMove();

/**
 * Searches by value if the map "myTools" contains the value "value".
 *
 * @param value - the value to search in "myTools"
 * @return RPSPoint - if the value found in the map so it is possible key for this value.
 *                  - if "myTools" doesn't contain this value then a "fake" point ((-1,-1)) is returned.
 */
    RPSPoint findKeyOfValueInMyTools(char value);

/**
 * In this function the auto player decides if it wants to make a joker change, and if it does
 * then it makes a JokerChange object with all the information about the change.
 * The auto player checks the known tools of its opponent (not 'O' in "opponentsTools"), and
 * checks if it (auto player) has a tool that can win this opponents tool. If not, checks if it
 * has a joker with representation that can win this opponent tool, if it has then it will return
 * nullptr (because it has a possible winning move for the next move), if not it will change
 * the joker to the right form that can "eat" the known opponents tool in the next move.
 * If there a no known opponent tools at all a nullptr will be returned.
 *
 * @return - nullptr if there is no joker change
 *         - unique_ptr<JokerChange> with all the information if the player wants to make a
 *         joker change with one of its jokers.
 *
 */
    virtual unique_ptr<JokerChange> getJokerChange(); // nullptr if no change is requested

/**
 * Checks if "playerJokers" has a joker that represents the tool 'c'.
 *
 * @param c - the tool representation.
 * @return if there is a joker with 'c' representation then it will return the point of this joker, if not it will
 * return a "fake" point (-1,-1).
 *
 */
    RPSPoint checkIfHasThisJokerRep(char c);

    bool checkIfPlayerJokersHasPoint(const RPSPoint &p);


/**
 * Deletes from the vector of Template T "v" the T object "p".
 *
 * @param v - the vector
 * @param p - the object to delete
 *
 */ template<class T>
    void eraseFromVector(vector<T> &v, T p);


    void eraseFromJokers(RPSPoint p, vector<unique_ptr<RPSPoint>> &v);
//    void eraseFromJokers(unique_ptr<RPSPoint> p, vector<unique_ptr<RPSPoint>> v);

/**
 * choses a random point from vector "v".
 *
 * @param v - the vector of RPSPoints
 * @return RPSPoint - the random point chosen
 *
 */
    RPSPoint getRandomPoint(vector<RPSPoint> v)const;

/**
 * Choses a random point from the keys in the map "m".
 *
 * @param m - the map
 * @return RPSPoint - from the keys in "m"
 */
    RPSPoint getRandomPoint(map<RPSPoint, char> m)const;


    /**
     *
     * @return player number
     */
    int getPlayer();

    virtual ~RSPPlayer_203022041(){}

};




#endif //EX1_RPSAUTOPLAYERALGORITHM_H
