#include <string>
#include "RPSGame.h"
using namespace std;

#ifndef OS_EX1_RPSPARER_H
#define OS_EX1_RPSPARER_H


/**
 *
 * @param line
 * @return
 * 0 - Success, line is correct
 * 1 - invalid line, not enough arguments or too much
 * 2 - invalid piece, not R P S J B F
 * 3 - invalid position
 */
int RPSParserParseLineInit(string line, int& X,int& Y, string &tool);
int RPSParser3TokensInitLine(vector<string> tokens,int& X, int& Y, string& tool);
bool RPSParserCheckIfPositionValid(string X, string Y);
int RPSParser4TokensInitLine(vector<string> tokens, int &X, int &Y, string &tool);



#endif //OS_EX1_RPSPARER_H
