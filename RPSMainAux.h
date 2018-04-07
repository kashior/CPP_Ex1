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


void RPSMainAuxCheckAndUpdateReasonForWinner(RPSGame* game, int &reason);

void RPSMainAuxFinalCheckOfGameBoard(int &winner, int &reason, RPSGame* game);

void RPSMainAuxUpdateWinner(int param1, int param2, int &winner);

int RPSMaimAuxTerminateTheGame(RPSGame* game);

void RPSMainAuxUpdateLoserAndBadLine(int winner, int &loser, int param1, int param2 , int &badLine);

void RPSMainAuxMakeOutputFile(int reason, int result1, int result2, int winner, RPSGame* game);

int RPSMainAuxGameHandler();


#endif //EX1_RPSMAINAUX_H
