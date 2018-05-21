

#ifndef RPSGAME_H
#define RPSGAME_H

#include <iostream>
#include <map>
#include <fstream>
#include <set>
#include <algorithm>
#include "RPSParser.h"
#include "RPSPiecePosition.h"
#include "RPSAutoPlayerAlgorithm.h"
#include "RPSFilePlayerAlgorithm.h"
#include "RPSBoard.h"
#include "RPSFightInfo.h"


/**
 * The main game object
 * Contains the board, moves counter and the 2 player algorithms
 * Has all the functions that influence the flow of the game.
 */
class RPSGame {

    unique_ptr<RPSPlayerAlgorithm> player1;
    unique_ptr<RPSPlayerAlgorithm> player2;

public:

    RPSBoard board;

    int movesCounter;


    //<constructors>
    RPSGame(bool &isPlayer1Auto, bool &isPlayer2Auto);
    //</constructors>

    //friend declerations
    friend class RPSManager;

    /**
     * This function fills the positioning vector(init stage) according to the player algorithm, and also positions
     * The tools on the board
     * @param lineNum Passed by reference to know where is the error, if there is one
     * @param playersPositioning - the vector we need to fill of player 1
     * @param playerAlg - the player algorithm of player 1
     * @return true if no errors were found , false otherwise
     */
    bool UpdateBoardPlayer1InitStage(int &lineNum, vector<unique_ptr<PiecePosition>> & playersPositioning,
                                     unique_ptr<RPSPlayerAlgorithm> &playerAlg);
    /**
     * This functions fills the vector of player 2, and positions the tools on the board.
     * If there are any collisions with player 1, checks who wins in the fight and keeps only the winning tool
     * Or remove both in case there is a tie. it alo fills a fights vector sent by reference
     * @param lineNum lineNum Passed by reference to know where is the error, if there is one
     * @param playersPositioning - the vector we need to fill of player 2
     * @param player1Alg - the player algorithm of player 1
     * @param player2Alg - the player algorithm of player 2
     * @param fights - the vector we need to fill in case there are collisions in positioning
     * @return true if no errors were found , false otherwise
     */
    bool UpdateBoardPlayer2InitStage(int &lineNum, vector<unique_ptr<PiecePosition>> &playersPositioning,
                                         unique_ptr<RPSPlayerAlgorithm> &player1Alg,
                                         unique_ptr<RPSPlayerAlgorithm> &player2Alg,
                                         vector<unique_ptr<FightInfo>> &fights);

    /**
     * The main fights function. uses some help functions but it is in charge on the fight itself.
     * @param curMove - the move performed that casued the fight
     * @param fights - the vector we need to fill with the fight details
     * @param player1Alg - player 1 player algorithm
     * @param player2Alg - player 2 player algorithm
     */
    void fightOuter(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                    unique_ptr<RPSPlayerAlgorithm> &player1Alg, unique_ptr<RPSPlayerAlgorithm> &player2Alg);

    /**
     * Function that removes both of the pieces from the board after fight
     * @param newMove - the move that caused the fight
     */
    void removeBothPiecesFromGame(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                                  unique_ptr<RPSPlayerAlgorithm> &player1,
                                  unique_ptr<RPSPlayerAlgorithm> &player2);

    /**
     * Gets the player algorithm and the piece that needs to be removed and do so.
     * @param player - the player algorithm that we need to remove the tools from
     * @param curMove - the move that caused the fight
     * @param pieceToRemove - the piece we need to remove from the vector
     */
    void removeToolsFromVectors(unique_ptr<RPSPlayerAlgorithm> &player, unique_ptr<RPSMove> &curMove,
                                char pieceToRemove);
    /**
     * Function that performs the classic Rock Paper Scissors fight
     * @param newMove - the move that caused the fight
     */
    void fightInner(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                    unique_ptr<RPSPlayerAlgorithm> &player1,
                    unique_ptr<RPSPlayerAlgorithm> &player2);

    /**
     * Check if a player lost the game as a result of the last move performed
     * @return true if the player loses the game and false otherwise
     */
    bool CheckIfPlayerLose(unique_ptr<RPSPlayerAlgorithm> &player);


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
    void fightAttackerWins(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                           unique_ptr<RPSPlayerAlgorithm> &player1, unique_ptr<RPSPlayerAlgorithm> &player2);

    /**
     * Function that removes the tool that caused the fight
     * @param newMove - the move that caused the fight
     */
    void fightAttackerLoses(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                            unique_ptr<RPSPlayerAlgorithm> &player1, unique_ptr<RPSPlayerAlgorithm> &player2);


    /**
     * This functions performs the move on the board in case the move was legal. calls to fight function in case there
     * is a fight
     * @param curMove - the move that need to be set
     * @param player - the player that did the move
     * @param curFight - updated by reference in case there is a fight
     * @return the RPSMove object with the details of the current move
     */
    RPSMove setMoveToBoard(unique_ptr<Move> curMove, int player, RPSFightInfo &curFight);

    /**
     * In case we moved a joker, change it's position in the player algorithm player jokers vector
     * @param playerAlg - the player algorithm container
     * @param curMove - the current move performed
     */
    void changeJokerPosition(unique_ptr<RPSPlayerAlgorithm> &playerAlg, unique_ptr<Move> &curMove);

//    void printBoardToScreen(int turnNum);
};

#endif //RPSGAME_H
