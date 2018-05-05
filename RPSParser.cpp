

#include "RPSParser.h"
#include "RPSMove.h"
#include "RPSPiecePosition.h"


int RPSParser::parseLineInit(const string &line, unique_ptr<RPSPiecePosition> &initPos) {

    istringstream iss(line);
    vector<string> tokens((istream_iterator<string>(iss)), istream_iterator<string>());
    if (tokens.size() > 4 || tokens.size() < 3) {
        return 1;
    }
    if (tokens.size() == 3)
        return parse3TokensInitLine(tokens, initPos);

    return parse4TokensInitLine(tokens, initPos);
}


int RPSParser::parse3TokensInitLine(vector<string> tokens, unique_ptr<RPSPiecePosition> &initPos) {
    if (tokens[0] != "R" && tokens[0] != "P" && tokens[0] != "S" && tokens[0] != "F" &&
        tokens[0] != "B")
        return 2;

    initPos->setPiece(tokens[0][0]);

    if (!checkIfPositionValid(tokens[1], tokens[2]))
        return 3;
    int x=stoi(tokens[1])-1;
    int y =stoi(tokens[2])-1;
    initPos->setPosition(new RPSPoint(x,y));

    return 0; // Success
}


int RPSParser::parse4TokensInitLine(vector<string> tokens, unique_ptr<RPSPiecePosition> &initPos) {
    if (tokens[0] != "J") return 1; //invalid line
    if (!checkIfPositionValid(tokens[1], tokens[2]))
        return 3;
    if (tokens[3] != "R" && tokens[3] != "P" && tokens[3] != "S" && tokens[3] != "B")
        return 2;
    int x=stoi(tokens[1])-1;
    int y =stoi(tokens[2])-1;
    initPos->setPosition(new RPSPoint(x,y));
    initPos->setPiece('J');
    initPos->setJokerRep(tokens[3][0]);

    return 0; // Success

}


int
RPSParser::parseLineMove(const string &line, unique_ptr<RPSMove> &newMove, unique_ptr<RPSJokerChange> &newJokerChange) {
    istringstream iss(line);
    vector<string> tokens((istream_iterator<string>(iss)), istream_iterator<string>());
    if (tokens.size() != 8 && tokens.size() != 4)
        return 1;
    if (tokens.size() == 4)
        return parse4TokensMoveLine(newMove, tokens);
    return parse8TokensMoveLine(newMove, tokens, newJokerChange);
}


int RPSParser::parse4TokensMoveLine(unique_ptr<RPSMove> &newMove, vector<string> tokens) {
    if (checkIfPositionValid(tokens[0], tokens[1]) && checkIfPositionValid(tokens[2], tokens[3])) {
        int fromX = stoi(tokens[0]) - 1;
        int fromY = stoi(tokens[1]) - 1;
        int toX = stoi(tokens[2]) - 1;
        int toY = stoi(tokens[3]) - 1;
        newMove->setFrom(fromX, fromY);
        newMove->setTo(toX, toY);
        return 0;
    }
    return 3;
}


int RPSParser::parse8TokensMoveLine(unique_ptr<RPSMove> &newMove, vector<string> tokens,
                                   unique_ptr<RPSJokerChange> &newJokerChange) {

    if (checkIfPositionValid(tokens[0], tokens[1]) && checkIfPositionValid(tokens[2], tokens[3])) {
        int fromX = stoi(tokens[0]) - 1;
        int fromY = stoi(tokens[1]) - 1;
        int toX = stoi(tokens[2]) - 1;
        int toY = stoi(tokens[3]) - 1;
        newMove->setFrom(fromX, fromY);
        newMove->setTo(toX, toY);

        if (tokens[4] != "J:")
            return 1;
        if (!checkIfPositionValid(tokens[5], tokens[6]))
            return 3;
        int joker_X = stoi(tokens[5]) - 1;
        int joker_Y = stoi(tokens[6]) - 1;

        if (tokens[7] != "R" && tokens[7] != "P" && tokens[7] != "S" && tokens[7] != "B")
            return 2;
        newJokerChange->setJokerNewRep(tokens[7][0]);
        newJokerChange->setJokerChangePosition(RPSPoint(joker_X,joker_Y));

//        newMove->setJoker(tokens[7][0], new RPSPoint(joker_X, joker_Y));
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



