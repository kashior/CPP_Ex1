

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
    if(!RPSParserCheckIfPositionValid(tokens[1],tokens[2]))
        return 3;
    X=stoi(tokens[1]);
    Y=stoi(tokens[2]);
    return 0; // Success
}


int RPSParser4TokensInitLine(vector<string> tokens, int &X, int &Y, string &tool){
    if (tokens[0] != "J") return 1; //invalid line
    if(!RPSParserCheckIfPositionValid(tokens[1],tokens[2]))
        return 3;
    X=stoi(tokens[1]);
    Y=stoi(tokens[2]);
    if (tokens[3] != "R" && tokens[3] != "P" && tokens[3] != "S"  && tokens[3] != "B")
        return 2;
    tool = "J_" + tokens[3];
    return 0; // Success

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
