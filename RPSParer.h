#ifndef RPSPARSER_H
#define RPSPARSER_H

#include <string>
#include <vector>
#include <sstream>
#include "RPSGame.h"




using namespace std;


struct move{
    int player;
    int fromX;
    int fromY;
    int toX;
    int toY;
//    string tool;
    int joker_X;
    int joker_Y;
    string joker_tool;
};

/**
 *
 * @param line
 * @return
 * 0 - Success, line is correct
 * 1 - invalid line, not enough arguments or too much
 * 2 - invalid piece, not R P S J B F
 * 3 - invalid position
 */
int RPSParserParseLineInit(string line, int &X, int &Y, string &tool, bool &isJoker);
int RPSParser3TokensInitLine(vector<string> tokens,int& X, int& Y, string& tool);
bool RPSParserCheckIfPositionValid(string X, string Y);
int RPSParser4TokensInitLine(vector<string> tokens, int &X, int &Y, string &tool);
int RPSParser4TokensMoveLine(move& newMove, vector<string> tokens);
int RPSParser8TokensMoveLine(move &newMove, vector<string> tokens);


#endif //OS_EX1_RPSPARER_H
