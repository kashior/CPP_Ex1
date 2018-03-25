

#ifndef RPSGAME_H
#define RPSGAME_H

#include <iostream>
#include <map>
#include <string>
#include <fstream>
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


class RPSGame {

    static int player1Points;
    static int player2Points;
    string board[M][N] = {};
//    int player1Tools[6] = {};
//    int player2Tools[6] = {};
    map<string, int> player1ToolCounters;
    map<string, int> player2ToolCounters;

//    bool player1Illegal;
//    bool player2Illegal;
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

    bool RPSGameUpdateBoardPlayer1InitStage(int X, int Y, string tool, int lineNum);
    bool RPSGameUpdateBoardPlayer2InitStage(int X, int Y, string tool, int lineNum, map<int[2], string>& boardMap);

    





};

#endif //OS_EX1_RPSGAME_H
