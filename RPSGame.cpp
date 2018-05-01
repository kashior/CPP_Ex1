
#include "RPSGame.h"
#include "RPSFilePlayerAlgorithm.h"
#include "RPSAutoPlayerAlgorithm.h"
#include "RPSBoard.h"
#include "RPSFightInfo.h"


RPSGame::RPSGame()  {
//    if(!isPlayer1Auto)
//        player1= make_unique<RPSFilePlayerAlgorithm>(1,"/");
//    else
//        player1=make_unique<RPSAutoPlayerAlgorithm>(1,"/");
//    if(!isPlayer2Auto)
//        player2= make_unique<RPSFilePlayerAlgorithm>(2,"/");
//    else
//        player2=make_unique<RPSAutoPlayerAlgorithm>(2,"/");
    board=make_unique<RPSBoard>();
    movesCounter=0;
}


bool RPSGame::RPSGameInitFileCheck(unique_ptr<PlayerAlgorithm> & playerAlg, int playerNum) {
    if (playerNum == 1){
        playerAlg->getInitialPositions(1,initPosPlayer1);
        if (initPosPlayer1.empty()) //empty file
            return false;
    }
    else{
        playerAlg->getInitialPositions(2,initPosPlayer2);
        if(initPosPlayer2.empty())
            return false;
    }
    return true;
}


bool RPSGame::UpdateBoardPlayer1InitStage(int &lineNum) {
    int x,y;
    for (unique_ptr<PiecePosition> pos : initPosPlayer1){
        x = pos->getPosition().getX();
        y = pos->getPosition().getY();
        if (board->board[y][x] != ' ') {
            cout << "Error: Two or more pieces are positioned on the same location in line " << lineNum <<
             " of player 1's positioning" << endl;
            return false;
        }
        if (player1->playerToolCounters[pos->getPiece()] == 0) {
            cout << "Error: A piece type appears more than it's number in line " << lineNum <<
                 " of player 1's positioning" << endl;
            return false;
        }
        if (pos->getJokerRep() !=  '#') {
            player1.playerJokers.push_back(make_unique<RPSPiecePosition>(pos->getPosition(), pos->getPiece(), pos->getJokerRep()));
            board->board[y][x] = pos->getJokerRep();
        }
        else
            board->board[y][x] = pos->getPiece();
        player1->playerToolCounters[pos->getPiece()]--;
        lineNum++;
    }
    return true;
}

bool
RPSGame::UpdateBoardPlayer2InitStage(int &lineNum, vector<unique_ptr<FightInfo>> &fights1, vector<unique_ptr<FightInfo>>&fights2) {
    int x,y;
    char piece;
    for (unique_ptr<PiecePosition> pos : initPosPlayer2){
        x = pos->getPosition().getX();
        y = pos->getPosition().getY();
        piece = (char)tolower(pos->getPiece());
        if ( (int) board->board[y][x] > 96) {
            cout << "Error: Two or more pieces are positioned on the same location in line " << lineNum <<
                 " of player 2's positioning" << endl;
            return false;
        }
        if (player2->playerToolCounters[pos->getPiece()] == 0) {
            cout << "Error: A piece type appears more than it's number in line " << lineNum <<
                 " of player 2's positioning" << endl;
            return false;
        }
        if (pos->getJokerRep() !=  '#')
            player2->playerJokers.push_back(make_unique<RPSPoint>(pos->getPosition());

        player2->playerToolCounters[pos->getPiece()]--;
        if (board->board[y][x] == ' ')
            board->board[y][x] = pos->getJokerRep()=='#' ? piece : (char) tolower(pos->getJokerRep());
        else{
            RPSMove initMove(RPSPoint(-1,-1), pos->getPosition(), piece, 2);
            FightOnPosition(initMove, fights1, fights2);
        }
        lineNum++;
    }
    return true;
}
//
//
//

void RPSGame::FightOnPosition(RPSMove curMove, vector<unique_ptr<FightInfo>> &fights1, vector<unique_ptr<FightInfo>> &fights2) {
    char attackerTool = curMove.getPlayer() == 1 ? curMove.getPiece(): (char)tolower(curMove.getPiece());
    char defenderTool = board->board[curMove.getTo().getY()][curMove.getTo().getX()];
    if ((attackerTool == defenderTool) || (attackerTool == 'B') || (defenderTool == 'B') ||
            (attackerTool == 'b') || (defenderTool == 'b') )
        RemoveBothPiecesFromGame(curMove, fights1, fights2);
    else if (curMove.getPiece() == 'f' ) //possible only in init stage
        RPSGameFightAttackerLoses(newMove);
    else if (defenderTool == 'F' || defenderTool == 'f')
        FightAttackerWins(newMove, <#initializer#>, <#initializer#>);
    else
        RPSGameRPSFight(newMove);
}

// remove both playertoolscounter and joker locations if there is , update board
void
RPSGame::RemoveBothPiecesFromGame(RPSMove curMove, vector<unique_ptr<FightInfo>> &fights1, vector<unique_ptr<FightInfo>> &fights2) {

    char attackedPiece = (char) toupper(board->board[curMove.getTo().getY()][curMove.getTo().getX()]);
    char attackerPiece = (char) toupper(curMove.getPiece());

    if (curMove.getPlayer() == 1) {
        RemoveToolsFromVectors(player1, curMove,  attackerPiece);
        RemoveToolsFromVectors(player2, curMove,  attackedPiece);
    } else {
        RemoveToolsFromVectors(player2, curMove,  attackerPiece);
        RemoveToolsFromVectors(player1, curMove,  attackedPiece);
    }

    updateFightVectors(0, curMove, fights1, fights2);

    board->board[curMove.getTo().getY()][curMove.getTo().getX()] = ' ';
    if (curMove.getFrom().getX() != -1) //not in init stage, need to change 'from' position on board to ' '
        board->board[curMove.getFrom().getY()][curMove.getFrom().getX()] = ' ';
}




void RPSGame::RPSGameRPSFight(RPSParser::OldMove &newMove) {
    if ((board[newMove.toY][newMove.toX] == "R" || board[newMove.toY][newMove.toX] == "r") &&
        (newMove.tool == "P" || newMove.tool == "p")) {
        FightAttackerWins(newMove, <#initializer#>, <#initializer#>);
    } else if ((board[newMove.toY][newMove.toX] == "P" || board[newMove.toY][newMove.toX] == "p") &&
               (newMove.tool == "R" || newMove.tool == "r")) {
        RPSGameFightAttackerLoses(newMove);
    } else if ((board[newMove.toY][newMove.toX] == "R" || board[newMove.toY][newMove.toX] == "r") &&
               (newMove.tool == "S" || newMove.tool == "s")) {
        RPSGameFightAttackerLoses(newMove);
    } else if ((board[newMove.toY][newMove.toX] == "S" || board[newMove.toY][newMove.toX] == "s") &&
               (newMove.tool == "R" || newMove.tool == "r")) {
        FightAttackerWins(newMove, <#initializer#>, <#initializer#>);
    } else if ((board[newMove.toY][newMove.toX] == "P" || board[newMove.toY][newMove.toX] == "p") &&
               (newMove.tool == "S" || newMove.tool == "s")) {
        FightAttackerWins(newMove, <#initializer#>, <#initializer#>);
    } else if ((board[newMove.toY][newMove.toX] == "S" || board[newMove.toY][newMove.toX] == "s") &&
               (newMove.tool == "P" || newMove.tool == "p")) {
        RPSGameFightAttackerLoses(newMove);
    }

}

void RPSGame::FightAttackerWins(RPSMove curMove, vector<unique_ptr<FightInfo>> &fights1, vector<unique_ptr<FightInfo>>  &fights2) {
    char attackedPiece= (char) toupper(board->board[curMove.getTo().getY()][curMove.getTo().getX()]);
    if (curMove.getPlayer() == 1)
        RemoveToolsFromVectors(player2, curMove, attackedPiece);
    else
        RemoveToolsFromVectors(player1, curMove, attackedPiece);
    updateFightVectors(curMove.getPlayer(), curMove, fights1, fights2);
    board->board[curMove.getTo().getY()][curMove.getTo().getY()] = curMove.getPiece();
    if (curMove.getFrom().getX() != -1) //not init stage
        board->board[curMove.getFrom().getY()][curMove.getFrom().getX()] = ' ';
}


void RPSGame::FightAttackerLoses(RPSMove curMove, vector<unique_ptr<FightInfo>> &fights1, vector<unique_ptr<FightInfo>>  &fights2) {
    char attackedPiece= (char) toupper(curMove.getPiece());
    if (curMove.getPlayer() == 1)
        RemoveToolsFromVectors(player1, curMove, attackedPiece);
    else
        RemoveToolsFromVectors(player2, curMove, attackedPiece);
    updateFightVectors(curMove.getPlayer()==1 ? 2 : 1 , curMove, fights1, fights2);
    if (curMove.getFrom().getX() != -1) //not init stage
        board->board[curMove.getFrom().getY()][curMove.getFrom().getX()] = ' ';
}

void RPSGame::RemoveToolsFromVectors(unique_ptr<RPSPlayerAlgorithm> &player, RPSMove curMove, char pieceToRemove) {

    auto it=find_if(player->playerJokers.begin(), player->playerJokers.end(),
                    [&](unique_ptr<Point>& obj){return obj->getX()==curMove.getTo().getX()
                                                       && obj->getY()==curMove.getTo().getY();});
    if (it != player->playerJokers.end()){
        player->playerToolCounters['J']++;
        player->playerJokers.erase(it);
    }
    else{
        player->playerToolCounters[pieceToRemove]++;
    }

}

void RPSGame::updateFightVectors(int winner, RPSMove curMove, vector<unique_ptr<FightInfo>> &fights1,
                                 vector<unique_ptr<FightInfo>> &fights2) {
    unique_ptr<FightInfo> fight1;
    unique_ptr<FightInfo> fight2;
    char attackedPiece= (char) toupper(board->board[curMove.getTo().getY()][curMove.getTo().getX()]);
    char attackerPiece = (char) toupper(curMove.getPiece());

    if (curMove.getPlayer() == 1){
        fight1 = make_unique<RPSFightInfo>(winner, attackedPiece, curMove.getTo());
        fight2 = make_unique<RPSFightInfo>(winner, attackerPiece, curMove.getTo());
    }
    else{
        fight1 = make_unique<RPSFightInfo>(winner, attackerPiece, curMove.getTo());
        fight2 = make_unique<RPSFightInfo>(winner, attackedPiece, curMove.getTo());
    }
    fights1.push_back(fight1);
    fights2.push_back(fight2);
}



//
//bool RPSGame::RPSGameCheckIfPlayer1Lose() {
//    if (player1ToolCounters["F"] == F)
//        return true;
//    return player1ToolCounters["R"] == R && player1ToolCounters["P"] == P
//           && player1ToolCounters["S"] == S && player1ToolCounters["J"] == J;
//}
//
//bool RPSGame::RPSGameCheckIfPlayer2Lose() {
//    if (player2ToolCounters["f"] == F)
//        return true;
//    return player2ToolCounters["r"] == R && player2ToolCounters["p"] == P
//           && player2ToolCounters["s"] == S && player2ToolCounters["j"] == J;
//
//}
//
//int RPSGame::RPSGameMoveFileCheck(string fileName1, string fileName2, int &lineNum) {
//
//    bool EOFile1 = false;
//    bool EOFile2 = false;
//    lineNum = 1;
//    ifstream player1File(fileName1);
//    ifstream player2File(fileName2);
//    if (player1File.fail() && player2File.fail()) {
//        cout << "Both moves files don't exist. It's a tie!" << endl;
//        return -1;// both files does not exist - tie (need to write output message)
//    }
//    if (player1File.fail()) {
//        return 1;
//    } else if (player2File.fail()) {
//        return 2;
//    }
//    string lineToParse;
//    int parseResult;
//    RPSParser::OldMove curMove;
//    while (!EOFile1 || !EOFile2) {//while at least one of the files did not end
//
//        curMove.joker_Y = -1; //reset this field, -1 is invalid position on board
//        curMove.isJoker = false;
//
//        if (!EOFile1) { //player 1's file not ended
//            curMove.player = 1;
//            getline(player1File, lineToParse);
//            if (lineToParse.empty())
//                EOFile1 = true; //player 1's file ended
//            else {
//                parseResult = RPSParser::parseLineMove(lineToParse, curMove);
//                if (!RPSGameCheckIfMoveIsValid(parseResult, 1, curMove, lineNum)) { //if the line move not valid
//                    return 1; // "bad moves input file for player 1 line lineNum"
//                }
//                if (curMove.joker_Y != -1) { //the current move line contains a change of a joker
//                    if (!RPSGameCheckIfChangeJokerPieceCommandIsValid(curMove)) {
//                        RPSGamePrintErrorMessageBadChangeOfJokerPiece(lineNum, curMove); //print error message to screen
//                        return 1; // "bad moves input file for player 1 line lineNum"
//                    }
//                }
//                // otherwise the move was set
//            }
//        }
//        if (RPSGameCheckIfPlayer1Lose() && RPSGameCheckIfPlayer2Lose())
//            return 5;
//        if (RPSGameCheckIfPlayer2Lose())
//            return 3;
//        if (RPSGameCheckIfPlayer1Lose())
//            return 4;
//        curMove.joker_Y = -1;
//
//        if (!EOFile2) { // player 2's file not ended
//            curMove.player = 2;
//            getline(player2File, lineToParse);
//            if (lineToParse.empty())
//                EOFile2 = true;
//            else {
//                parseResult = RPSParser::parseLineMove(lineToParse, curMove);
//                if (!RPSGameCheckIfMoveIsValid(parseResult, 2, curMove, lineNum)) { //if the line move not valid
//                    return 2; // "bad moves input file for player 2 line lineNum"
//                }
//                if (curMove.joker_Y != -1) { //the current move line contains a change of a joker
//                    if (!RPSGameCheckIfChangeJokerPieceCommandIsValid(curMove)) {
//                        RPSGamePrintErrorMessageBadChangeOfJokerPiece(lineNum, curMove); //print error message to screen
//                        return 2; // "bad moves input file for player 2 line lineNum"
//                    }
//                }
//            }
//        }
//        if (RPSGameCheckIfPlayer1Lose() && RPSGameCheckIfPlayer2Lose())
//            return 5;
//        if (RPSGameCheckIfPlayer2Lose())
//            return 3;
//        if (RPSGameCheckIfPlayer1Lose())
//            return 4;
//        lineNum++;// continue reading the next move lines in the file/s
//    }
//    return 0; // both files read without any errors - need to check the board
//}
//
//
//bool RPSGame::RPSGameCheckIfChangeJokerPieceCommandIsValid(RPSParser::OldMove &curMove) {
//    RPSPiecePosition pos(curMove.joker_X, curMove.joker_Y);
//    if (curMove.player == 1) {
//        if (player1JokerLocations.find(pos) != player1JokerLocations.end()) { //there is a joker in this pos
//            board[curMove.joker_Y][curMove.joker_X] = curMove.joker_tool; // change to the new piece in the board
//            return true;
//        }
//    } else { //player 2
//        if (player2JokerLocations.find(pos) != player2JokerLocations.end()) { //there is a joker in this pos
//            transform(curMove.joker_tool.begin(), curMove.joker_tool.end(), curMove.joker_tool.begin(), ::tolower);
//            board[curMove.joker_Y][curMove.joker_X] = curMove.joker_tool; // change to the new piece in the board
//            return true;
//        }
//    }
//    return false; // invalid command of changing the joker piece
//}
//
//
//void RPSGame::RPSGamePrintErrorMessageBadChangeOfJokerPiece(int lineNum,const RPSParser::OldMove& curMove) {
//    cout << "Error: Invalid change of joker piece in line " << lineNum << " of player "
//         << curMove.player << "'s file" << endl;
//}
//
//
//bool RPSGame::RPSGameCheckIfMoveIsValid(int parseResult, int player, RPSParser::OldMove &curMove, int lineNum) {
//
//    curMove.isJoker = false; // we will check later if it a joker
//
//    switch (parseResult) {
//
//        case 1:
//            cout << "Error: Invalid number of arguments in line " << lineNum << " of player "
//                 << player << "'s file" << endl;
//            return false;
//
//        case 2:
//            cout << "Error: Invalid piece argument in line " << lineNum << " of player "
//                 << player << "'s file" << endl;
//            return false;
//
//        case 3:
//            cout << "Error: Invalid position on board in line " << lineNum << " of player "
//                 << player << "'s file" << endl;
//            return false;
//
//        default:
//            RPSPiecePosition sourcePos(curMove.fromX, curMove.fromY);
//            RPSPiecePosition destinationPos(curMove.toX, curMove.toY);
//            if (player == 1) {
//                if (RPSGameIsPositionContainsPlayers1Piece(lineNum, curMove)) {
//                    return false; //move is not valid
//                }
//                //else update the rest of the fields of curMove
//                curMove.tool = board[curMove.fromY][curMove.fromX];
//                curMove.player = 1;
//
//                RPSGameSetMoveOnBoard(curMove);
//                if (player1JokerLocations.find(sourcePos) != player1JokerLocations.end()) {
//
//                    player1JokerLocations.erase(sourcePos);
//                    player1JokerLocations.insert(destinationPos);
//                }
//            } else { //player == 2
//                if (RPSGameIsPositionContainsPlayers2Piece(lineNum, curMove)) {
//                    return false;
//                }
//                curMove.tool = board[curMove.fromY][curMove.fromX];
//                transform(curMove.tool.begin(), curMove.tool.end(), curMove.tool.begin(), ::tolower);
//                curMove.player = 2;
//                RPSGameSetMoveOnBoard(curMove);
//                if (player2JokerLocations.find(sourcePos) != player2JokerLocations.end()) {
//                    player2JokerLocations.erase(sourcePos);
//                    player2JokerLocations.insert(destinationPos);
//                }
//
//            }
//    }
//    return true;
//}
//
//
//bool RPSGame::RPSGameIsPositionContainsPlayers1Piece(const int &lineNum, const RPSParser::OldMove &curMove) {
//    if ((board[curMove.fromY][curMove.fromX] != "R" && board[curMove.fromY][curMove.fromX] != "P" &&
//         board[curMove.fromY][curMove.fromX] != "S" && board[curMove.fromY][curMove.fromX] != "F") ||
//        (board[curMove.toY][curMove.toX] == "R" || board[curMove.toY][curMove.toX] == "P" ||
//         board[curMove.toY][curMove.toX] == "S" || board[curMove.toY][curMove.toX] == "F" ||
//         board[curMove.toY][curMove.toX] == "B")) {
//        cout << "Error: Illegal move in line " << lineNum << " of player 1's file" << endl;
//        return true;
//    }
//    return false; //all good
//}
//
//
//bool RPSGame::RPSGameIsPositionContainsPlayers2Piece(const int &lineNum, const RPSParser::OldMove &curMove) {
//    if ((board[curMove.fromY][curMove.fromX] != "r" && board[curMove.fromY][curMove.fromX] != "p" &&
//         board[curMove.fromY][curMove.fromX] != "s" && board[curMove.fromY][curMove.fromX] != "f")
//        || (board[curMove.toY][curMove.toX] == "r" || board[curMove.toY][curMove.toX] == "p" ||
//            board[curMove.toY][curMove.toX] == "s" || board[curMove.toY][curMove.toX] == "f" ||
//            board[curMove.toY][curMove.toX] == "b")) {
//        cout << "Error: Illegal move in line " << lineNum << " of player 2's file" << endl;
//        return true;
//    }
//    return false; //all good
//}


