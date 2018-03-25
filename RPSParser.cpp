

#include <iterator>
#include "RPSParer.h"


int RPSParserParseLineInit(string line, int &X, int &Y, string &tool) {

    istringstream iss(line);
    vector<string> tokens((istream_iterator<string>(iss)), istream_iterator<string>());
    if (tokens.size() > 4 || tokens.size() < 3) {
        return 1;
    } else if (tokens.size() == 3)
        return RPSParser3TokensInitLine(tokens, X, Y, tool);
    return RPSParser4TokensInitLine(tokens, X, Y, tool);
}

int RPSParser3TokensInitLine(vector<string> tokens, int &X, int &Y, string &tool) {
    if (tokens[0] != "R" && tokens[0] != "P" && tokens[0] != "S" && tokens[0] != "F" &&
        tokens[0] != "B")
        return 2;
    else {
        tool = tokens[0];
    }
    if (!RPSParserCheckIfPositionValid(tokens[1], tokens[2]))
        return 3;
    X = stoi(tokens[1]);
    Y = stoi(tokens[2]);
    return 0; // Success
}


int RPSParser4TokensInitLine(vector<string> tokens, int &X, int &Y, string &tool) {
    if (tokens[0] != "J") return 1; //invalid line
    if (!RPSParserCheckIfPositionValid(tokens[1], tokens[2]))
        return 3;
    X = stoi(tokens[1]);
    Y = stoi(tokens[2]);
    if (tokens[3] != "R" && tokens[3] != "P" && tokens[3] != "S" && tokens[3] != "B")
        return 2;
    tool = "J_" + tokens[3];
    return 0; // Success

}


int RPSParserParseLineMove(const string &line, move &newMove) {
    istringstream iss(line);
    vector<string> tokens((istream_iterator<string>(iss)), istream_iterator<string>());
    if (tokens.size() != 8 && tokens.size() != 4)
        return 1;
    if (tokens.size() == 4)
        return RPSParser4TokensMoveLine(newMove, tokens);
    return RPSParser8TokensMoveLine(newMove,tokens);
}


int RPSParser4TokensMoveLine(move &newMove, vector<string> tokens) {
    if (RPSParserCheckIfPositionValid(tokens[0], tokens[1]) && RPSParserCheckIfPositionValid(tokens[2], tokens[3])) {
        newMove.fromX = stoi(tokens[0]);
        newMove.fromY = stoi(tokens[1]);
        newMove.toX = stoi(tokens[2]);
        newMove.toY = stoi(tokens[3]);
        return 0;
    }
    return 3;

}

int RPSParser8TokensMoveLine(move &newMove, vector<string> tokens) {
    if (RPSParserCheckIfPositionValid(tokens[0], tokens[1]) && RPSParserCheckIfPositionValid(tokens[2], tokens[3])) {
        newMove.fromX = stoi(tokens[0]);
        newMove.fromY = stoi(tokens[1]);
        newMove.toX = stoi(tokens[2]);
        newMove.toY = stoi(tokens[3]);
        if (tokens[4] != "J:")
            return 1;
        if (!RPSParserCheckIfPositionValid(tokens[5], tokens[6]))
            return 3;
        newMove.joker_X = stoi(tokens[5]);
        newMove.joker_Y = stoi(tokens[6]);
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
