
#include "RPSGame.h"

using namespace std;

RPSGame::RPSGame() : winner(0), player1Error(""), player2Error(""), player1Points(0), player2Points(0) {
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


int RPSGame::RPSGameInitFileCheck(string fileName, int player, map<string, int> &toolCounter) {

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
    if (board[initMove.toX][initMove.toY] != "_") {
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
        Position loc = {initMove.joker_X, initMove.joker_Y};
        player1JokerLocations.insert(loc);
        player1ToolCounters["J"]--;
    } else
        player1ToolCounters[initMove.tool]--;

    board[initMove.toX][initMove.toY] = initMove.tool;

    return true;
}


bool RPSGame::RPSGameUpdateBoardPlayer2InitStage(Move &initMove, int lineNum, map<Position, string> &boardMap) {
    transform(initMove.tool.begin(), initMove.tool.end(), initMove.tool.begin(), ::tolower);
    Position key = {initMove.toX, initMove.toY};
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
        Position loc = {initMove.joker_X, initMove.joker_Y};
        player2JokerLocations.insert(loc);
        player2ToolCounters["j"]--;
    } else
        player2ToolCounters[initMove.tool]--;
    boardMap[key] = initMove.tool;

    return true;
}


void RPSGame::RPSGameMergePlayer2BoardWithPlayer1Board(map<Position, string> &mapBoard) {
    map<Position, string>::iterator it;
    Move initMove;
    Position pos;
    for (it = mapBoard.begin(); it != mapBoard.end(); it++) {
        if (board[it->first.X][it->first.Y] == "_")
            board[it->first.X][it->first.Y] = it->second;
        else {
            initMove.isJoker = false;
            initMove.toX = initMove.fromX = it->first.X;
            initMove.toY = initMove.fromY = it->first.Y;
            initMove.player = 2;
            initMove.tool = it->second;
            pos = {it->first.X, it->first.Y};
            if (player2JokerLocations.find(pos) != player2JokerLocations.end()) {
                initMove.isJoker = true;
            }
            RPSGameFightOnPosition(initMove);
        }
    }
}


void RPSGame::RPSGameSetMoveOnBoard(Move &newMove) {
    Position sourcePos = {newMove.fromX, newMove.fromY};
    Position destinationPos = {newMove.toX, newMove.toY};
    if (board[newMove.toX][newMove.toY] == "_") { //empty position
        board[newMove.toX][newMove.toY] = newMove.tool;
        if (newMove.isJoker) {
            if (newMove.player == 1) {
                player1JokerLocations.erase(sourcePos);
                player1JokerLocations.insert(destinationPos);
            } else {
                player2JokerLocations.erase(sourcePos);
                player2JokerLocations.insert(destinationPos);
            }
        }
    } else {
        RPSGameFightOnPosition(newMove);
    }
    board[newMove.fromX][newMove.fromY] = "_";
}


void RPSGame::RPSGameFightOnPosition(Move &newMove) {
    string attackerTool = newMove.tool;
    string defenderTool = board[newMove.toX][newMove.toY];
    transform(attackerTool.begin(), attackerTool.end(), attackerTool.begin(), ::toupper);
    transform(defenderTool.begin(), defenderTool.end(), defenderTool.begin(), ::toupper);
    if ((attackerTool == defenderTool) || (attackerTool == "B") || (defenderTool == "B"))
        RPSGameRemoveBothPiecesFromGame(newMove);
    else if (newMove.tool == "F" || newMove.tool == "f")
        RPSGameFightAttackerLoses(newMove);
    else if (board[newMove.toX][newMove.toY] == "F" || board[newMove.toX][newMove.toY] == "f")
        RPSGameFightAttackerWins(newMove);
    else
        RPSGameRPSFight(newMove);
}


void RPSGame::RPSGameRemoveBothPiecesFromGame(Move &newMove) {
    Position sourcePos = {newMove.fromX, newMove.fromY};
    Position destinationPos = {newMove.toX, newMove.toY};
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
            player2ToolCounters[board[newMove.toX][newMove.toY]]++;
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
            player1ToolCounters[board[newMove.toX][newMove.toY]]++;
        }
    }
    board[newMove.toX][newMove.toY] = "_"; //update board
}


void RPSGame::RPSGameRPSFight(Move &newMove) {
    if ((board[newMove.toX][newMove.toY] == "R" || board[newMove.toX][newMove.toY] == "r") &&
        (newMove.tool == "P" || newMove.tool == "p")) {
        RPSGameFightAttackerWins(newMove);
    } else if ((board[newMove.toX][newMove.toY] == "P" || board[newMove.toX][newMove.toY] == "p") &&
               (newMove.tool == "R" || newMove.tool == "r")) {
        RPSGameFightAttackerLoses(newMove);
    } else if ((board[newMove.toX][newMove.toY] == "R" || board[newMove.toX][newMove.toY] == "r") &&
               (newMove.tool == "S" || newMove.tool == "s")) {
        RPSGameFightAttackerLoses(newMove);
    } else if ((board[newMove.toX][newMove.toY] == "S" || board[newMove.toX][newMove.toY] == "s") &&
               (newMove.tool == "R" || newMove.tool == "r")) {
        RPSGameFightAttackerWins(newMove);
    } else if ((board[newMove.toX][newMove.toY] == "P" || board[newMove.toX][newMove.toY] == "p") &&
               (newMove.tool == "S" || newMove.tool == "s")) {
        RPSGameFightAttackerWins(newMove);
    } else if ((board[newMove.toX][newMove.toY] == "S" || board[newMove.toX][newMove.toY] == "s") &&
               (newMove.tool == "P" || newMove.tool == "p")) {
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
           && player1ToolCounters["S"] == S && player1ToolCounters["J"] == J;
}

bool RPSGame::RPSGameCheckIfPlayer2Lose() {

    if (player2ToolCounters["f"] == F)
        return true;
    return player2ToolCounters["r"] == R && player2ToolCounters["p"] == P
           && player2ToolCounters["s"] == S && player2ToolCounters["j"] == J;

}


/**
 * Reads both moves files of the players simultaneously, begins with player 1's file.
 * It parses each line in a time, checks if it is a valid move, and if it is valid, sets the move on the game
 * board.
 *
 * @param fileName1 - player 1's moves file
 * @param fileName2 - player 2's moves file
 * @param lineNum - if there will be an error in one of the files, it will be in line number lineNum
 * @return -1 - if both files are not exist
 *         0 - if both were read without any errors
 *         1 - if there was an error in player 1's file
 *         2 - if there was an error in player 2's file
 *         3 - if player 1 wins
 *         4 - if player 2 wins
 *         5 - tie
 *
 */
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
//    int validationResult;
    Move curMove;
    while (!EOFile1 || !EOFile2) {//while at least one of the files did not end

        curMove.joker_X = -1; //reset this field, -1 is invalid position on board

        if (!EOFile1) { //player 1's file not ended
            getline(player1File, lineToParse);
            if (lineToParse.empty())
                EOFile1 = true; //player 1's file ended
            else {
                parseResult = RPSParserParseLineMove(lineToParse, curMove);
                if (!RPSGameCheckIfMoveIsValid(parseResult, 1, curMove, lineNum)) { //if the line move not valid
                    return 1; // "bad moves input file for player 1 line lineNum"
                }
                if (curMove.joker_X != -1) { //the current move line contains a change of a joker
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
        curMove.joker_X = -1;

        if (!EOFile2) { // player 2's file not ended
            getline(player2File, lineToParse);
            if (lineToParse.empty())
                EOFile2 = true;
            else {
                parseResult = RPSParserParseLineMove(lineToParse, curMove);
                if (!RPSGameCheckIfMoveIsValid(parseResult, 2, curMove, lineNum)) { //if the line move not valid
                    return 2; // "bad moves input file for player 2 line lineNum"
                }
                if (curMove.joker_X != -1) { //the current move line contains a change of a joker
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


/**
 * In case a move line contains also a change of a joker piece, this function checks if indeed
 * <JOKER_X> <JOKER_Y> is a position containing a joker. If the position is valid,
 * we update the position on the board to contain <NEW_REP>.
 *
 * @param curMove - the move we currently check and want to set
 * @return false - if position does not contain a joker of the current player
 *         true - otherwise
 */
bool RPSGame::RPSGameCheckIfChangeJokerPieceCommandIsValid(Move &curMove) {
    Position pos = {curMove.joker_X, curMove.joker_Y};
    if (curMove.player == 1) {
        if (player1JokerLocations.find(pos) != player1JokerLocations.end()) { //there is a joker in this pos
            board[curMove.joker_X][curMove.joker_Y] = curMove.joker_tool; // change to the new piece in the board
            return true;
        }
    } else { //player 2
        if (player2JokerLocations.find(pos) != player2JokerLocations.end()) { //there is a joker in this pos
            transform(curMove.joker_tool.begin(), curMove.joker_tool.end(), curMove.joker_tool.begin(), ::tolower);
            board[curMove.joker_X][curMove.joker_Y] = curMove.joker_tool; // change to the new piece in the board
            return true;
        }
    }
    return false; // invalid command of changing the joker piece
}


/**
 * In case RPSGameCheckIfChangeJokerPieceCommandIsValid(Move &curMove)==false,
 * an error message will be printed to the screen by this function.
 *
 * @param lineNum - the current line num checked in a file
 * @param curMove - the current move in the line
 */
void RPSGame::RPSGamePrintErrorMessageBadChangeOfJokerPiece(int lineNum, Move curMove) {
    cout << "Error: Invalid change of joker piece in line " << lineNum << " of player "
         << curMove.player << "'s file" << endl;
}


/**
 * According to the parseResult this function print an error message (in case the parser found a mistake
 * in the move line) or checks a few more things to check if the move line is completely valid, if it indeed
 * valid the move will be set on the board.
 *
 * @param parseResult - of the parser
 * @param player - the current player
 * @param curMove - the current move we check its validation
 * @param lineNum - the line number in the file
 * @return true - if the move line is valid
 *         false - otherwise
 */
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
            if (player == 1) {
                if (RPSGameIsPositionContainsPlayers1Piece(curMove.fromX, curMove.fromY, curMove.toX, curMove.toY,
                                                           lineNum)) {
                    return false; //move is not valid
                }
                //else update the rest of the fields of curMove
                curMove.tool = board[curMove.fromX][curMove.fromY];
                curMove.player = 1;
                Position pos = {curMove.fromX, curMove.fromY};
                if (player1JokerLocations.find(pos) != player1JokerLocations.end())
                    curMove.isJoker = true;
                RPSGameSetMoveOnBoard(curMove);
            } else { //player == 2
                if (RPSGameIsPositionContainsPlayers2Piece(curMove.fromX, curMove.fromY, curMove.toX, curMove.toY,
                                                           lineNum)) {
                    return false;
                }
                curMove.tool = board[curMove.fromX][curMove.fromY];
                transform(curMove.tool.begin(), curMove.tool.end(), curMove.tool.begin(), ::tolower);
                curMove.player = 2;
                Position pos = {curMove.fromX, curMove.fromY};
                if (player2JokerLocations.find(pos) != player2JokerLocations.end())
                    curMove.isJoker = true;
                RPSGameSetMoveOnBoard(curMove);
            }
    }
    return true;
}


/**
 * Checks if source position on board contains player 1's piece (capital letter), and if destination
 * position is empty or if it contains player 2's piece (not capital letter).
 *
 * @param fromX - X coordinate of source
 * @param fromY - Y coordinate of source
 * @param toX - X coordinate of destination
 * @param toY - Y coordinate of destination
 * @param lineNum - the line number in the moves file
 * @return false - if source position on board contains player 1's piece, and if destination
 *         position is empty or if it contains player 2's piece (valid move)
 *         true - otherwise (invalid move)
 */
bool RPSGame::RPSGameIsPositionContainsPlayers1Piece(int fromX, int fromY, int toX, int toY, int lineNum) {
    if ((board[fromX][fromY] != "R" && board[fromX][fromY] != "P" &&
         board[fromX][fromY] != "S" && board[fromX][fromY] != "F") ||
        (board[toX][toY] == "R" || board[toX][toY] == "P" ||
         board[toX][toY] == "S" || board[toX][toY] == "F" ||
         board[toX][toY] == "B")) {
        cout << "Error: Illegal move in line " << lineNum << "of player 1's file" << endl;
        return true;
    }
    return false; //all good
}


/**
 * Checks if source position on board contains player 2's piece (lower letter), and if destination
 * position is empty or if it contains player 1's piece (not lower letter).
 *
 * @param fromX - X coordinate of source
 * @param fromY - Y coordinate of source
 * @param toX - X coordinate of destination
 * @param toY - Y coordinate of destination
 * @param lineNum - the line number in the moves file
 * @return false - if source position on board contains player 2's piece, and if destination
 *         position is empty or if it contains player 1's piece (valid move)
 *         true - otherwise (invalid move)
 */
bool RPSGame::RPSGameIsPositionContainsPlayers2Piece(int fromX, int fromY, int toX, int toY, int lineNum) {
    if ((board[fromX][fromY] != "r" && board[fromX][fromY] != "p" && board[fromX][fromY] != "s" &&
         board[fromX][fromY] != "f")
        || (board[toX][toY] == "r" || board[toX][toY] == "p" || board[toX][toY] == "s" || board[toX][toY] == "f"
            || board[toX][toY] == "b")) {
        cout << "Error: Illegal move in line " << lineNum << "of player 2's file" << endl;
        return true;
    }
    return false; //all good
}

