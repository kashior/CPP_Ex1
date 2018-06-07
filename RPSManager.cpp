

#include "RPSManager.h"


RPSManager::RPSManager(bool isPlayer1Auto, bool isPlayer2Auto) {
//    curGame = make_unique<RPSGame>(isPlayer1Auto, isPlayer2Auto);
    player1Points = 0;
    player2Points = 0;

}




RPSMove RPSManager::setMoveToBoard(unique_ptr<Move> curMove, int player, RPSFightInfo &curFight) {

    char fromPiece = board.board[curMove->getFrom().getY()][curMove->getFrom().getX()];
    char toPiece = board.board[curMove->getTo().getY()][curMove->getTo().getX()];
    RPSPoint fromPoint(curMove->getFrom().getX(),curMove->getFrom().getY());
    RPSPoint toPoint(curMove->getTo().getX(),curMove->getTo().getY());
    unique_ptr<RPSMove> resultMove=
            make_unique<RPSMove>(fromPoint, toPoint, board.board[fromPoint.getY()][fromPoint.getX()],player);

    if(toPiece!=' ') { //fight!
        curFight.setIsFight(true);
        vector<unique_ptr<FightInfo>> fights;
        curFight.setPlayer1Piece(isupper(fromPiece) == 0 ? toPiece : fromPiece);
        curFight.setPlayer2Piece(isupper(fromPiece) == 0 ? fromPiece : toPiece);
        curFight.setPosition(RPSPoint(curMove->getTo().getX(),curMove->getTo().getY()));
        doFight(fights, resultMove);
        curFight.setWinner(fights.at(0)->getWinner());
//        if (player == 1 && fights.at(0)->getWinner() == 1) //attacker wins maybe it was a joker
//            changeJokerPosition(player1, curMove);
//        else if (player == 2 && fights.at(0)->getWinner() == 2)
//            changeJokerPosition(player2, curMove);
    }

    else {
        curFight.setIsFight(false);
        board.board[curMove->getTo().getY()][curMove->getTo().getX()] = fromPiece;
        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] = ' ';
//        if (player == 1)
//            changeJokerPosition(player1, curMove);
//        else
//            changeJokerPosition(player2, curMove);
    }
    RPSMove moveRes(fromPoint,toPoint,resultMove->getPiece(),resultMove->getPlayer());
    return moveRes;
}



void RPSManager::doFight(vector<unique_ptr<FightInfo>> &fights, unique_ptr<RPSMove> fightMove) {
    char attackerTool = fightMove->getPlayer() == 1 ? fightMove->getPiece() : (char) tolower(fightMove->getPiece());
    char defenderTool = board.board[fightMove->getTo().getY()][fightMove->getTo().getX()];
    if ((attackerTool == defenderTool) || (attackerTool == 'B') || (defenderTool == 'B') ||
        (attackerTool == 'b') || (defenderTool == 'b') || (char) toupper(defenderTool) == attackerTool ||
        (char) toupper(attackerTool) == defenderTool)
        removeBothPiecesFromGame(fightMove, fights);
    else if (fightMove->getPiece() == 'f') //possible only in init stage
        fightAttackerLoses(fightMove, fights);
    else if (defenderTool == 'F' || defenderTool == 'f')
        fightAttackerWins(fightMove, fights);
    else
        fightInner(fightMove, fights);
}


void RPSManager::fightInner(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights) {
    int x = curMove->getTo().getX();
    int y = curMove->getTo().getY();
    char piece = curMove->getPiece();
    if ((board.board[y][x] == 'R' || board.board[y][x] == 'r') && (piece == 'P' || piece == 'p'))
        fightAttackerWins(curMove, fights);

    else if ((board.board[y][x] == 'P' || board.board[y][x] == 'p') && (piece == 'R' || piece == 'r'))
        fightAttackerLoses(curMove, fights);

    else if ((board.board[y][x] == 'R' || board.board[y][x] == 'r') && (piece == 'S' || piece == 's'))
        fightAttackerLoses(curMove, fights);

    else if ((board.board[y][x] == 'S' || board.board[y][x] == 's') && (piece == 'R' || piece == 'r'))
        fightAttackerWins(curMove, fights);

    else if ((board.board[y][x] == 'P' || board.board[y][x] == 'p') && (piece == 'S' || piece == 's'))
        fightAttackerWins(curMove, fights);

    else if ((board.board[y][x] == 'S' || board.board[y][x] == 's') && (piece == 'P' || piece == 'p'))
        fightAttackerLoses(curMove, fights);
}

void RPSManager::removeBothPiecesFromGame(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights) {

    updateFightVectors(0, curMove, fights);
    board.board[curMove->getTo().getY()][curMove->getTo().getX()] = ' ';
    if (curMove->getFrom().getX() != -1) //not in init stage, need to change 'from' position on board to ' '
        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] = ' ';

}

// used only during init stage!!
void RPSManager::updateFightVectors(int winner, unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights) {

    unique_ptr<FightInfo> fight;

    auto attackedPiece = (char) toupper(board.board[curMove->getTo().getY()][curMove->getTo().getX()]);
    auto attackerPiece = (char) toupper(curMove->getPiece());
    RPSPoint fightPoint(curMove->getTo().getX(), curMove->getTo().getY());

    if (curMove->getPlayer() == 1)
        fight = make_unique<RPSFightInfo>(winner, attackerPiece, attackedPiece, fightPoint, true);
    else
        fight = make_unique<RPSFightInfo>(winner, attackedPiece, attackerPiece, fightPoint, true);

    fights.push_back(move(fight));
}

void RPSManager::fightAttackerWins(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights) {
    char attackerPiece = 1 == (int) curMove->getPlayer ? curMove->getPiece() : (char) tolower(curMove->getPiece());
    updateFightVectors(curMove->getPlayer(), curMove, fights);
    board.board[curMove->getTo().getY()][curMove->getTo().getX()] = attackerPiece;
    if (curMove->getFrom().getX() != -1) //not init stage
        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] = ' ';
}

void RPSManager::fightAttackerLoses(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights) {
    updateFightVectors(curMove->getPlayer() == 1 ? 2 : 1, curMove, fights);
    if (curMove->getFrom().getX() != -1) //not init stage
        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] = ' ';
}

void RPSManager::updateInitialBoard(vector<unique_ptr<FightInfo>> &fights) {
    int x, y;
    char piece;

    //update player 1 tools to the board
    for (auto &pos: player1Positioning)
        board.board[pos->getPosition().getY()][pos->getPosition().getX()] = pos->getPiece();

    // update player 2 tools to board , fight if necessary
    for (auto &pos: player2Positioning) {
        x = pos->getPosition().getX();
        y = pos->getPosition().getY();
        piece = (char) tolower(pos->getPiece());
        if (board.board[pos->getPosition().getY()][pos->getPosition().getX()] != ' ')//FIGHT!
        {
            RPSPoint nullPoint;
            RPSPoint fightPoint(x, y);
            unique_ptr<RPSMove> fightMove = make_unique<RPSMove>(nullPoint, fightPoint, piece, 2);
            doFight(fights, move(fightMove));
        } else
            board.board[pos->getPosition().getY()][pos->getPosition().getX()] = piece;
    }
}


int RPSManager::gameHandler() {

    int reason; // the "reason" for output file
    // 1=all flags of the opponent are captures
    // 2=All moving PIECEs of the opponent are eaten
    // 6=100 moves done without a fight
    int winner; // 0=tie, 1=player1 wins, 2=player2 wins, 3= continue the game

    vector<unique_ptr<FightInfo>> fights;

    player1->getInitialPositions(1, player1Positioning);
    player2->getInitialPositions(2, player2Positioning);

    updateInitialBoard(fights);

    // lets notify on initial positions to the players algorithms
    player1->notifyOnInitialBoard(board, fights);
    player2->notifyOnInitialBoard(board, fights);

    if((winner=checkWinner(reason))!=3)
        return winner;

    unique_ptr<Move> curMovePtr;
    unique_ptr<JokerChange> curJokerChangePtr;
    RPSMove curMove;
    RPSFightInfo curFight;

    while ((winner == 3) && (movesCounter < 100) ) {

            // player 1's turn

            curMovePtr =player1->getMove(); // get the move from player algorithm

//            if (!checkIfMoveIsValid(curMovePtr, 1, moreMoves1)) {
//                param1 = false;
//                winner = 2;
//                reason = 5;
//                break;
//            }

            //if (moreMoves1) { //if there are still more moves for player1 to make
            curMove = setMoveToBoard(move(curMovePtr), 1,
                                              curFight);// apply move to board after it has been checked
            if (curFight.getIsFight())// if there was a fight during this move
                movesCounter = 0;
            else {
                movesCounter++;
                if (movesCounter >= 100) {
                    winner = 0;
                    cout << "100 moves done without a fight! It's a tie\n" << endl;
                    reason = 6;
                    break;
                }
            }

            player1->notifyFightResult(curFight);// in case there was a fight, notify player algorithm
            checkWinner(reason);
            if (winner != 3)
                break;

            player2->notifyOnOpponentMove(curMove);
            player2->notifyFightResult(curFight);

            curJokerChangePtr = player1->getJokerChange(); // in case there was joker change, get it
            if (curJokerChangePtr != nullptr) {
                board.board[curJokerChangePtr->getJokerChangePosition().getY()]
                [curJokerChangePtr->getJokerChangePosition().getX()] = curJokerChangePtr->getJokerNewRep();
            }

//player 2's turn

            curMovePtr = player2->getMove(); // get the move from player algorithm

            curMove = setMoveToBoard(move(curMovePtr), 2,
                                                  curFight);
            if (curFight.getIsFight())// if there was a fight during this move
                movesCounter = 0;
            else {
                movesCounter++;
                if (movesCounter >= 100){
                    winner = 0;
                    cout << "100 moves done without a fight! It's a tie\n" << endl;
                    reason = 6;
                    break;
                }
            }

            player2->notifyFightResult(curFight);// in case there was a fight, notify player algorithm

            winner = checkWinner(reason);// first check for winner
            if (winner != 3)
                break;

            player1->notifyOnOpponentMove(curMove);
            player1->notifyFightResult(curFight);

            curJokerChangePtr = player2->getJokerChange(); // in case there was joker change, get it
            if (curJokerChangePtr != nullptr) {
                auto jokerPiece = (char) tolower(curJokerChangePtr->getJokerNewRep());
                board.board[curJokerChangePtr->getJokerChangePosition().getY()]
                        [curJokerChangePtr->getJokerChangePosition().getX()] = jokerPiece;

            }
        }

    makeOutputFile(reason, winner);

    return winner;
}


void RPSManager::updateWinner(bool param1, bool param2, int &winner) {
    if (!param1 && !param2)
        winner = 0; //tie
    else if (!param1) { //bad input file for player 1
        winner = 2;
        player2Points += 1;
    } else { // bad input file for player 2
        winner = 1;
        player1Points += 1;
    }
}


void RPSManager::makeOutputFile(int reason, int winner) {
    ofstream fout("rps.output");
    int loser;
    int badLine;
    if (winner == 3)
        winner = 0;

    fout << "Winner: " << winner << endl;
    fout << "Reason: ";

//    if (reason == 4) { //"bad positioning input file"
//        if (!param1 && !param2) //both files are bad
//            fout << "Bad Positioning input file for both players - player 1: line " << lineNum1 <<
//                 ", player 2: line " << lineNum2 << endl;
//        else {
//            updateLoserAndBadLine(winner, loser, lineNum1, lineNum2, badLine);
//            fout << "Bad Positioning input file for player " << loser << " - line " << badLine << endl;
//        }
    if (reason == 1 || reason == 2) {
        if (reason == 1)
            fout << "All flags of the opponent are captured" << endl;
        else
            fout << "All moving PIECEs of the opponent are eaten" << endl;
//    } else if (reason == 3)
//        fout << "A tie - both Moves input files done without a winner" << endl;
//
//    else if (reason == 5) {
//        updateLoserAndBadLine(winner, loser, lineNum1, lineNum2, badLine);
//        fout << "Bad Moves input file for player " << loser << " - line " << badLine << endl;
    } else { //(reason == 6)
        fout << "A tie - 100 moves have invoked without a fight" << endl;
    }

    fout << endl;
    // printing the game board state
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < M; i++) {
            fout << board.getPiece(i, j);
        }
        fout << endl;
    }
    fout.close();
}


void RPSManager::updateLoserAndBadLine(int winner, int &loser, int param1, int param2, int &badLine) {
    if (winner == 1) {
        loser = 2;
        badLine = param2;
    } else {
        loser = 1;
        badLine = param1;
    }
}


int RPSManager::checkWinner(int & reason) {
    int flags1 = 0, flags2 = 0;
    int moving1 = 0, moving2 = 0;
    char piece;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            piece = board.getPiece(j, i);
            if (piece == 'R' || piece == 'P' || piece == 'S')
                moving1++;
            else if (piece == 'r' || piece == 'p' || piece == 's')
                moving2++;
            else if(piece=='F')
                flags1++;
            else if (piece=='f')
                flags2++;
        }
    }
    if((flags1==0 || moving1==0)&& (flags2==0 || moving2==0)){
        if (flags1==0 || flags2==0)
            reason=1;
        else
            reason=2;
        return 0;
    }
    if(flags1==0 || moving1==0){
        if (flags1==0)
            reason=1;
        else
            reason=2;
        return 2;
    }
    if (flags2==0 || moving2==0){
        if (flags2==0)
            reason=1;
        else
            reason=2;
        return 1;
    }
    return 3;
}


void RPSManager::checkAndUpdateReasonForWinner(int &reason) {
    if (curGame->player1->playerToolCounters['F'] == F || curGame->player2->playerToolCounters['F'] == F)
        reason = 1; // "all flags of the opponent are captured"
    else
        reason = 2; // "all moving pieces of the opponent are eaten"
}


bool RPSManager::checkIfMoveIsValid(unique_ptr<Move> &curMove, int player, bool &moreMoves) {
    if (curMove->getFrom().getX() == -2)
        return false;
    else if (curMove->getFrom().getX() == -1) {
        moreMoves = false;
        return true;
    }
    return checkIfMoveIsValidBoardwise(curMove, player);


}

bool RPSManager::checkIfMoveIsValidBoardwise(unique_ptr<Move> &curMove, int player) {
    if (curGame->board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] == ' ')
        return false;
    if (player == 1) {
        if (isupper(curGame->board.board[curMove->getTo().getY()][curMove->getTo().getX()]))
            return false;
        if (curGame->board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] == 'F'
            || curGame->board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] == 'B')
            return false; //can't move un-moving pieces
    }
    if (player == 2) {
        if (islower(curGame->board.board[curMove->getTo().getY()][curMove->getTo().getX()]))
            return false;
        if (curGame->board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] == 'f'
            || curGame->board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] == 'b')
            return false; //can't move un-moving pieces
    }


    return true; //all good
}


bool RPSManager::checkIfJokerChangeIsValid(unique_ptr<JokerChange> &curChange, int player) {
    const unique_ptr<RPSPlayerAlgorithm> &curPlayer = player == 1 ? curGame->player1 : curGame->player2;

    for (const unique_ptr<RPSPoint> &joker:curPlayer->playerJokers) {
        if (curChange->getJokerChangePosition().getX() == joker->getX() &&
            curChange->getJokerChangePosition().getY() == joker->getY())
            return true;

    }
    return false;
}

