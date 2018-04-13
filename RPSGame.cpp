
#include "RPSGame.h"

using namespace std;

RPSGame::RPSGame() : winner(0), player1Points(0), player2Points(0) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
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
    player2ToolCounters.erase("F");

}

RPSGame::Position::Position(int _X , int _Y) : Y(_Y), X(_X) {}

int RPSGame::RPSGameInitFileCheck(const string fileName, int player, map<string, int> &toolCounter) {

    ifstream fin(fileName);
    if (fin.fail()) {
        cout << "Error: There's no such file or directory, Player " << player << "'s file is not exist" << endl;
        return -1;
    }
    string lineToParse;
//    int X = 0, Y = 0;
//    bool isJoker;
    int lineNum = 1;
    int parserResult;
//    string tool;
    Move initMove;
    map<Position, string> player2BoardMap;
    while (true) {
        lineToParse = "";
        initMove.isJoker = false;
        getline(fin, lineToParse);
        if (lineToParse.empty()) {
            if (lineNum == 1) {
                cout << "Error: Init file of player " << player << " is empty" << endl;
                return -1;
            }
            break;
        }
        parserResult = RPSParserParseLineInit(lineToParse, initMove);
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
                    if (!RPSGameUpdateBoardPlayer1InitStage(initMove, lineNum)) {
                        return lineNum;
                    }
                } else {
                    if (!RPSGameUpdateBoardPlayer2InitStage(initMove, lineNum, player2BoardMap)) {
                        return lineNum;
                    }
                }
                lineNum++;
        }
    }
    if (player == 1) {
        if (toolCounter["F"] != 0) {
            cout << "Error: Missing Flags - Flags are not positioned according to their number in line " <<
                 lineNum - 1 << " of player " << player << "'s file" <<
                 endl;
            return -1;
        }
    }
    if (player == 2) {
        if (toolCounter["f"] != 0) {
            cout << "Error: Missing Flags - Flags are not positioned according to their number in line " <<
                 lineNum - 1 << " of player " << player << "'s file" <<
                 endl;
            return -1;
        }
        RPSGameMergePlayer2BoardWithPlayer1Board(player2BoardMap);
    }
    return 0;
}

bool RPSGame::RPSGameUpdateBoardPlayer1InitStage(const Move &initMove, const int &lineNum) {
    if (board[initMove.toY][initMove.toX] != "_") {
        cout << "Error: Two or more pieces are positioned on the same location in line " << lineNum <<
             " of player 1's file" << endl;
        return false;
    }
    if (player1ToolCounters[initMove.tool] == 0) {
        cout << "Error: A piece type appears in file more than it's number in line " << lineNum <<
             " of player 1's file" << endl;
        return false;
    }
    if (initMove.isJoker) {
        player1JokerLocations.insert(Position(initMove.joker_X, initMove.joker_Y));
        player1ToolCounters["J"]--;
        board[initMove.toY][initMove.toX] = initMove.joker_tool;
    } else {
        player1ToolCounters[initMove.tool]--;
        board[initMove.toY][initMove.toX] = initMove.tool;
    }


    return true;
}


bool RPSGame::RPSGameUpdateBoardPlayer2InitStage(Move &initMove, int lineNum, map<Position, string> &boardMap) {
    transform(initMove.tool.begin(), initMove.tool.end(), initMove.tool.begin(), ::tolower);
    Position key(initMove.toX, initMove.toY);
    if (boardMap.find(key) != boardMap.end()) { //position already contain piece
        cout << "Error: Two or more pieces are positioned on the same location in line " << lineNum <<
             " of player 2's file" << endl;
        return false;
    }
    if (player2ToolCounters[initMove.tool] == 0) {
        cout << "Error: A piece type appears in file more than it's number in line " << lineNum <<
             " of player 2's file" << endl;
        return false;
    }
    if (initMove.isJoker) {
        transform(initMove.joker_tool.begin(), initMove.joker_tool.end(), initMove.joker_tool.begin(), ::tolower);
        player2JokerLocations.insert(Position(initMove.joker_X, initMove.joker_Y));
        player2ToolCounters["j"]--;
        boardMap[key] = initMove.joker_tool;
    } else {

        player2ToolCounters[initMove.tool]--;
        boardMap[key] = initMove.tool;
    }


    return true;
}


void RPSGame::RPSGameMergePlayer2BoardWithPlayer1Board(map<Position, string> &mapBoard) {
    map<Position, string>::iterator it;
    Move initMove;

    for (it = mapBoard.begin(); it != mapBoard.end(); it++) {
        if (board[it->first.Y][it->first.X] == "_")
            board[it->first.Y][it->first.X] = it->second;
        else {
            initMove.isJoker = false;
            initMove.toY = initMove.fromY = it->first.Y;
            initMove.toX = initMove.fromX = it->first.X;
            initMove.player = 2;
            initMove.tool = it->second;
            Position pos(it->first.X, it->first.Y);
            if (player2JokerLocations.find(pos) != player2JokerLocations.end()) {
                initMove.isJoker = true;
            }
            RPSGameFightOnPosition(initMove);
        }
    }
}


void RPSGame::RPSGameSetMoveOnBoard(Move &newMove) {

    if (board[newMove.toY][newMove.toX] == "_") { //empty position
        board[newMove.toY][newMove.toX] = newMove.tool;

    } else {
        RPSGameFightOnPosition(newMove);
    }
    board[newMove.fromY][newMove.fromX] = "_";
}


void RPSGame::RPSGameFightOnPosition(Move &newMove) {
    string attackerTool = newMove.tool;
    string defenderTool = board[newMove.toY][newMove.toX];
    transform(attackerTool.begin(), attackerTool.end(), attackerTool.begin(), ::toupper);
    transform(defenderTool.begin(), defenderTool.end(), defenderTool.begin(), ::toupper);
    if ((attackerTool == defenderTool) || (attackerTool == "B") || (defenderTool == "B"))
        RPSGameRemoveBothPiecesFromGame(newMove);
    else if (newMove.tool == "F" || newMove.tool == "f")
        RPSGameFightAttackerLoses(newMove);
    else if (board[newMove.toY][newMove.toX] == "F" || board[newMove.toY][newMove.toX] == "f")
        RPSGameFightAttackerWins(newMove);
    else
        RPSGameRPSFight(newMove);
}


void RPSGame::RPSGameRemoveBothPiecesFromGame(Move &newMove) {
    Position sourcePos(newMove.fromX, newMove.fromY);
    Position destinationPos(newMove.toX, newMove.toY);
    if (newMove.player == 1) {
        if (newMove.isJoker) {
            player1JokerLocations.erase(sourcePos);
            player1ToolCounters["J"]++;
        } else {
            player1ToolCounters[newMove.tool]++;
        }
        if (player2JokerLocations.find(destinationPos) != player2JokerLocations.end()) {
            player2JokerLocations.erase(destinationPos);
            player2ToolCounters["j"]++;
        } else {
            player2ToolCounters[board[newMove.toY][newMove.toX]]++;
        }
    } else { //player 2
        if (newMove.isJoker) {
            player2JokerLocations.erase(sourcePos);
            player2ToolCounters["j"]++;
        } else {
            player2ToolCounters[newMove.tool]++;
        }
        if (player1JokerLocations.find(destinationPos) != player1JokerLocations.end()) {
            player1JokerLocations.erase(destinationPos);
            player1ToolCounters["J"]++;
        } else {
            player1ToolCounters[board[newMove.toY][newMove.toX]]++;
        }
    }
    board[newMove.toY][newMove.toX] = "_"; //update board
}


void RPSGame::RPSGameRPSFight(Move &newMove) {
    if ((board[newMove.toY][newMove.toX] == "R" || board[newMove.toY][newMove.toX] == "r") &&
        (newMove.tool == "P" || newMove.tool == "p")) {
        RPSGameFightAttackerWins(newMove);
    } else if ((board[newMove.toY][newMove.toX] == "P" || board[newMove.toY][newMove.toX] == "p") &&
               (newMove.tool == "R" || newMove.tool == "r")) {
        RPSGameFightAttackerLoses(newMove);
    } else if ((board[newMove.toY][newMove.toX] == "R" || board[newMove.toY][newMove.toX] == "r") &&
               (newMove.tool == "S" || newMove.tool == "s")) {
        RPSGameFightAttackerLoses(newMove);
    } else if ((board[newMove.toY][newMove.toX] == "S" || board[newMove.toY][newMove.toX] == "s") &&
               (newMove.tool == "R" || newMove.tool == "r")) {
        RPSGameFightAttackerWins(newMove);
    } else if ((board[newMove.toY][newMove.toX] == "P" || board[newMove.toY][newMove.toX] == "p") &&
               (newMove.tool == "S" || newMove.tool == "s")) {
        RPSGameFightAttackerWins(newMove);
    } else if ((board[newMove.toY][newMove.toX] == "S" || board[newMove.toY][newMove.toX] == "s") &&
               (newMove.tool == "P" || newMove.tool == "p")) {
        RPSGameFightAttackerLoses(newMove);
    }

}

void RPSGame::RPSGameFightAttackerWins(Move &newMove) {
    Position pos(newMove.toX, newMove.toY);
    if (newMove.player == 1) {
        if (player2JokerLocations.find(pos) != player2JokerLocations.end()) {
            player2JokerLocations.erase(player2JokerLocations.find(pos));
            player2ToolCounters["j"]++;
        } else {
            player2ToolCounters[board[newMove.toY][newMove.toX]]++;
        }
    } else {
        if (player1JokerLocations.find(pos) != player1JokerLocations.end()) {
            player1JokerLocations.erase(player1JokerLocations.find(pos));
            player1ToolCounters["J"]++;
        } else {
            player1ToolCounters[board[newMove.toY][newMove.toX]]++;
        }
    }
    if (newMove.player == 2)
        transform(newMove.tool.begin(), newMove.tool.end(), newMove.tool.begin(), ::tolower);
    board[newMove.toY][newMove.toX] = newMove.tool;
}


void RPSGame::RPSGameFightAttackerLoses(Move &newMove) {
    Position pos(newMove.fromX, newMove.fromY);
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
           && player1ToolCounters["S"] == S && player1ToolCounters["J"] == J;
}

bool RPSGame::RPSGameCheckIfPlayer2Lose() {
    if (player2ToolCounters["f"] == F)
        return true;
    return player2ToolCounters["r"] == R && player2ToolCounters["p"] == P
           && player2ToolCounters["s"] == S && player2ToolCounters["j"] == J;

}

int RPSGame::RPSGameMoveFileCheck(string fileName1, string fileName2, int &lineNum) {

    bool EOFile1 = false;
    bool EOFile2 = false;
    lineNum = 1;
    ifstream player1File(fileName1);
    ifstream player2File(fileName2);
    if (player1File.fail() && player2File.fail()) {
        cout << "Both moves files don't exist. It's a tie!" << endl;
        return -1;// both files does not exist - tie (need to write output message)
    }
    if (player1File.fail()) {
        return 1;
    } else if (player2File.fail()) {
        return 2;
    }
    string lineToParse;
    int parseResult;
    Move curMove;
    while (!EOFile1 || !EOFile2) {//while at least one of the files did not end

        curMove.joker_Y = -1; //reset this field, -1 is invalid position on board
        curMove.isJoker = false;

        if (!EOFile1) { //player 1's file not ended
            curMove.player = 1;
            getline(player1File, lineToParse);
            if (lineToParse.empty())
                EOFile1 = true; //player 1's file ended
            else {
                parseResult = RPSParserParseLineMove(lineToParse, curMove);
                if (!RPSGameCheckIfMoveIsValid(parseResult, 1, curMove, lineNum)) { //if the line move not valid
                    return 1; // "bad moves input file for player 1 line lineNum"
                }
                if (curMove.joker_Y != -1) { //the current move line contains a change of a joker
                    if (!RPSGameCheckIfChangeJokerPieceCommandIsValid(curMove)) {
                        RPSGamePrintErrorMessageBadChangeOfJokerPiece(lineNum, curMove); //print error message to screen
                        return 1; // "bad moves input file for player 1 line lineNum"
                    }
                }
                // otherwise the move was set
            }
        }
        if (RPSGameCheckIfPlayer1Lose() && RPSGameCheckIfPlayer2Lose())
            return 5;
        if (RPSGameCheckIfPlayer2Lose())
            return 3;
        if (RPSGameCheckIfPlayer1Lose())
            return 4;
        curMove.joker_Y = -1;

        if (!EOFile2) { // player 2's file not ended
            curMove.player = 2;
            getline(player2File, lineToParse);
            if (lineToParse.empty())
                EOFile2 = true;
            else {
                parseResult = RPSParserParseLineMove(lineToParse, curMove);
                if (!RPSGameCheckIfMoveIsValid(parseResult, 2, curMove, lineNum)) { //if the line move not valid
                    return 2; // "bad moves input file for player 2 line lineNum"
                }
                if (curMove.joker_Y != -1) { //the current move line contains a change of a joker
                    if (!RPSGameCheckIfChangeJokerPieceCommandIsValid(curMove)) {
                        RPSGamePrintErrorMessageBadChangeOfJokerPiece(lineNum, curMove); //print error message to screen
                        return 2; // "bad moves input file for player 2 line lineNum"
                    }
                }
            }
        }
        if (RPSGameCheckIfPlayer1Lose() && RPSGameCheckIfPlayer2Lose())
            return 5;
        if (RPSGameCheckIfPlayer2Lose())
            return 3;
        if (RPSGameCheckIfPlayer1Lose())
            return 4;
        lineNum++;// continue reading the next move lines in the file/s
    }
    return 0; // both files read without any errors - need to check the board
}


bool RPSGame::RPSGameCheckIfChangeJokerPieceCommandIsValid(Move &curMove) {
    Position pos(curMove.joker_X, curMove.joker_Y);
    if (curMove.player == 1) {
        if (player1JokerLocations.find(pos) != player1JokerLocations.end()) { //there is a joker in this pos
            board[curMove.joker_Y][curMove.joker_X] = curMove.joker_tool; // change to the new piece in the board
            return true;
        }
    } else { //player 2
        if (player2JokerLocations.find(pos) != player2JokerLocations.end()) { //there is a joker in this pos
            transform(curMove.joker_tool.begin(), curMove.joker_tool.end(), curMove.joker_tool.begin(), ::tolower);
            board[curMove.joker_Y][curMove.joker_X] = curMove.joker_tool; // change to the new piece in the board
            return true;
        }
    }
    return false; // invalid command of changing the joker piece
}


void RPSGame::RPSGamePrintErrorMessageBadChangeOfJokerPiece(int lineNum, Move curMove) {
    cout << "Error: Invalid change of joker piece in line " << lineNum << " of player "
         << curMove.player << "'s file" << endl;
}


bool RPSGame::RPSGameCheckIfMoveIsValid(int parseResult, int player, Move &curMove, int lineNum) {

    curMove.isJoker = false; // we will check later if it a joker

    switch (parseResult) {

        case 1:
            cout << "Error: Invalid number of arguments in line " << lineNum << " of player "
                 << player << "'s file" << endl;
            return false;

        case 2:
            cout << "Error: Invalid piece argument in line " << lineNum << " of player "
                 << player << "'s file" << endl;
            return false;

        case 3:
            cout << "Error: Invalid position on board in line " << lineNum << " of player "
                 << player << "'s file" << endl;
            return false;

        default:
            Position sourcePos(curMove.fromX, curMove.fromY);
            Position destinationPos(curMove.toX, curMove.toY);
            if (player == 1) {
                if (RPSGameIsPositionContainsPlayers1Piece(lineNum, curMove)) {
                    return false; //move is not valid
                }
                //else update the rest of the fields of curMove
                curMove.tool = board[curMove.fromY][curMove.fromX];
                curMove.player = 1;

                RPSGameSetMoveOnBoard(curMove);
                if (player1JokerLocations.find(sourcePos) != player1JokerLocations.end()) {

                    player1JokerLocations.erase(sourcePos);
                    player1JokerLocations.insert(destinationPos);
                }
            } else { //player == 2
                if (RPSGameIsPositionContainsPlayers2Piece(lineNum, curMove)) {
                    return false;
                }
                curMove.tool = board[curMove.fromY][curMove.fromX];
                transform(curMove.tool.begin(), curMove.tool.end(), curMove.tool.begin(), ::tolower);
                curMove.player = 2;
                RPSGameSetMoveOnBoard(curMove);
                if (player2JokerLocations.find(sourcePos) != player2JokerLocations.end()) {
                    player2JokerLocations.erase(sourcePos);
                    player2JokerLocations.insert(destinationPos);
                }

            }
    }
    return true;
}


bool RPSGame::RPSGameIsPositionContainsPlayers1Piece(const int &lineNum, const Move &curMove) {
    if ((board[curMove.fromY][curMove.fromX] != "R" && board[curMove.fromY][curMove.fromX] != "P" &&
         board[curMove.fromY][curMove.fromX] != "S" && board[curMove.fromY][curMove.fromX] != "F") ||
        (board[curMove.toY][curMove.toX] == "R" || board[curMove.toY][curMove.toX] == "P" ||
         board[curMove.toY][curMove.toX] == "S" || board[curMove.toY][curMove.toX] == "F" ||
         board[curMove.toY][curMove.toX] == "B")) {
        cout << "Error: Illegal move in line " << lineNum << " of player 1's file" << endl;
        return true;
    }
    return false; //all good
}


bool RPSGame::RPSGameIsPositionContainsPlayers2Piece(const int &lineNum, const Move &curMove) {
    if ((board[curMove.fromY][curMove.fromX] != "r" && board[curMove.fromY][curMove.fromX] != "p" &&
         board[curMove.fromY][curMove.fromX] != "s" && board[curMove.fromY][curMove.fromX] != "f")
        || (board[curMove.toY][curMove.toX] == "r" || board[curMove.toY][curMove.toX] == "p" ||
            board[curMove.toY][curMove.toX] == "s" || board[curMove.toY][curMove.toX] == "f" ||
            board[curMove.toY][curMove.toX] == "b")) {
        cout << "Error: Illegal move in line " << lineNum << " of player 2's file" << endl;
        return true;
    }
    return false; //all good
}


