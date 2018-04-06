#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <set>
#include "RPSGame.h"
#include <algorithm>

using namespace std;


#ifndef EX1_RPSMAINAUX_H
#define EX1_RPSMAINAUX_H

void RPSMainAuxFinalCheckOfGameBoard(int &winner, int &reason, RPSGame* game);

int RPSMainAuxGameHandler();

void RPSMainAuxUpdateWinner(int param1, int param2, int &winner);

void RPSMainAuxMakeOutputFile(int reason, int result1, int result2, int winner, RPSGame* game);

void RPSMainAuxUpdateLoserAndBadLine(int winner, int &loser, int param1, int param2 , int &badLine);

#endif //EX1_RPSMAINAUX_H
