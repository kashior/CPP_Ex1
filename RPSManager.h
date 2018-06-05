

#ifndef EX1_RPSMANAGER_H
#define EX1_RPSMANAGER_H

#include "RPSGame.h"

/**
 * The handler of the game
 * contains the game object, the vectors for positioning and points system
 * All his target is to run the game between the 2 players inside the game object,
 * And of course to create an output file
 */
class RPSManager {

    unique_ptr<PlayerAlgorithm> player1;
    unique_ptr<PlayerAlgorithm> player2;
    int player1Points;
    int player2Points;
    vector<unique_ptr<PiecePosition>> player1Positioning;
    vector<unique_ptr<PiecePosition>> player2Positioning;
    RPSBoard board;
    int movesCounter;

public:

    //<constructors>
    RPSManager(bool isPlayer1Auto, bool isPlayer2Auto);
    //</constructors>

    // friend declerations
    friend class RPSGame;

    /**
     * Update the board from 2 vectors and perform fights
     * */

    void updateInitialBoard( vector<unique_ptr<FightInfo>> & fights);

    /**
     * Checks who wins? attacker tool or defender tool. update fights vector
     */
     void doFight(vector<unique_ptr<FightInfo>> & fights,unique_ptr<RPSMove>fightMove);

     /**
      * remove both pieces from the board and update the fights vector
      */

     void removeBothPiecesFromGame(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights);



    /**
     * this is a help functions that updates the fight vector itself
     * @param winner - who won the fight
     * @param curMove - the move that caused the fight
     * @param fights - the vector that needs to be filled
     */
    void updateFightVectors(int winner, unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights);


    /**
    * Function that removes the tool that tried to defend himself from the attack
    * @param curMove - the move that caused the fight
    */
    void fightAttackerWins(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights);


    /**
 * Function that removes the tool that caused the fight
 * @param newMove - the move that caused the fight
 */
    void fightAttackerLoses(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights);


    /**
 * Function that performs the classic Rock Paper Scissors fight
 * @param newMove - the move that caused the fight
 */
    void fightInner(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights);




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
     */
    int gameHandler();


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
    int checkWinner();

    /**
     * In case there is a winner (or two winners) the function checks if the reason for the winning is
     * because all opponents flags are eaten or because all moving tool are eaten, and updates the reason.
     *
     * @param reason - will be 1 if all flags of the opponent are captured
     *                 or 2 if all moving pieces of the opponent are eaten.
     */
    void checkAndUpdateReasonForWinner(int &reason);

    /**
     * Manages the first stage of the game, init stage
     * @param reason - the reason for winning or losing. passed by reference because we use it also later
     * @param winner - the winner player number. passed by reference because we ue it also later
     * @param fights - the fights vector to be filled
     * @return true if there are no winners during init stage, false otherwise
     */
    bool initStage( int &winner, vector<unique_ptr<FightInfo>> &fights);

    /**
     * Performs a check on this specific move validity
     * @param curMove - the current move
     * @param player - the player that plays
     * @param moreMoves - boolean parameter to be updated. passed by reference
     * @return true if this move is valid, false otherwise
     */
    bool checkIfMoveIsValid(unique_ptr<Move> &curMove, int player, bool &moreMoves);

    /**
     * Help function for the above checks on the board if this move is possible
     * @param curMove - the current move
     * @param player - the player that plays
     * @return true if this move is valid, false otherwise
     */
    bool checkIfMoveIsValidBoardwise(unique_ptr<Move> &curMove, int player);

    /**
     * Checks for validity of a joker change, if there is one
     * @param curChange - the joker change played
     * @param player - the player that plays
     * @return true if this joker change is valid, false otherwise
     */
    bool checkIfJokerChangeIsValid(unique_ptr<JokerChange> &curChange, int player);

};


#endif //EX1_RPSMANAGER_H
