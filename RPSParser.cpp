
#include <iterator>
#include "RPSParser.h"



int RPSParserParseLineInit(const string &line, Move &initMove) {

    istringstream iss(line);
    vector<string> tokens((istream_iterator<string>(iss)), istream_iterator<string>());
    if (tokens.size() > 4 || tokens.size() < 3) {
        return 1;
    }
    if (tokens.size() == 3)
        return RPSParser3TokensInitLine(tokens, initMove);
    initMove.isJoker=true;
    return RPSParser4TokensInitLine(tokens, initMove);
}



int RPSParser3TokensInitLine(vector<string> tokens, Move &initMove) {
    if (tokens[0] != "R" && tokens[0] != "P" && tokens[0] != "S" && tokens[0] != "F" &&
        tokens[0] != "B")
        return 2;

    initMove.tool = tokens[0];

    if (!RPSParserCheckIfPositionValid(tokens[1], tokens[2]))
        return 3;
    initMove.toX=initMove.fromX = stoi(tokens[1])-1;
    initMove.toY=initMove.fromY = stoi(tokens[2])-1;
    return 0; // Success
}



int RPSParser4TokensInitLine(vector<string> tokens, Move &initMove) {
    if (tokens[0] != "J") return 1; //invalid line
    if (!RPSParserCheckIfPositionValid(tokens[1], tokens[2]))
        return 3;
    initMove.toX=initMove.fromX = initMove.joker_X=stoi(tokens[1])-1;
    initMove.toY=initMove.fromY = initMove.joker_Y=stoi(tokens[2])-1;
    if (tokens[3] != "R" && tokens[3] != "P" && tokens[3] != "S" && tokens[3] != "B")
        return 2;
    initMove.tool=initMove.joker_tool = tokens[3];
    return 0; // Success

}


int RPSParserParseLineMove(const string &line, Move &newMove) {
    istringstream iss(line);
    vector<string> tokens((istream_iterator<string>(iss)), istream_iterator<string>());
    if (tokens.size() != 8 && tokens.size() != 4)
        return 1;
    if (tokens.size() == 4)
        return RPSParser4TokensMoveLine(newMove, tokens);
    return RPSParser8TokensMoveLine(newMove, tokens);
}


int RPSParser4TokensMoveLine(Move &newMove, vector<string> tokens) {
    if (RPSParserCheckIfPositionValid(tokens[0], tokens[1]) && RPSParserCheckIfPositionValid(tokens[2], tokens[3])) {
        newMove.fromX = stoi(tokens[0])-1;
        newMove.fromY = stoi(tokens[1])-1;
        newMove.toX = stoi(tokens[2])-1;
        newMove.toY = stoi(tokens[3])-1;
        return 0;
    }
    return 3;
}


int RPSParser8TokensMoveLine(Move &newMove, vector<string> tokens) {
    if (RPSParserCheckIfPositionValid(tokens[0], tokens[1]) && RPSParserCheckIfPositionValid(tokens[2], tokens[3])) {
        newMove.fromX = stoi(tokens[0])-1;
        newMove.fromY = stoi(tokens[1])-1;
        newMove.toX = stoi(tokens[2])-1;
        newMove.toY = stoi(tokens[3])-1;
        if (tokens[4] != "J:")
            return 1;
        if (!RPSParserCheckIfPositionValid(tokens[5], tokens[6]))
            return 3;
        newMove.joker_X = stoi(tokens[5])-1;
        newMove.joker_Y = stoi(tokens[6])-1;
        if (tokens[7] != "R" || tokens[7] != "P" || tokens[7] != "S" || tokens[7] != "B")
            return 2;
        newMove.joker_tool = tokens[7];
        return 0;
    }
    return 3;
}



bool RPSParserCheckIfPositionValid(string X, string Y) {
    try {
        int posX = stoi(X);
        int posY = stoi(Y);
        return !(posX < 1 || posX > M || posY < 1 || posY > N);

    }
    catch (invalid_argument) { return false; }
    catch (out_of_range) { return false; }
}



