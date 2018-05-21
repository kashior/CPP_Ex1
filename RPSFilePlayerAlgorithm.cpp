

#include <fstream>
#include <iostream>
#include <memory>
#include "RPSFilePlayerAlgorithm.h"

using namespace std;



RPSFilePlayerAlgorithm::RPSFilePlayerAlgorithm(int player, string dir)
        : RPSPlayerAlgorithm(player), _directory(dir), moveFileLineCounter(0) {
    setMovesFromMoveFile();
}


void RPSFilePlayerAlgorithm::getInitialPositions(int player, vector<unique_ptr<PiecePosition>> &vectorToFill) {
    string c = player == 1 ? "1" : "2";

    string fileName =  "player" + c + ".rps_board";
    ifstream fin(fileName);
    if (fin.fail()) {
        cout << "Error: There's no such file or directory, Player " << player << "'s file is not exist" << endl;
        vectorToFill.clear();
        return;
    }
    string lineToParse;
    lineNum=1;
    int parserResult;
    unique_ptr<RPSPiecePosition> initPos;
    while (true) {
        lineToParse = "";
        getline(fin, lineToParse);
        if (lineToParse.empty()) {
            if (lineNum == 1) {
                cout << "Error: Init file of player " << player << " is empty" << endl;
                vectorToFill.clear();
                return;
            }
            break;
        }
        initPos = make_unique<RPSPiecePosition>();
        parserResult = RPSParser::parseLineInit(lineToParse, initPos);
        switch (parserResult) {
            case 1:
                cout << "Error: Invalid number of arguments in line " << lineNum << " of player "
                     << player << "'s file" << endl;
                vectorToFill.clear();
                return;
            case 2:
                cout << "Error: Invalid piece argument in line " << lineNum << " of player "
                     << player << "'s file" << endl;
                vectorToFill.clear();
                return;
            case 3:
                cout << "Error: Invalid position on board in line " << lineNum << " of player "
                     << player << "'s file" << endl;
                vectorToFill.clear();
                return;
            default: //this line is a valid positioning
                vectorToFill.push_back(move(initPos));
                lineNum++;
        }
    }
    fin.close();
}


void RPSFilePlayerAlgorithm::setMovesFromMoveFile() {
    string c = getPlayer() == 1 ? "1" : "2";
    string lineToParse;
    string fileName =  "player" + c + ".rps_moves";
    ifstream fin(fileName);
    if (fin.fail()) {
        cout << "Error: There's no such file or directory, Player " << getPlayer() << "'s file is not exist" << endl;
        playerMoves.clear();
        return;
    }
    unique_ptr<RPSMove> curMove;
    unique_ptr<RPSJokerChange> curJokerChange;
    bool firstRow = true;
    int parseResult;
    while (true) {

        lineToParse = "";
        getline(fin, lineToParse);
        if (lineToParse.empty()) {
            if (firstRow) {
                playerMoves.clear();
                cout << "Error: Player" + c + "'s moves file is empty" << endl;
                return;
            }
            break;
        }
        firstRow = false;
        curMove = make_unique<RPSMove>();
        curJokerChange=make_unique<RPSJokerChange>();
        parseResult = RPSParser::parseLineMove(lineToParse, curMove, curJokerChange);
        if (parseResult!=0){ //the move is invalid
            curMove->setFrom(-2,-2); // this is a flag for the game manager to know its
                                     // an invalid move.
            playerMoves.push_back(make_pair(move(curMove),move(curJokerChange)));
            break; //no need to check more moves after invalid move
        }
        playerMoves.push_back(make_pair(move(curMove),move(curJokerChange)));
    }
    fin.close();
}


void RPSFilePlayerAlgorithm::notifyOnInitialBoard(const Board &b, const std::vector<unique_ptr<FightInfo>> &fights) {}


void RPSFilePlayerAlgorithm::notifyOnOpponentMove(const Move &move) {}


void RPSFilePlayerAlgorithm::notifyFightResult(const FightInfo &fightInfo) {}


 unique_ptr<Move> RPSFilePlayerAlgorithm::getMove() {
    if(playerMoves.size()>moveFileLineCounter)
        return move(playerMoves[moveFileLineCounter++].first);
    return make_unique<RPSMove>();
}


 unique_ptr<JokerChange> RPSFilePlayerAlgorithm::getJokerChange() {
    if (playerMoves.size()==0)
        return nullptr;
    if(playerMoves[moveFileLineCounter-1].second->getJokerNewRep()!= '#'){
        return move(playerMoves[moveFileLineCounter-1].second);
    }
    return nullptr;
}



