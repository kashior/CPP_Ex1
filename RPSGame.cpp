
#include "RPSGame.h"

using namespace std;

RPSGame::RPSGame() : winner(0), player1Error(""), player2Error("") {
    for (int i=0 ; i<N ; i++){
        for (int j=0 ; j<M ; j++){
            board[i][j] = "_";
        }
    }
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


int RPSGame::RPSGameInitFileCheck(string fileName, int player,map<string, int> toolCounter) {

    ifstream fin(fileName);
    if (fin.fail()) {
        cout << "Error: There's no such file or directory, Player " << player << "'s file is not exist" << endl;
        return -1;
    }
    string lineToParse;
    int X = 0, Y = 0;
    bool isJoker;
    int lineNum = 1;
    int parserResult;
    string tool;
    map<Position,string> player2BoardMap;
    while (true) {
        isJoker = false;
        getline(fin, lineToParse);
        if (lineToParse.empty()) {
            if (lineNum == 1) {
                cout << "Error: Init file of player " << player << " is empty" << endl;
                return -1;
            }
            break;
        }
        parserResult = RPSParserParseLineInit(lineToParse, X, Y, tool, isJoker);
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
                    if (!RPSGameUpdateBoardPlayer1InitStage(X, Y, tool, lineNum, isJoker)) {
                        return lineNum;
                    }
                } else {
                    if (!RPSGameUpdateBoardPlayer2InitStage(X, Y, tool, lineNum, player2BoardMap, isJoker)) {
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
    if (isJoker) {
        Position loc = {X, Y};
        player1JokerLocations.insert(loc);
    }
    return true;
}


bool
RPSGame::RPSGameUpdateBoardPlayer2InitStage(int X, int Y, const string& tool, int lineNum, map<Position, string> &boardMap,
                                            const bool &isJoker) {
    Position key = {X, Y};
    if (boardMap.find(key)!= boardMap.end()) { //position already contain piece
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
    boardMap[key]=tool;
    if (isJoker) {
        Position loc = {X, Y};
        player2JokerLocations.insert(loc);
    }
    return true;
}


void RPSGame::RPSGameMergePlayer2BoardWithPlayer1Board(map<Position, string>& mapBoard) {
    map<Position, string>::iterator it;
    Move initMove;
    Position pos;
    for (it = mapBoard.begin(); it != mapBoard.end(); it++) {
        if (board[it->first.X][it->first.Y].empty())
            board[it->first.X][it->first.Y] = it->second;
        else {
            initMove.isJoker = false;
            initMove.fromX = it->first.X;
            initMove.fromY = it->first.Y;
            initMove.toX = it->first.X;
            initMove.toY = it->first.Y;
            initMove.player = 2;
            initMove.tool = it->second;
            pos = {it->first.X, it->first.Y};
            if (player2JokerLocations.find(pos) != player2JokerLocations.end()){
                initMove.isJoker = true;
            }
            RPSGameFightOnPosition(initMove);
        }
    }
}



void RPSGame::RPSGameSetMoveOnBoard(Move &newMove){
    Position sourcePos = {newMove.fromX, newMove.fromY};
    Position destinationPos = {newMove.toX, newMove.toY};
    if (board[newMove.toX][newMove.toY] == "_"){ //empty position
        board[newMove.toX][newMove.toY] = newMove.tool;
        if (newMove.isJoker){
            if (newMove.player == 1){
                player1JokerLocations.erase(sourcePos);
                player1JokerLocations.insert(destinationPos);
            }
            else{
                player2JokerLocations.erase(sourcePos);
                player2JokerLocations.insert(destinationPos);
            }
        }
    }
    else{
        RPSGameFightOnPosition(newMove);
    }
    board[newMove.fromX][newMove.fromY] = "_";
}


void RPSGame::RPSGameFightOnPosition(Move &newMove) {
    if ( (newMove.tool == board[newMove.toX][newMove.toY]) || (newMove.tool == "b") ||
            (board[newMove.toX][newMove.toY] == "B" || board[newMove.toX][newMove.toY] == "b") )
        RPSGameRemoveBothPiecesFromGame(newMove);
     // tool=="b" is possible in the init stage when merging player 2 to the board
    else if (newMove.tool == "F" || newMove.tool == "f")
        RPSGameFightAttackerLoses(newMove);
    else if (board[newMove.toX][newMove.toY] == "F" || board[newMove.toX][newMove.toY] == "f")
        RPSGameFightAttackerWins(newMove);
    else
        RPSGameRPSFight(newMove);
}


void RPSGame::RPSGameRemoveBothPiecesFromGame(Move &newMove){
    Position sourcePos = {newMove.fromX, newMove.fromY};
    Position destinationPos = {newMove.toX, newMove.toY};
    if (newMove.player == 1){
        if (newMove.isJoker){
            player1JokerLocations.erase(sourcePos);
            player1ToolCounters["J"]++;
        }
        else{
            player1ToolCounters[newMove.tool]++;
        }
        if (player2JokerLocations.find(destinationPos) != player2JokerLocations.end()){
            player2JokerLocations.erase(destinationPos);
            player2ToolCounters["j"]++;
        }
        else{
            player2ToolCounters[board[newMove.toX][newMove.toY]]++;
        }
    }
    else{ //player 2
        if (newMove.isJoker){
            player2JokerLocations.erase(sourcePos);
            player2ToolCounters["j"]++;
        }
        else{
            player2ToolCounters[newMove.tool]++;
        }
        if (player1JokerLocations.find(destinationPos) != player1JokerLocations.end()){
            player1JokerLocations.erase(destinationPos);
            player1ToolCounters["J"]++;
        }
        else{
            player1ToolCounters[board[newMove.toX][newMove.toY]]++;
        }
    }
    board[newMove.toX][newMove.toY] = "_"; //update board
}


void RPSGame::RPSGameRPSFight(Move &newMove) {
    if ((board[newMove.toX][newMove.toY] == "R" || board[newMove.toX][newMove.toY] == "r") && newMove.tool == "P") {
        RPSGameFightAttackerWins(newMove);
    } else if ((board[newMove.toX][newMove.toY] == "P" || board[newMove.toX][newMove.toY] == "p") && newMove.tool == "R") {
        RPSGameFightAttackerLoses(newMove);
    } else if ((board[newMove.toX][newMove.toY] == "R" || board[newMove.toX][newMove.toY] == "r") && newMove.tool == "S") {
        RPSGameFightAttackerLoses(newMove);
    } else if ((board[newMove.toX][newMove.toY] == "S" || board[newMove.toX][newMove.toY] == "s") && newMove.tool == "R") {
        RPSGameFightAttackerWins(newMove);
    } else if ((board[newMove.toX][newMove.toY] == "P" || board[newMove.toX][newMove.toY] == "p") && newMove.tool == "S") {
        RPSGameFightAttackerWins(newMove);
    } else if ((board[newMove.toX][newMove.toY] == "S" || board[newMove.toX][newMove.toY] == "s") && newMove.tool == "P") {
        RPSGameFightAttackerLoses(newMove);
    }

}

void RPSGame::RPSGameFightAttackerWins(Move &newMove) {
    Position pos = {newMove.toX, newMove.toY};
    if (newMove.player == 1) {
        if (player2JokerLocations.find(pos) != player2JokerLocations.end()) {
            player2JokerLocations.erase(player2JokerLocations.find(pos));
            player2ToolCounters["j"]++;
        } else {
            player2ToolCounters[board[newMove.toX][newMove.toY]]++;
        }
    } else {
        if (player1JokerLocations.find(pos) != player1JokerLocations.end()) {
            player1JokerLocations.erase(player1JokerLocations.find(pos));
            player1ToolCounters["J"]++;
        } else {
            player1ToolCounters[board[newMove.toX][newMove.toY]]++;
        }
    }
    if (newMove.player == 2)
        transform(newMove.tool.begin(), newMove.tool.end(), newMove.tool.begin(), ::tolower);
    board[newMove.toX][newMove.toY] = newMove.tool;
}


void RPSGame::RPSGameFightAttackerLoses(Move &newMove) {
    Position pos = {newMove.fromX, newMove.fromY};
    if (newMove.player == 1) {
        if (newMove.isJoker) {
            player1JokerLocations.erase(player1JokerLocations.find(pos));
            player1ToolCounters["J"]++;
        } else {
            player1ToolCounters[newMove.tool]++;
        }
    } else { //player 2
        if (newMove.isJoker) {
            player2JokerLocations.erase(player2JokerLocations.find(pos));
            player1ToolCounters["j"]++;
        } else {
            transform(newMove.tool.begin(), newMove.tool.end(), newMove.tool.begin(), ::tolower);
            player2ToolCounters[newMove.tool]++;
        }
    }
}


bool RPSGame::RPSGameCheckIfPlayer1Lose() {
    if (player1ToolCounters["F"] == F)
        return true;
    return player1ToolCounters["R"] == R && player1ToolCounters["P"] == P
           && player1ToolCounters["S"] == S && player1ToolCounters["B"] == B && player1ToolCounters["J"] == J;
}

bool RPSGame::RPSGameCheckIfPlayer2Lose() {

    if (player2ToolCounters["f"] == F)
        return true;
    return player2ToolCounters["r"] == R && player2ToolCounters["p"] == P
           && player2ToolCounters["s"] == S && player2ToolCounters["b"] == B && player2ToolCounters["j"] == J;

}


