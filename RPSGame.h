

#ifndef RPSGAME_H
#define RPSGAME_H

#include <iostream>
#include <map>
#include <fstream>
#include <set>
#include "RPSParser.h"
#include <algorithm>

using namespace std;




struct Position {
    int X;
    int Y;

    bool operator<(const Position &rhs) const {
        if (X < rhs.X)
            return true;
        if (X > rhs.X)
            return false;
        return Y < rhs.Y;
    }

    bool operator==(const Position &rhs) const {
        return X == rhs.X && Y == rhs.Y;
    }
};

class RPSGame {

public:
    string board[M][N] = {};
    set<Position> player1JokerLocations;
    set<Position> player2JokerLocations;
    string player1Error;
    string player2Error;
    int winner;
    map<string, int> player1ToolCounters;
    map<string, int> player2ToolCounters;
    int player1Points;
    int player2Points;
    RPSGame();


/**
 *
 * @param filename
 * @return
 * -1 - if the file cannot be opened
 * 0  - if the file is valid
 * (1-num_of_lines_in_file) - the line there was an error
 */
    int RPSGameInitFileCheck(const string fileName, int player, map<string, int>& toolCounter);

    bool RPSGameUpdateBoardPlayer1InitStage(const Move &initMove, const int &lineNum);

    bool
    RPSGameUpdateBoardPlayer2InitStage(Move &initMove, int lineNum, map<Position, string> &boardMap);

    void RPSGameMergePlayer2BoardWithPlayer1Board(map<Position, string> &mapBoard);

    void RPSGameSetMoveOnBoard(Move &newMove);

    void RPSGameFightOnPosition(Move &newMove);

    void RPSGameRemoveBothPiecesFromGame(Move &newMove);

    void RPSGameRPSFight(Move &newMove);

    bool RPSGameCheckIfPlayer1Lose();

    bool RPSGameCheckIfPlayer2Lose();

    void RPSGameFightAttackerWins(Move &newMove);

    void RPSGameFightAttackerLoses(Move &newMove);

    int RPSGameMoveFileCheck(string fileName1, string fileName2, int &lineNum);

    bool RPSGameCheckIfChangeJokerPieceCommandIsValid(Move &curMove);

    void RPSGamePrintErrorMessageBadChangeOfJokerPiece(int lineNum, Move curMove);

    bool RPSGameCheckIfMoveIsValid(int parseResult, int player, Move& curMove, int lineNum);

    bool RPSGameIsPositionContainsPlayers1Piece(int fromX, int fromY, int toX, int toY, int lineNum);

    bool RPSGameIsPositionContainsPlayers2Piece(int fromX, int fromY, int toX, int toY, int lineNum);

};

#endif //OS_EX1_RPSGAME_H
