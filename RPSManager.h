

#ifndef EX1_RPSMANAGER_H
#define EX1_RPSMANAGER_H

#include "RPSGame.h"
//#include "RPSAutoPlayerAlgorithm.h"
//#include "RPSFilePlayerAlgorithm.h"


class RPSManager{

    unique_ptr<RPSGame> curGame;
    int player1Points;
    int player2Points;
    vector<unique_ptr<PiecePosition>> player1Positioning;
    vector<unique_ptr<PiecePosition>> player2Positioning;

public:

    friend class RPSGame;

    RPSManager(bool isPlayer1Auto, bool isPlayer2Auto);

/**
  * Gets the initial positioning of a player, puts the positions in a
  * positioning vector and checks if the positioning is valid.
  * Actually a bad positioning is only possible if the player is a "File Algorithm".
  *
  * @param playerNum - the player to check its positioning
  * @return true - if the positioning is valid, false - otherwise
  */
    bool initCheck(int playerNum);


/**
 * The "main manager" of the game. Handles the all flow of the game.
 *
 * @param isPlayer1Auto - true if player 1 is auto-algorithm, false if it is file-algorythm.
 * @param isPlayer2Auto - true if player 2 is auto-algorithm, false if it is file-algorythm.
 */
    void gameHandler();


/**
 * Updates the winner of the game according to the parameters sent to it.
 *
 * @param param1 - true if player 1 is the winner, false otherwise
 * @param param2 - true if player 2 is the winner, false otherwise
 * @param winner - will be set by the function to 1 if player 1 is the winner, 2 if player 2 is the winner or set to 0
 * if its a tie.
 *
 */
    void updateWinner(bool param1, bool param2, int &winner);

/**
 * Writes the output file "rps.output"
 *
 * @param reason
 * @param param1
 * @param param2
 * @param winner
 * @param lineNum1 - the bad line in player1's files (if there is)
 * @param lineNum2 - the bad line in player2's files (if there is)
 */
    void makeOutputFile(int reason, bool param1, bool param2, int winner, int lineNum1, int lineNum2);

/**
 *
 * This function is called after reading the moves input files, and when one of the lines in one of
 * the files is invalid. Then it updates the loser (according to the winner), and the "bad" line (number of invalid line).
 *
 * @param winner
 * @param loser
 * @param param1
 * @param param2
 * @param badLine
 */
    void updateLoserAndBadLine(int winner, int &loser, int param1, int param2, int &badLine);

/**
 * This function is called when we know that at least one of the players lost all of his flags or
 * moving pieces.
 * It checks the game board and updates the winner.
 *
 * @param winner - will be set to 1 if player1 wins, 2 if player2 wins or 0 if its a tie (both "won").
 * @param reason - according to the reason value we will know which reason to write in the output file.
 *
 */
    void checkWinner(int &winner, int &reason);

/**
 * In case there is a winner (or two winners) the function checks if the reason for the winning is
 * because all opponents flags are eaten or because all moving tool are eaten, and updates the reason.
 *
 * @param reason - will be 1 if all flags of the opponent are captured
 *                 or 2 if all moving pieces of the opponent are eaten.
 */
    void checkAndUpdateReasonForWinner(int &reason);

/**
 * Parses the command line, checks if it is from the form "<auto/file>-vs-<auto/file>", and if it is valid
 * sets the matching player-algorithms.
 *
 * @param isPlayer1Auto - will be set to true if player1 is auto, false otherwise.
 * @param isPlayer2Auto - will be set to true if player2 is auto, false otherwise.
 * @param args - the string from the command line.
 *
 * @return - true if the command line is valid, false otherwise.
 *
 */


    bool checkIfMoveIsValid(unique_ptr<Move> &curMove, int player, bool &moreMoves);

    bool checkIfMoveIsValidBoardwise(unique_ptr<Move> &curMove, int player);

    bool checkIfJokerChangeIsValid(unique_ptr<JokerChange> &curChange, int player);


};




#endif //EX1_RPSMANAGER_H
