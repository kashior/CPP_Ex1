

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
 * Contains the board, and some variables to maintain the game updated at all times
 * Has all the functions that influence the flow of the game
 */
class RPSGame {

    unique_ptr<RPSPlayerAlgorithm> player1;
    unique_ptr<RPSPlayerAlgorithm> player2;

public:

    RPSBoard board;

    int movesCounter;



    RPSGame(bool &isPlayer1Auto, bool &isPlayer2Auto);

    friend class RPSManager;


    bool UpdateBoardPlayer1InitStage(int &lineNum, vector<unique_ptr<PiecePosition>> & playersPositioning,
                                     unique_ptr<RPSPlayerAlgorithm> &playerAlg);

    bool UpdateBoardPlayer2InitStage(int &lineNum, vector<unique_ptr<PiecePosition>> &playersPositioning,
                                         unique_ptr<RPSPlayerAlgorithm> &player1Alg,
                                         unique_ptr<RPSPlayerAlgorithm> &player2Alg,
                                         vector<unique_ptr<FightInfo>> &fights);

    void fightOuter(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                    unique_ptr<RPSPlayerAlgorithm> &player1Alg, unique_ptr<RPSPlayerAlgorithm> &player2Alg);

/**
 * Function that removes both of the pieces from the board after fight
 * @param newMove - the move that caused the fight
 */
    void removeBothPiecesFromGame(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                                  unique_ptr<RPSPlayerAlgorithm> &player1,
                                  unique_ptr<RPSPlayerAlgorithm> &player2);

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
 * Check if player 1 lost the game as a result of the last move performed
 * @return true if player 1 loses the game and false otherwise
 */
    bool CheckIfPlayerLose(unique_ptr<RPSPlayerAlgorithm> &player);



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


    RPSMove setMoveToBoard(unique_ptr<Move> curMove, int player, RPSFightInfo &curFight);

    void changeJokerPosition(unique_ptr<RPSPlayerAlgorithm> &playerAlg, unique_ptr<Move> &curMove);

    void printBoardToScreen(int turnNum);
};

#endif //RPSGAME_H
