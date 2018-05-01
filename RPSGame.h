

#ifndef RPSGAME_H
#define RPSGAME_H

#include <iostream>
#include <map>
#include <fstream>
#include <set>
#include <algorithm>
#include "RPSParser.h"
#include "RPSPiecePosition.h"
#include "Board.h"
#include "RPSPlayerAlgorithm.h"
#include "RPSBoard.h"
#include "RPSFightInfo.h"


/**
 * The main game object
 * Contains the board, and some variables to maintain the game updated at all times
 * Has all the functions that influence the flow of the game
 */
class RPSGame {

public:
    unique_ptr<RPSBoard> board;
    vector<unique_ptr<PiecePosition>> initPosPlayer1;
    vector<unique_ptr<PiecePosition>> initPosPlayer2;
    int movesCounter;
    //unique_ptr<RPSPlayerAlgorithm> player1;
    //unique_ptr<RPSPlayerAlgorithm> player2;

    friend class RPSManager;

    RPSGame();


/** NEED TO CORRECT DOC
 * This function gets the file name and the player number, and process the initialization file of that player
 * @param filename
 * @return
 * -1 - if the file cannot be opened
 * 0  - if the file is valid
 * (1-num_of_lines_in_file) - the line there was an error
 */
    bool RPSGameInitFileCheck(unique_ptr<PlayerAlgorithm> & playerAlg, int playerNum);

    bool UpdateBoardPlayer1InitStage(int &lineNum);

    bool UpdateBoardPlayer2InitStage(int &lineNum, vector<unique_ptr<FightInfo>> &fights1, vector<unique_ptr<FightInfo>> &fights2);

    void FightOnPosition(RPSMove curMove, vector<unique_ptr<FightInfo>> &fights1, vector<unique_ptr<FightInfo>> &fights2);

/**
 * Function that removes both of the pieces from the board after fight
 * @param newMove - the move that caused the fight
 */
    void RemoveBothPiecesFromGame(RPSMove curMove, vector<unique_ptr<FightInfo>> &fights1, vector<unique_ptr<FightInfo>> &fights2);

    void RemoveToolsFromVectors(unique_ptr<RPSPlayerAlgorithm> &player, RPSMove curMove, char pieceToRemove);
/**
 * Function that performs the classic Rock Paper Scissors fight
 * @param newMove - the move that caused the fight
 */
    void RPSGameRPSFight(RPSParser::OldMove &newMove);

/**
 * Check if player 1 lost the game as a result of the last move performed
 * @return true if player 1 loses the game and false otherwise
 */
//    bool RPSGameCheckIfPlayer1Lose();
//
///**
// * Check if player 2 lost the game as a result of the last move performed
// * @return true if player 2 loses the game and false otherwise
// */
//    bool RPSGameCheckIfPlayer2Lose();


    void updateFightVectors(int winner, RPSMove curMove, vector<unique_ptr<FightInfo>> &fights1,
                            vector<unique_ptr<FightInfo>> &fights2);



/**
 * Function that removes the tool that tried to defend himself from the attack
 * @param curMove - the move that caused the fight
 */
    void FightAttackerWins(RPSMove curMove, vector<unique_ptr<FightInfo>> &fights1, vector<unique_ptr<FightInfo>>  &fights2);

/**
 * Function that removes the tool that caused the fight
 * @param newMove - the move that caused the fight
 */
    void FightAttackerLoses(RPSMove curMove, vector<unique_ptr<FightInfo>> &fights1, vector<unique_ptr<FightInfo>>  &fights2);

///**
// * This function treats both of the move files of the 2 players.
// * reads line by line when player 1 starts, checks for errors and winners and also update the game object at all times
// * @param fileName1 - the filename of player 1 move file
// * @param fileName2 - the filename of player 2 move file
// * @param lineNum - if there will be an error in one of the files, it will be in line number lineNum
// * @return -1 - if both files does not exist
// *         0 - if both were read without any errors
// *         1 - if there was an error in player 1's file
// *         2 - if there was an error in player 2's file
// *         3 - if player 1 wins
// *         4 - if player 2 wins
// *         5 - tie
// */
//    int RPSGameMoveFileCheck(string fileName1, string fileName2, int &lineNum);
//
///**
// * In case a move line contains also a change of a joker piece, this function checks if indeed
// * <JOKER_X> <JOKER_Y> is a position containing a joker. If the position is valid,
// * we update the position on the board to contain <NEW_REP>.
// *
// * @param curMove - the move we currently check and want to set
// * @return false - if position does not contain a joker of the current player
// *         true - otherwise
// */
//    bool RPSGameCheckIfChangeJokerPieceCommandIsValid(RPSParser::OldMove &curMove);
//
///**
// * In case RPSGameCheckIfChangeJokerPieceCommandIsValid(Move &curMove)==false,
// * an error message will be printed to the screen by this function.
// *
// * @param lineNum - the current line num checked in a file
// * @param curMove - the current move in the line
// */
//    void RPSGamePrintErrorMessageBadChangeOfJokerPiece(int lineNum, const RPSParser::OldMove& curMove);
//
///**
// * Checks for validity of the current move in the game
// * @param parseResult - the integer that the move file check function returned
// * @param player - the current player turn
// * @param curMove - the current move to check
// * @param lineNum - the current line in move file
// * @return true if the move is valid, false otherwise
// */
//    bool RPSGameCheckIfMoveIsValid(int parseResult, int player, RPSParser::OldMove &curMove, int lineNum);
//
///**
// * Checks if source position on board contains player 1's piece (capital letter), and if destination
// * position is empty or if it contains player 2's piece (not capital letter).
// *
// * @param fromX - X coordinate of source
// * @param fromY - Y coordinate of source
// * @param toX - X coordinate of destination
// * @param toY - Y coordinate of destination
// * @param lineNum - the line number in the moves file
// * @return false - if source position on board contains player 1's piece, and if destination
// *         position is empty or if it contains player 2's piece (valid move)
// *         true - otherwise (invalid move)
// */
//    bool RPSGameIsPositionContainsPlayers1Piece(const int &lineNum, const RPSParser::OldMove &curMove);
//
///**
// * Checks if source position on board contains player 2's piece (lower letter), and if destination
// * position is empty or if it contains player 1's piece (not lower letter).
// *
// * @param fromX - X coordinate of source
// * @param fromY - Y coordinate of source
// * @param toX - X coordinate of destination
// * @param toY - Y coordinate of destination
// * @param lineNum - the line number in the moves file
// * @return false - if source position on board contains player 2's piece, and if destination
// *         position is empty or if it contains player 1's piece (valid move)
// *         true - otherwise (invalid move)
// */
//    bool RPSGameIsPositionContainsPlayers2Piece(const int &lineNum, const RPSParser::OldMove &curMove);
//
};

#endif //RPSGAME_H
