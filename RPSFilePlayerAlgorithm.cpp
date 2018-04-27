

#include <fstream>
#include <iostream>
#include <memory>
#include "RPSFilePlayerAlgorithm.h"

using namespace std;

RPSFilePlayerAlgorithm::RPSFilePlayerAlgorithm(int player, string dir)
        : RPSPlayerAlgorithm(player), _directory(dir), moveFileLineCounter(0) {}

void RPSFilePlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill) {
    string c = player == 1 ? "1" : "2";

    string fileName = _directory + "player" + c + ".rps_board";
    ifstream fin(fileName);
    if (fin.fail()) {
        cout << "Error: There's no such file or directory, Player " << player << "'s file is not exist" << endl;
        vectorToFill.clear();
        return;
    }
    string lineToParse;
    int lineNum = 1;
    int parserResult;
    unique_ptr<PiecePosition> initPos = make_unique<RPSPiecePosition>();
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
            default:
                vectorToFill.push_back(initPos);
                playerToolCounters[initPos->getPiece()]--;
                if (initPos->getJokerRep() != '#')
                    playerJokers.push_back(initPos->getPosition());
                lineNum++;
        }
    }
    fin.close();
}

void RPSFilePlayerAlgorithm::setMovesFromMoveFile() {
    string c = _player == 1 ? "1" : "2";
    string lineToParse;
    string fileName = _directory + "player" + c + ".rps_moves";
    ifstream fin(fileName);
    if (fin.fail()) {
        cout << "Error: There's no such file or directory, Player " << _player << "'s file is not exist" << endl;
        playerMoves.clear();
        return;
    }
    unique_ptr<RPSMove> curMove = make_unique<RPSMove>();
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
        parseResult = RPSParser::parseLineMove(lineToParse, curMove);
        if(parseResult!=0)
            break;
        playerMoves.push_back(curMove);
        }
    fin.close();
    }



void
RPSFilePlayerAlgorithm::notifyOnInitialBoard(const Board &b, const std::vector<unique_ptr<FightInfo>> &fights) {}

void RPSFilePlayerAlgorithm::notifyOnOpponentMove(const Move &move) {} // called only on opponents move
void RPSFilePlayerAlgorithm::notifyFightResult(const FightInfo &fightInfo) {}// called only if there was a fight


virtual unique_ptr<Move> RPSFilePlayerAlgorithm::getMove() {
    return move(playerMoves[moveFileLineCounter]);
}

virtual unique_ptr<JokerChange> RPSFilePlayerAlgorithm::getJokerChange() {
    return playerMoves[moveFileLineCounter]->getJoker()!= nullptr ? playerMoves[moveFileLineCounter]->getJoker() : nullptr;
}