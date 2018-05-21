
#ifndef EX1_RPSFILEPLAYERALGORITHM_H
#define EX1_RPSFILEPLAYERALGORITHM_H


#include "RPSPlayerAlgorithm.h"
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
using namespace std;


/**
 * class RPSFilePlayerAlgorithm
 * Manages the initialization and moves during the game of a the file player according to the files
 * "player(1/2).rps_board and player(1/2).rps_moves.
 *
 * string _directory - directory for the files mentioned above
 *
 * vector<pair<unique_ptr<RPSMove>,unique_ptr<RPSJokerChange>>> playerMoves - vector
 * of pairs of all the moves that are in the file player(1/2).rps_moves and the joker change of this move,
 * if there is.
 *
 * int moveFileLineCounter - counter of the lines in the moves file (mainly for error handeling)
 *
 */

class RPSFilePlayerAlgorithm: public RPSPlayerAlgorithm {
    string _directory;
    vector<pair<unique_ptr<RPSMove>,unique_ptr<RPSJokerChange>>> playerMoves;
    int moveFileLineCounter;

public:

/**
 * C'tor for RPSFilePlayerAlgorithm.
 * initaites the player (1 or 2), sets the directory to "dir" and sets the line counter to 0.
 *
 * @param player
 * @param dir
 */
    RPSFilePlayerAlgorithm(int player, string dir);

/**
 * Fills the initial positions vector.
 * The file player goes over the rps_board file and converts each line to a PiecePosition object.
 * In case of invalid line in the file, handles it propertly.
 *
 * @param player - the current player 1 or 2
 * @param vectorToFill
 */
    virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) ;


    virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights);


    virtual void notifyOnOpponentMove(const Move& move); // called only on opponents move


    virtual void notifyFightResult(const FightInfo& fightInfo) ; // called only if there was a fight

/**
 * Return the next move in the moves file (but from the moves vector).
 * If there are no more moves in the file, returns an empty move.
 *
 * @return unique_ptr<Move>
 */
    virtual unique_ptr<Move> getMove();

/**
 * Checks if there is a joker change in move number 'moveFileLineCounter'. If yes,
 * return the JokerChange with all the information (stored in "playerMoves"), else returns
 * nullptr.
 *
 * @return unique_ptr<JokerChange> or nullptr
 *
 */
    virtual unique_ptr<JokerChange> getJokerChange() ; // nullptr if no change is requested

/**
 * Puts all the moves written in rps_moves into a vector of RPSMoves.
 * In case of an invalid directory or an empty file handles it.
 */
    void setMovesFromMoveFile();


    virtual ~RPSFilePlayerAlgorithm() {}

};


#endif //EX1_RPSFILEPLAYERALGORITHM_H
