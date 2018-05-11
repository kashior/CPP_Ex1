

#include "RPSManager.h"


RPSManager::RPSManager(bool isPlayer1Auto, bool isPlayer2Auto) {
    curGame = make_unique<RPSGame>(isPlayer1Auto,isPlayer2Auto);
    player1Points = 0;
    player2Points = 0;

}

bool RPSManager::initCheck(int playerNum) {
    if (playerNum == 1) {
        curGame->player1->getInitialPositions(1, player1Positioning);
        if (player1Positioning.empty()) //empty file
            return false;
    } else {
        curGame->player2->getInitialPositions(2, player2Positioning);
        if (player2Positioning.empty())
            return false;
    }
    return true;
}


/**
 * The "main manager" of the game. Handles the all flow of the game.
 *
 * @param isPlayer1Auto - true if player 1 is auto-algorithm, false if it is file-algorythm.
 * @param isPlayer2Auto - true if player 2 is auto-algorithm, false if it is file-algorythm.
 */
void RPSManager::gameHandler() {

    bool file1OK;
    bool file2OK;


    file1OK = initCheck(1);
    file2OK = initCheck(2);

    int reason; // the "reason" for output file
    // 1=all flags of the opponent are captures
    // 2=All moving PIECEs of the opponent are eaten
    // 3=A tie - both Moves input files done without a winner
    // 4=bad positioning input file for one player or both
    // 5=bad moves input file for some player
    int winner; // 0=tie, 1=player1 wins, 2=player2 wins 3= continue

    //now we will update the game board with the positions vectors and continue checking if files are valid


    //want to check if input file/s is/are valid
    if (!file1OK || !file2OK) { //at least one of the positioning input files is bad
        updateWinner(file1OK, file2OK, winner);
        makeOutputFile(4, file1OK, file2OK, winner, curGame->player1->lineNum, curGame->player2->lineNum);
        return;
    }
    curGame->player1->lineNum = 1; //for player1 input file (if exists)
    curGame->player2->lineNum = 1; //for player2 input file (if exists)

    file1OK = curGame->UpdateBoardPlayer1InitStage(curGame->player1->lineNum, player1Positioning, curGame->player1);

    ///////////////
    curGame->printBoardToScreen();
    ///////////////
    vector<unique_ptr<FightInfo>> fights;
    file2OK = curGame->UpdateBoardPlayer2InitStage(curGame->player2->lineNum, player2Positioning, curGame->player1, curGame->player2,
                                                   fights);

    ////////////////
    curGame->printBoardToScreen();
    /////////////////

    if (!file1OK || !file2OK) { //at least one of the positioning input files is bad
        updateWinner(file1OK, file2OK, winner);
        makeOutputFile(4, file1OK, file2OK, winner, curGame->player1->lineNum, curGame->player2->lineNum);
        return;
    }
    // input file/s are valid, now before setting the moves we want to check if maybe there is already a winner...

    checkWinner(winner, reason);
    if (winner != 3) {
        makeOutputFile(reason, true, true, winner, 0, 0);
        return;
    }

    // lets notify on initial positions to the players algorithms
    curGame->player1->notifyOnInitialBoard(curGame->board, fights);
    curGame->player2->notifyOnInitialBoard(curGame->board, fights);

    // now lets read moves files (if there are)

    bool param1 = true, param2 = true;
    bool moreMoves1 = true, moreMoves2 = true; //Set to "true" if there are more moves in the moves files of
                                                // of player 1 and 2. Set to "false" ones there are no more
                                                // moves in the file. For auto players the value is always
                                                // "true".


    unique_ptr<Move> curMovePtr;
    unique_ptr<JokerChange> curJokerChangePtr;
    curGame->player1->lineNum = curGame->player2->lineNum = 1;
    RPSMove curMove;
    RPSFightInfo curFight;

    while ((winner == 3) && (curGame->movesCounter < 100) && (moreMoves1  || moreMoves2)) {

        if (moreMoves1){
        // player 1's turn

            curMovePtr = curGame->player1->getMove(); // get the move from player algorithm

            if (!checkIfMoveIsValid(curMovePtr, 1, moreMoves1)) {
                param1 = false;
                winner = 2;
                reason = 5;
                break;
            }

            if (moreMoves1){ //if there are still more moves for player1 to make
                curMove = curGame->setMoveToBoard(move(curMovePtr), 1,
                                                  curFight);// apply move to board after it has been checked
                curGame->player1->notifyFightResult(curFight);// in case there was a fight, notify player algorithm
                checkWinner(winner, reason);// first check for winner
                if (winner != 3)
                    break;
                curGame->player1->lineNum++;
                curGame->player2->notifyOnOpponentMove(curMove);
                curGame->player2->notifyFightResult(curFight);

                curJokerChangePtr = curGame->player1->getJokerChange(); // in case there was joker change, get it
                if (curJokerChangePtr != nullptr) {
                    if(checkIfJokerChangeIsValid(curJokerChangePtr, 1))
                        curGame->board.board[curJokerChangePtr->getJokerChangePosition().getY()]
                        [curJokerChangePtr->getJokerChangePosition().getX()]=curJokerChangePtr->getJokerNewRep();
                    else
                    {
                        param1 = false;
                        winner = 2;
                        reason = 5;
                        break;
                    }
                }

                ++curGame->movesCounter;

                ////////////////////////////
                curGame->printBoardToScreen();
                /////////////////////////////
            }
        }

        if (moreMoves2){

//player 2's turn
            curMovePtr = curGame->player2->getMove(); // get the move from player algorithm

            if (!checkIfMoveIsValid(curMovePtr, 2, moreMoves2)) {
                param2 = false;
                winner = 1;
                reason = 5;
                break;
            }

            if (moreMoves2){ //if there are still more moves for player2 to make
                curMove = curGame->setMoveToBoard(move(curMovePtr), 2,
                                                  curFight);// apply move to board after it has been checked
                curGame->player2->notifyFightResult(curFight);// in case there was a fight, notify player algorithm
                checkWinner(winner, reason);// first check for winner
                if (winner != 3)
                    break;
                curGame->player2->lineNum++;
                curGame->player1->notifyOnOpponentMove(curMove);
                curGame->player1->notifyFightResult(curFight);

                curJokerChangePtr = curGame->player2->getJokerChange(); // in case there was joker change, get it
                if (curJokerChangePtr != nullptr) {
                    auto jokerPiece= (char)tolower(curJokerChangePtr->getJokerNewRep());
                    if(checkIfJokerChangeIsValid(curJokerChangePtr, 2))
                        curGame->board.board[curJokerChangePtr->getJokerChangePosition().getY()]
                        [curJokerChangePtr->getJokerChangePosition().getX()]=jokerPiece;
                    else {
                        param2 = false;
                        winner = 1;
                        reason = 5;
                        break;
                    }
                }

                ++curGame->movesCounter;

                //////////////////////////////
                curGame->printBoardToScreen();
                //////////////////////////////
            }
        }
    }

    makeOutputFile(reason, param1, param2, winner, curGame->player1->lineNum, curGame->player2->lineNum);
}



/**
 * Updates the winner of the game according to the parameters sent to it.
 *
 * @param param1 - true if player 1 is the winner, false otherwise
 * @param param2 - true if player 2 is the winner, false otherwise
 * @param winner - will be set by the function to 1 if player 1 is the winner, 2 if player 2 is the winner or set to 0
 * if its a tie.
 *
 */
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



void RPSManager::makeOutputFile(int reason, bool param1, bool param2, int winner, int lineNum1, int lineNum2) {
    ofstream fout("rps.output");
    int loser;
    int badLine;
    if(winner ==3)
        winner=0;

    fout << "Winner: " << winner << endl;
    fout << "Reason: ";

    if (reason == 4) { //"bad positioning input file"
        if (!param1 && !param2) //both files are bad
            fout << "Bad Positioning input file for both players - player 1: line " << lineNum1 <<
                 ", player 2: line " << lineNum2 << endl;
        else {
            updateLoserAndBadLine(winner, loser, lineNum1, lineNum2, badLine);
            fout << "Bad Positioning input file for player " << loser << " - line " << badLine << endl;
        }
    } else if (reason == 1 || reason == 2) {
        if (reason == 1)
            fout << "All flags of the opponent are captured" << endl;
        else
            fout << "All moving PIECEs of the opponent are eaten" << endl;
    } else if (reason == 3)
        fout << "A tie - both Moves input files done without a winner" << endl;

    else { //(reason == 5)
        updateLoserAndBadLine(winner, loser, lineNum1, lineNum2, badLine);
        fout << "Bad Moves input file for player " << loser << " - line " << badLine << endl;
    }

    fout << endl;
    // printing the game board state
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < M; i++) {
            fout << curGame->board.getPiece(i, j);
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


void RPSManager::checkWinner(int &winner, int &reason) {
    if (curGame->CheckIfPlayerLose(curGame->player1) && curGame->CheckIfPlayerLose(curGame->player2)) { // both wins
        winner = 0;
    } else if (curGame->CheckIfPlayerLose(curGame->player1)) { //player 2 wins
        winner = 2;
        player2Points += 1;
    } else if (curGame->CheckIfPlayerLose(curGame->player2)) {// player 1 wins
        winner = 1;
        player1Points += 1;
    } else { // no one wins
        winner = 3;
        reason = 3; // "A tie - both moves input files done without a winner"
        return;
    }
    checkAndUpdateReasonForWinner(reason);
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
    else if (curMove->getFrom().getX() == -1){
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
    const unique_ptr<RPSPlayerAlgorithm> & curPlayer= player==1 ? curGame->player1 : curGame->player2;
//    char jokerPiece = curChange->getJokerNewRep(); parser already do it
//    if(jokerPiece != 'R' && jokerPiece != 'P' && jokerPiece!='S'&& jokerPiece != 'B' )
//        return false;

    for (const unique_ptr<RPSPoint> &joker:curPlayer->playerJokers) {
        if (curChange->getJokerChangePosition().getX() == joker->getX() &&
            curChange->getJokerChangePosition().getY() == joker->getY())
            return true;

    }
    return false;
}

