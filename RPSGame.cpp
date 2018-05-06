
#include "RPSGame.h"
#include "RPSFilePlayerAlgorithm.h"
#include "RPSAutoPlayerAlgorithm.h"
#include "RPSBoard.h"
#include "RPSFightInfo.h"


RPSGame::RPSGame()  {
//    board = make_unique<RPSBoard>();
    movesCounter = 0;
}



bool RPSGame::UpdateBoardPlayer1InitStage(int &lineNum, vector<unique_ptr<PiecePosition>> & playersPositioning,
                                          unique_ptr<RPSPlayerAlgorithm> &playerAlg) {
    int x,y;
    for (unique_ptr<PiecePosition> pos : playersPositioning){
        x = pos->getPosition().getX();
        y = pos->getPosition().getY();
        if (board.board[y][x] != ' ') {
            cout << "Error: Two or more pieces are positioned on the same location in line " << lineNum <<
             " of player 1's positioning" << endl;
            return false;
        }
        if (playerAlg->playerToolCounters[pos->getPiece()] == 0) {
            cout << "Error: A piece type appears more than it's number in line " << lineNum <<
                 " of player 1's positioning" << endl;
            return false;
        }
        if (pos->getJokerRep() !=  '#') {
            playerAlg->addJokerPositionToVector(pos);
            board.board[y][x] = pos->getJokerRep();
        }
        else
            board.board[y][x] = pos->getPiece();
        playerAlg->playerToolCounters[pos->getPiece()]--;
        lineNum++;
    }
    return true;
}


bool RPSGame::UpdateBoardPlayer2InitStage(int &lineNum, vector<unique_ptr<PiecePosition>> &playersPositioning,
                                          unique_ptr<RPSPlayerAlgorithm> &player1Alg,
                                          unique_ptr<RPSPlayerAlgorithm> &player2Alg,
                                          vector<unique_ptr<FightInfo>> &fights) {
    int x,y;
    char piece;
    for (unique_ptr<PiecePosition> pos : playersPositioning){
        x = pos->getPosition().getX();
        y = pos->getPosition().getY();
        piece = (char)tolower(pos->getPiece());
        if ( (int) board.board[y][x] > 96) { //the position already contains player 2 tool
            cout << "Error: Two or more pieces are positioned on the same location in line " << lineNum <<
                 " of player 2's positioning" << endl;
            return false;
        }
        if (player2Alg->playerToolCounters[pos->getPiece()] == 0) {
            cout << "Error: A piece type appears more than it's number in line " << lineNum <<
                 " of player 2's positioning" << endl;
            return false;
        }
        if (pos->getJokerRep() !=  '#')
            player2Alg->addJokerPositionToVector(pos);

        player2Alg->playerToolCounters[pos->getPiece()]--;
        if (board.board[y][x] == ' ')
            board.board[y][x] = pos->getJokerRep()=='#' ? piece : (char) tolower(pos->getJokerRep());
        else {
            unique_ptr<RPSMove> initMove=make_unique<RPSMove>(RPSPoint(-1,-1), pos->getPosition(), piece, 2);
            fightOuter(initMove, fights, player1Alg, player2Alg);
        }
        lineNum++;
    }
    return true;
}


void RPSGame::fightOuter(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                         unique_ptr<RPSPlayerAlgorithm> &player1Alg, unique_ptr<RPSPlayerAlgorithm> &player2Alg) {

    char attackerTool = curMove->getPlayer() == 1 ? curMove->getPiece(): (char)tolower(curMove->getPiece());
    char defenderTool = board.board[curMove->getTo().getY()][curMove->getTo().getX()];
    if ((attackerTool == defenderTool) || (attackerTool == 'B') || (defenderTool == 'B') ||
            (attackerTool == 'b') || (defenderTool == 'b') )
        removeBothPiecesFromGame(curMove, fights, player1Alg, player2Alg);
    else if (curMove->getPiece() == 'f' ) //possible only in init stage
        fightAttackerLoses(curMove, fights, player2Alg, player2Alg);
    else if (defenderTool == 'F')
        fightAttackerWins(curMove, fights, player1Alg, player2Alg);
    else if (defenderTool == 'f')
        fightAttackerWins(curMove, fights, player1Alg, player2Alg);
    else
        fightInner(curMove, fights, player1Alg, player2Alg);
}

// remove both playertoolscounter and joker locations if there is , update board
void RPSGame::removeBothPiecesFromGame(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                                       unique_ptr<RPSPlayerAlgorithm> &player1,
                                       unique_ptr<RPSPlayerAlgorithm> &player2) {

    auto attackedPiece = (char) toupper(board.board[curMove->getTo().getY()][curMove->getTo().getX()]);
    auto attackerPiece = (char) toupper(curMove->getPiece());

    if (curMove->getPlayer() == 1) {
        removeToolsFromVectors(player1, curMove, attackerPiece);
        removeToolsFromVectors(player2, curMove, attackedPiece);
    } else {
        removeToolsFromVectors(player2, curMove, attackerPiece);
        removeToolsFromVectors(player1, curMove, attackedPiece);
    }

    updateFightVectors(0, curMove, fights);

    board.board[curMove->getTo().getY()][curMove->getTo().getX()] = ' ';
    if (curMove->getFrom().getX() != -1) //not in init stage, need to change 'from' position on board to ' '
        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] = ' ';
}




void RPSGame::fightInner(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                         unique_ptr<RPSPlayerAlgorithm> &player1,
                         unique_ptr<RPSPlayerAlgorithm> &player2) {
    if ((board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'R' ||
            board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'r') &&
        (curMove->getPiece() == 'P' || curMove->getPiece() == 'p'))
        fightAttackerWins(curMove, fights, player1, player2);

    else if ((board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'P' ||
            board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'p') &&
               (curMove->getPiece() == 'R' || curMove->getPiece() == 'r'))
        fightAttackerLoses(curMove, fights, player1, player2);

    else if ((board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'R' ||
            board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'r') &&
               (curMove->getPiece() == 'S' || curMove->getPiece() == 's'))
        fightAttackerLoses(curMove, fights, player1, player2);

    else if ((board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'S'
              || board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 's') &&
               (curMove->getPiece() == 'R' || curMove->getPiece() == 'r'))
        fightAttackerWins(curMove, fights, player1, player2);

    else if ((board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'P' ||
            board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'p') &&
               (curMove->getPiece() == 'S' || curMove->getPiece() == 's'))
        fightAttackerWins(curMove, fights, player1, player2);

    else if ((board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'S' ||
            board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 's') &&
               (curMove->getPiece() == 'P' || curMove->getPiece() == 'p'))
        fightAttackerLoses(curMove, fights, player1, player2);
}


void RPSGame::fightAttackerWins(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                                unique_ptr<RPSPlayerAlgorithm> &player1, unique_ptr<RPSPlayerAlgorithm> &player2) {
    auto attackedPiece= (char) toupper(board.board[curMove->getTo().getY()][curMove->getTo().getX()]);
    if (curMove->getPlayer() == 1)
        removeToolsFromVectors(player2, curMove, attackedPiece);
    else
        removeToolsFromVectors(player1, curMove, attackedPiece);
    updateFightVectors(curMove->getPlayer(), curMove, fights);
    board.board[curMove->getTo().getY()][curMove->getTo().getY()] = curMove->getPiece();
    if (curMove->getFrom().getX() != -1) //not init stage
        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] = ' ';
}


void RPSGame::fightAttackerLoses(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                                 unique_ptr<RPSPlayerAlgorithm> &player1, unique_ptr<RPSPlayerAlgorithm> &player2) {
    auto attackedPiece= (char) toupper(curMove->getPiece());
    if (curMove->getPlayer() == 1)
        removeToolsFromVectors(player1, curMove, attackedPiece);
    else
        removeToolsFromVectors(player2, curMove, attackedPiece);
    updateFightVectors(curMove->getPlayer() == 1 ? 2 : 1, curMove, fights);
    if (curMove->getFrom().getX() != -1) //not init stage
        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] = ' ';
}


void RPSGame::removeToolsFromVectors(unique_ptr<RPSPlayerAlgorithm> &player, unique_ptr<RPSMove> &curMove,
                                     char pieceToRemove) {
        auto it = find_if( player->playerJokers.begin(), player->playerJokers.end(),
                          [&](unique_ptr<Point> &obj) {
                              return obj->getX() == curMove->getTo().getX()
                                     && obj->getY() == curMove->getTo().getY();
                          });
        if (it !=  player->playerJokers.end()) {
            player->playerToolCounters['J']++;
            player->playerJokers.erase(it);
        } else {
            player->playerToolCounters[pieceToRemove]++;
        }

}

void RPSGame::updateFightVectors(int winner, unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights) {

    unique_ptr<FightInfo> fight;

    auto attackedPiece= (char) toupper(board.board[curMove->getTo().getY()][curMove->getTo().getX()]);
    auto attackerPiece = (char) toupper(curMove->getPiece());

    if (curMove->getPlayer() == 1)
        fight = make_unique<RPSFightInfo>(winner, attackerPiece, attackedPiece, curMove->getTo());
    else
        fight = make_unique<RPSFightInfo>(winner, attackedPiece, attackerPiece, curMove->getTo());

    fights.push_back(fight);
}




bool RPSGame::CheckIfPlayerLose(unique_ptr<RPSPlayerAlgorithm> &player) {
    if (player->playerToolCounters['F'] == F)
        return true;
    return player->playerToolCounters['R'] == R && player->playerToolCounters['P'] == P
           && player->playerToolCounters['S'] == S && player->playerToolCounters['J'] == J;
}
RPSMove RPSGame::setMoveToBoard(unique_ptr<Move> curMove, int player, RPSFightInfo &curFight) {
    unique_ptr<RPSMove> resultMove=make_unique<RPSMove>(curMove->getFrom(),curMove->getTo(),
                        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()],player);
    char fromPiece = board.board[curMove->getFrom().getY()][curMove->getFrom().getX()];
    char toPiece = board.board[curMove->getTo().getY()][curMove->getTo().getX()];
    if(toPiece!=' ') { //fight!
        vector<unique_ptr<FightInfo>> fights;
        curFight.setPlayer1Piece(isupper(fromPiece) == 0 ? toPiece : fromPiece);
        curFight.setPlayer2Piece(isupper(fromPiece) == 0 ? fromPiece : toPiece);
        curFight.setPosition(curMove->getTo());
        fightOuter(resultMove, fights, player1, player2);
        curFight.setWinner(fights.at(0)->getWinner());
    } else
        board.board[curMove->getTo().getY()][curMove->getTo().getX()]=fromPiece;

    return RPSMove(resultMove->getFrom(),resultMove->getTo(),resultMove->getPiece(),resultMove->getPlayer());
}

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


