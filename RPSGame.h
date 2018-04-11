

#ifndef RPSGAME_H
#define RPSGAME_H

#include <iostream>
#include <map>
#include <fstream>
#include <set>
#include <algorithm>
#include "RPSParser.h"

/**
 * A struct that represent a position on board
 * X - represents the column
 * Y - represents the row
 */
struct Position {
    int Y;
    int X;

    bool operator<(const Position &rhs) const {
        if (X < rhs.X)
            return true;
        if (X > rhs.X)
            return false;
        return Y < rhs.Y;
    }

    bool operator==(const Position &rhs) const {
        return Y == rhs.Y && X == rhs.X;
    }
};

/**
 * The main game object
 * Contains the board, and some variables to maintain the game updated at all times
 * Has all the functions that influence the flow of the game
 */
class RPSGame {

public:
    string board[N][M] = {};
    set<Position> player1JokerLocations;
    set<Position> player2JokerLocations;
    int winner;
    map<string, int> player1ToolCounters;
    map<string, int> player2ToolCounters;
    int player1Points;
    int player2Points;

    RPSGame();

/**
 * This function gets the file name and the player number, and process the initialization file of that player
 * @param filename
 * @return
 * -1 - if the file cannot be opened
 * 0  - if the file is valid
 * (1-num_of_lines_in_file) - the line there was an error
 */
    int RPSGameInitFileCheck(string fileName, int player, map<string, int> &toolCounter);

/**
 * Help function that updates the board for player 1's init moves
 * @param initMove - the current move the function will update
 * @param lineNum - line number in file
 * @return true - if the move can be updated successfully, false otherwise
 */

    bool RPSGameUpdateBoardPlayer1InitStage(const Move &initMove, const int &lineNum);

/**
 * Help function that updates a map of init moves for player 2's init moves
 * @param initMove - the current move the function will update
 * @param lineNum - line number in file
 * @return true - if the move can be updated successfully, false otherwise
 */
    bool RPSGameUpdateBoardPlayer2InitStage(Move &initMove, int lineNum, map<Position, string> &boardMap);

/**
 * This function takes the map of player 2 init moves and merge it inside the board.
 * It also take care of "Fights" that can occur as a result of the merge.
 * This function basically starts the game
 * @param mapBoard - The init moves of player 2
 */
    void RPSGameMergePlayer2BoardWithPlayer1Board(map<Position, string> &mapBoard);

/**
 * Simply updates the board with a new move
 * @param newMove - the move the function should update on the board
 */
    void RPSGameSetMoveOnBoard(Move &newMove);

/**
 * Decides which tool will stay on board and which tool will "die"
 * Also updates the game object to keep the counting of soldiers updated
 * @param newMove - the move that caused the fight
 */
    void RPSGameFightOnPosition(Move &newMove);

/**
 * Function that removes both of the pieces from the board after fight
 * @param newMove - the move that caused the fight
 */
    void RPSGameRemoveBothPiecesFromGame(Move &newMove);

/**
 * Function that performs the classic Rock Paper Scissors fight
 * @param newMove - the move that caused the fight
 */
    void RPSGameRPSFight(Move &newMove);

/**
 * Check if player 1 lost the game as a result of the last move performed
 * @return true if player 1 loses the game and false otherwise
 */
    bool RPSGameCheckIfPlayer1Lose();

/**
 * Check if player 2 lost the game as a result of the last move performed
 * @return true if player 2 loses the game and false otherwise
 */
    bool RPSGameCheckIfPlayer2Lose();

/**
 * Function that removes the tool that tried to defend himself from the attack
 * @param newMove - the move that caused the fight
 */
    void RPSGameFightAttackerWins(Move &newMove);

/**
 * Function that removes the tool that caused the fight
 * @param newMove - the move that caused the fight
 */
    void RPSGameFightAttackerLoses(Move &newMove);

/**
 * This function treats both of the move files of the 2 players.
 * reads line by line when player 1 starts, checks for errors and winners and also update the game object at all times
 * @param fileName1 - the filename of player 1 move file
 * @param fileName2 - the filename of player 2 move file
 * @param lineNum - if there will be an error in one of the files, it will be in line number lineNum
 * @return -1 - if both files does not exist
 *         0 - if both were read without any errors
 *         1 - if there was an error in player 1's file
 *         2 - if there was an error in player 2's file
 *         3 - if player 1 wins
 *         4 - if player 2 wins
 *         5 - tie
 */
    int RPSGameMoveFileCheck(string fileName1, string fileName2, int &lineNum);

/**
 * In case a move line contains also a change of a joker piece, this function checks if indeed
 * <JOKER_X> <JOKER_Y> is a position containing a joker. If the position is valid,
 * we update the position on the board to contain <NEW_REP>.
 *
 * @param curMove - the move we currently check and want to set
 * @return false - if position does not contain a joker of the current player
 *         true - otherwise
 */
    bool RPSGameCheckIfChangeJokerPieceCommandIsValid(Move &curMove);

/**
 * In case RPSGameCheckIfChangeJokerPieceCommandIsValid(Move &curMove)==false,
 * an error message will be printed to the screen by this function.
 *
 * @param lineNum - the current line num checked in a file
 * @param curMove - the current move in the line
 */
    void RPSGamePrintErrorMessageBadChangeOfJokerPiece(int lineNum, Move curMove);

/**
 * Checks for validity of the current move in the game
 * @param parseResult - the integer that the move file check function returned
 * @param player - the current player turn
 * @param curMove - the current move to check
 * @param lineNum - the current line in move file
 * @return true if the move is valid, false otherwise
 */
    bool RPSGameCheckIfMoveIsValid(int parseResult, int player, Move &curMove, int lineNum);

/**
 * Checks if source position on board contains player 1's piece (capital letter), and if destination
 * position is empty or if it contains player 2's piece (not capital letter).
 *
 * @param fromX - X coordinate of source
 * @param fromY - Y coordinate of source
 * @param toX - X coordinate of destination
 * @param toY - Y coordinate of destination
 * @param lineNum - the line number in the moves file
 * @return false - if source position on board contains player 1's piece, and if destination
 *         position is empty or if it contains player 2's piece (valid move)
 *         true - otherwise (invalid move)
 */
    bool RPSGameIsPositionContainsPlayers1Piece(const int &lineNum, const Move &curMove);

/**
 * Checks if source position on board contains player 2's piece (lower letter), and if destination
 * position is empty or if it contains player 1's piece (not lower letter).
 *
 * @param fromX - X coordinate of source
 * @param fromY - Y coordinate of source
 * @param toX - X coordinate of destination
 * @param toY - Y coordinate of destination
 * @param lineNum - the line number in the moves file
 * @return false - if source position on board contains player 2's piece, and if destination
 *         position is empty or if it contains player 1's piece (valid move)
 *         true - otherwise (invalid move)
 */
    bool RPSGameIsPositionContainsPlayers2Piece(const int &lineNum, const Move &curMove);

};

#endif //RPSGAME_H
