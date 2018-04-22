

#include "RPSParser.h"
#include "RPSMove.h"


int RPSParser::parseLineInit(const string &line, RPSMove &initMove) {

    istringstream iss(line);
    vector<string> tokens((istream_iterator<string>(iss)), istream_iterator<string>());
    if (tokens.size() > 4 || tokens.size() < 3) {
        return 1;
    }
    if (tokens.size() == 3)
        return parse3TokensInitLine(tokens, initMove);
    initMove.setJoker();
    return parse4TokensInitLine(tokens, initMove);
}


int RPSParser::parse3TokensInitLine(vector<string> tokens, RPSMove &initMove) {
    if (tokens[0] != "R" && tokens[0] != "P" && tokens[0] != "S" && tokens[0] != "F" &&
        tokens[0] != "B")
        return 2;

    initMove.setPiece(tokens[0][0]);

    if (!checkIfPositionValid(tokens[1], tokens[2]))
        return 3;
    int x=stoi(tokens[1])-1;
    int y =stoi(tokens[2])-1);
    initMove.setFrom(x,y);
    initMove.setTo(x,y);
    return 0; // Success
}


int RPSParser::parse4TokensInitLine(vector<string> tokens, RPSMove &initMove) {
    if (tokens[0] != "J") return 1; //invalid line
    if (!checkIfPositionValid(tokens[1], tokens[2]))
        return 3;
//    initMove.toX = initMove.fromX = initMove.joker_X = stoi(tokens[1]) - 1;
//    initMove.toY = initMove.fromY = initMove.joker_Y = stoi(tokens[2]) - 1;
    if (tokens[3] != "R" && tokens[3] != "P" && tokens[3] != "S" && tokens[3] != "B")
        return 2;
    initMove.setPiece('J');
    initMove.joker_tool = tokens[3];
    initMove.isJoker=true;
    return 0; // Success

}


int RPSParser::parseLineMove(const string &line, OldMove &newMove) {
    istringstream iss(line);
    vector<string> tokens((istream_iterator<string>(iss)), istream_iterator<string>());
    if (tokens.size() != 8 && tokens.size() != 4)
        return 1;
    if (tokens.size() == 4)
        return parse4TokensMoveLine(newMove, tokens);
    return parse8TokensMoveLine(newMove, tokens);
}


int RPSParser::parse4TokensMoveLine(OldMove &newMove, vector<string> tokens) {
    if (checkIfPositionValid(tokens[0], tokens[1]) && checkIfPositionValid(tokens[2], tokens[3])) {
        newMove.fromX = stoi(tokens[0]) - 1;
        newMove.fromY = stoi(tokens[1]) - 1;
        newMove.toX = stoi(tokens[2]) - 1;
        newMove.toY = stoi(tokens[3]) - 1;
        return 0;
    }
    return 3;
}


int RPSParser::parse8TokensMoveLine(OldMove &newMove, vector<string> tokens) {
    if (checkIfPositionValid(tokens[0], tokens[1]) && checkIfPositionValid(tokens[2], tokens[3])) {
        newMove.fromX = stoi(tokens[0]) - 1;
        newMove.fromY = stoi(tokens[1]) - 1;
        newMove.toX = stoi(tokens[2]) - 1;
        newMove.toY = stoi(tokens[3]) - 1;
        if (tokens[4] != "[J:" && tokens[4] != "J:")
            return 1;
        if (!checkIfPositionValid(tokens[5], tokens[6]))
            return 3;
        newMove.joker_X = stoi(tokens[5]) - 1;
        newMove.joker_Y = stoi(tokens[6]) - 1;
        if (tokens[7] != "R]" && tokens[7] != "P]" && tokens[7] != "S]" && tokens[7] != "B]" &&
                tokens[7] != "R" && tokens[7] != "P" && tokens[7] != "S" && tokens[7] != "B")
            return 2;
        newMove.joker_tool = tokens[7][0];
        return 0;
    }
    return 3;
}


bool RPSParser::checkIfPositionValid(string X, string Y) {
    try {
        int posX = stoi(X);
        int posY = stoi(Y);
        return !(posX < 1 || posX > M || posY < 1 || posY > N);

    }
    catch (invalid_argument) { return false; }
    catch (out_of_range) { return false; }
}



