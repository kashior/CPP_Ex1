#include "RPSGame.h"

using namespace std;

RPSGame::RPSGame() : winner(0), player1Error(""), player2Error("") {
    player1ToolCounters["R"] = R;
    player1ToolCounters["P"] = P;
    player1ToolCounters["S"] = S;
    player1ToolCounters["B"] = B;
    player1ToolCounters["J"] = J;
    player1ToolCounters["F"] = F;
    player2ToolCounters["r"] = R;
    player2ToolCounters["p"] = P;
    player2ToolCounters["s"] = S;
    player2ToolCounters["b"] = B;
    player2ToolCounters["j"] = J;
    player2ToolCounters["f"] = F;
}


int RPSGame::RPSGameInitFileCheck(string fileName, int player, map<string, int> toolCounter) {

    ifstream fin(fileName);
    if (fin.fail()) {
        cout << "Error: There's no such file or directory, Player " << player << "'s file is not exist" << endl;
        return -1;
    }
    string lineToParse;
    int X = 0, Y = 0;
    bool isJoker=false;
    int lineNum = 1;
    int parserResult;
    string tool;
    map<int[2], string> player2BoardMap;
    while (true) {
        getline(fin, lineToParse);
        if (lineToParse.empty()) {
            if (lineNum == 1) {
                cout << "Error: Init file of player " << player << " is empty" << endl;
                return -1;
            }
            break;
        }
        parserResult = RPSParserParseLineInit(lineToParse, X, Y, tool, <#initializer#>);
        switch (parserResult) {
            case 1:
                cout << "Error: Invalid number of arguments in line " << lineNum << " of player "
                     << player << "'s file" << endl;
                return lineNum;
            case 2:
                cout << "Error: Invalid piece argument in line " << lineNum << " of player "
                     << player << "'s file" << endl;
                return lineNum;
            case 3:
                cout << "Error: Invalid position on board in line " << lineNum << " of player "
                     << player << "'s file" << endl;
                return lineNum;
            default:
                if (player == 1) {
                    if (!RPSGameUpdateBoardPlayer1InitStage(X, Y, tool, lineNum, <#initializer#>)) {
                        return lineNum;
                    }
                } else {
                    if (!RPSGameUpdateBoardPlayer2InitStage(X, Y, tool, lineNum, player2BoardMap, <#initializer#>)) {
                        return lineNum;
                    }
                }
                lineNum++;
        }
    }

    if (toolCounter["F"] != 0) {
        cout << "Error: Missing Flags - Flags are not positioned according to their number in line " <<
             lineNum - 1 << " of player " << player << "'s file" <<
             endl;
        return lineNum - 1;
    }
    if (player == 2) {
        RPSGameMergePlayer2BoardWithPlayer1Board(player2BoardMap);
    }

    return 0;
}

bool RPSGame::RPSGameUpdateBoardPlayer1InitStage(int X, int Y, string tool, int lineNum, const bool &isJoker) {
    if (!board[X][Y].empty()) {
        cout << "Error: Two or more pieces are positioned on the same location in line " << lineNum <<
             " of player 1's file" << endl;
        return false;
    }
    if (player1ToolCounters[tool] == 0) {
        cout << "Error: A piece type appears in file more than it's number in line " << lineNum <<
             " of player 1's file" << endl;
        return false;
    }
    player1ToolCounters[tool]--;
    board[X][Y] = tool;
    if(isJoker){
        int loc[2]={X,Y};
        player1JokerLocations.insert(loc);
    }
    return true;
}


bool
RPSGame::RPSGameUpdateBoardPlayer2InitStage(int X, int Y, string tool, int lineNum, map<int[2], string> &boardMap,
                                            const bool &isJoker) {
    int key[2] = {X, Y};
    if (boardMap.find(key) != boardMap.end()) { //position already contain piece
        cout << "Error: Two or more pieces are positioned on the same location in line " << lineNum <<
             " of player 2's file" << endl;
        return false;
    }
    if (player2ToolCounters[tool] == 0) {
        cout << "Error: A piece type appears in file more than it's number in line " << lineNum <<
             " of player 2's file" << endl;
        return false;
    }
    player2ToolCounters[tool]--;
    boardMap[key] = tool;
    if(isJoker){
        int loc[2]={X,Y};
        player2JokerLocations.insert(loc);
    }
    return true;
}


void RPSGame::RPSGameMergePlayer2BoardWithPlayer1Board(map<int[2], string> mapBoard) {
    map<int[2], string>::iterator it;
    for (it = mapBoard.begin(); it != mapBoard.end(); it++) {
        if (board[it->first[0]][it->first[1]].empty())
            board[it->first[0]][it->first[1]] = it->second;
        else {
            RPSGameFightOnPosition(it->first[0], it->first[1], it->second);
        }
    }
}


void RPSGame::RPSGameFightOnPosition(int X, int Y, const string &tool) {
    if (tool == board[X][Y]) {}
    else if (tool == "B") {
        player1ToolCounters[board[X][Y]]++;
        board[X][Y] = "b";
    } else if (board[X][Y] == "B") {
        transform(tool.begin(), tool.end(), tool.begin(), ::tolower);
        player2ToolCounters[tool]++;
    } else if (board[X][Y] == "F") {
        player1ToolCounters[board[X][Y]]++;
        board[X][Y] = tool;
        transform(board[X][Y].begin(), board[X][Y].end(), board[X][Y].begin(), ::tolower);
    } else if (tool == "F") {
        player2ToolCounters["f"]++;
    } else
        RPSGameRPSFight(X, Y, tool);

}

void RPSGame::RPSGameRPSFight(int X, int Y, const string &tool) {
    if (board[X][Y] == "R"  && tool == "P") {
        player1ToolCounters["R"]++;
        board[X][Y] = "p";
    } else if (board[X][Y] == "P"&& tool == "R") {
        player2ToolCounters["r"]++;
    } else if (board[X][Y] == "R" && tool == "S") {
        player2ToolCounters["s"]++;
    } else if (board[X][Y] == "S" && tool == "R") {
        player1ToolCounters["S"]++;
        board[X][Y] = "r";
    } else if (board[X][Y] == "P" && tool == "S") {
        player1ToolCounters["P"]++;
        board[X][Y] = "s";
    } else if (board[X][Y] == "S" && tool == "P") {
        player2ToolCounters["p"]++;
    }

}