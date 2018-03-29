

#ifndef RPSGAME_H
#define RPSGAME_H

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <set>
#include "RPSParer.h"

using namespace std;


#define M 10
#define N 10
#define R 2
#define P 5
#define S 1
#define J 2
#define B 2
#define F 1

struct Position {
    int X;
    int Y;

    bool operator<(const Position &rhs) const {
        if (X < rhs.X)
            return true;
        else if (X > rhs.X)
            return false;
        return Y < rhs.Y;
    }

    bool operator==(const Position &rhs) const {
        return X == rhs.X && Y == rhs.Y;
    }
};

class RPSGame {

    string board[M][N] = {};
    map<string, int> player1ToolCounters;
    map<string, int> player2ToolCounters;
    set<Position> player1JokerLocations;
    set<Position> player2JokerLocations;
    string player1Error;
    string player2Error;
    int winner;

public:

    RPSGame();


/**
 *
 * @param filename
 * @return
 * -1 - if the file cannot be opened
 * 0  - if the file is valid
 * (1-num_of_lines_in_file) - the line there was an error
 */
    int RPSGameInitFileCheck(string fileName, int player, map<string, int> toolCounter);

    bool RPSGameUpdateBoardPlayer1InitStage(int X, int Y, string tool, int lineNum, const bool &isJoker);

    bool
    RPSGameUpdateBoardPlayer2InitStage(int X, int Y, const string &tool, int lineNum, map<Position, string> &boardMap,
                                       const bool &isJoker);

    void RPSGameMergePlayer2BoardWithPlayer1Board(map<Position, string> &mapBoard);

    void RPSGameFightOnPosition(int X, int Y, string &attackerTool, int attackerPlayer);

    void RPSGameRPSFight(int X, int Y, string &attackerTool, int attackerPlayer);

    bool RPSGameCheckIfPlayer1Lose();

    bool RPSGameCheckIfPlayer2Lose();

    void RPSGameFightAttackerWins(int X, int Y, string &attackerTool, int player);

    void RPSGameFightAttackerLoses(int X, int Y, string &attackerTool, int player);


};

#endif //OS_EX1_RPSGAME_H
