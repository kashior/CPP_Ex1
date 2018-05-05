

#include "RPSManager.h"


RPSManager::RPSManager(){
    curGame = make_unique<RPSGame>();
    player1Points = 0;
    player2Points = 0;
}


bool RPSManager::initCheck(int playerNum){
    if (playerNum == 1){
        curGame->player1->getInitialPositions(1, player1Positioning);
        if (player1Positioning.empty()) //empty file
            return false;
    } else {
        curGame->player2->getInitialPositions(2, player2Positioning);
        if(player2Positioning.empty())
            return false;
    }
    return true;
}


void RPSManager::gameHandler(bool isPlayer1Auto, bool isPlayer2Auto) {

    bool file1OK ;
    bool file2OK ;

    if(!isPlayer1Auto)
        curGame->player1 = make_unique<RPSFilePlayerAlgorithm>(1,"/");
    else
        curGame->player1=make_unique<RPSAutoPlayerAlgorithm>(1,"/");
    file1OK = initCheck(1);

    if(!isPlayer2Auto)
        curGame->player2= make_unique<RPSFilePlayerAlgorithm>(2,"/");
    else
        curGame->player2=make_unique<RPSAutoPlayerAlgorithm>(2,"/");
    file2OK = initCheck(2);

    int reason; // the "reason" for output file
    // 1=all flags of the opponent are captures
    // 2=All moving PIECEs of the opponent are eaten
    // 3=A tie - both Moves input files done without a winner
    // 4=bad positioning input file for one player or both
    // 5=bad moves input file for some player
    int winner; // 0=tie, 1=player1 wins, 2=player2 wins 3= continue

    //now we will update the game board with the positions vectors and continue checking if files are valid
    int lineNum1 = 1; //for player1 input file (if exists)
    int lineNum2 = 1; //for player2 input file (if exists)

    //want to check if input file/s is/are valid
    if (!file1OK || !file2OK){ //at least one of the positioning input files is bad
        updateWinner(file1OK, file2OK, winner);
        makeOutputFile(4, file1OK, file2OK, winner, lineNum1, lineNum2);
        return;
    }
    file1OK = curGame->UpdateBoardPlayer1InitStage(lineNum1, player1Positioning, curGame->player1);
    vector<unique_ptr<FightInfo>> fights;
    file2OK = curGame->UpdateBoardPlayer2InitStage(lineNum2, player2Positioning, curGame->player1, curGame->player2, fights);

    if (!file1OK || !file2OK){ //at least one of the positioning input files is bad
        updateWinner(file1OK, file2OK, winner);
        makeOutputFile(4, file1OK, file2OK, winner, lineNum1, lineNum2);
        return;
    }
    // input file/s are valid, now before setting the moves we want to check if maybe there is already a winner...

    checkWinner(winner, reason);
    if(winner!=3) {
        makeOutputFile(reason, true, true, winner, 0, 0);
        return;
    }

    // lets notify on initial positions to the players algorithms
    curGame->player1->notifyOnInitialBoard(curGame->board, fights);
    curGame->player2->notifyOnInitialBoard(curGame->board, fights);


    // now lets read moves files (if there are)

    bool param1=true, param2=true;

    unique_ptr<Move> curMovePtr;
    unique_ptr<JokerChange> curJokerChangePtr;
    lineNum1=lineNum2=1;
    RPSMove curMove;
    RPSFightInfo curFight;

    while( (winner == 3)  && (curGame->movesCounter < 100) ){
// player 1's turn
        curMovePtr = curGame->player1->getMove(); // get the move from player algorithm
        curJokerChangePtr=curGame->player1->getJokerChange(); // in case there was joker change, get it

        if(!checkIfMoveIsValid(curMovePtr, 1)) {
            param1=false;
            winner=2;
            reason=5;
            break;
        }
        curMove= curGame->setMoveToBoard(move(curMovePtr), 1, curFight);// apply move to board after it has been checked
        curGame->player1->notifyFightResult(curFight);// in case there was a fight, notify player algorithm
        checkWinner(winner, reason);// first check for winner
        if(winner!=3)
            break;
        //then, check for jokerchange and apply
        //TODO: implement
        lineNum1++;
        curGame->player2->notifyOnOpponentMove(curMove);
        curGame->player2->notifyFightResult(curFight);
        curGame->movesCounter++;

//player 2's turn
        curMovePtr = curGame->player2->getMove(); // get the move from player algorithm
        curJokerChangePtr=curGame->player2->getJokerChange(); // in case there was joker change, get it

        if(!checkIfMoveIsValid(curMovePtr, 2)) {
            param2=false;
            winner=1;
            reason=5;
            break;
        }
        curMove= curGame->setMoveToBoard(move(curMovePtr), 2, curFight);// apply move to board after it has been checked
        curGame->player2->notifyFightResult(curFight);// in case there was a fight, notify player algorithm
        checkWinner(winner, reason);// first check for winner
        if(winner!=3)
            break;
        //then, check for jokerchange and apply
        //TODO: implement
        lineNum2++;
        curGame->player1->notifyOnOpponentMove(curMove);
        curGame->player1->notifyFightResult(curFight);
        curGame->movesCounter++;
    }
    makeOutputFile(reason, param1, param2, winner, lineNum1, lineNum2);
}





void RPSManager::updateWinner(bool param1, bool param2, int &winner) {
    if (!param1 && !param2)
        winner = 0; //tie
    else if (!param1) { //bad input file for player 1
        winner = 2;
        player2Points += 1;
    }
    else { // bad input file for player 2
        winner = 1;
        player1Points += 1;
    }
}



void RPSManager::makeOutputFile(int reason, bool param1, bool param2, int winner, int lineNum1, int lineNum2) {
    ofstream fout("rps.output");
    int loser;
    int badLine;

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
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            fout << curGame->board.getPiece(i,j);
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
    if (curGame->CheckIfPlayerLose(curGame->player1)&&curGame->CheckIfPlayerLose(curGame->player2)) { // both wins
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

bool RPSManager::parseArguments(bool &isPlayer1Auto, bool &isPlayer2Auto, string args) {
    string delimiter = "-";
    string parameters[3];
    size_t pos = 0;
    string token;
    int i=0;
    while ((pos = args.find(delimiter)) != string::npos) {
        if(i==3)
            return false;
        token = args.substr(0, pos);
        args.erase(0, pos + delimiter.length());
        parameters[i++]=token;
    }
    if(parameters[0]=="auto")
        isPlayer1Auto=true;
    else if(parameters[0]=="file")
        isPlayer1Auto=false;
    else
        return false;
    if(parameters[1]!="vs")
        return false;
    if(parameters[2]=="auto")
        isPlayer2Auto=true;
    else if(parameters[2]=="file")
        isPlayer2Auto=false;
    else
        return false;
    return true;
}

bool RPSManager::checkIfMoveIsValid(unique_ptr<Move> &curMove, int player) {
    if(curMove->getFrom().getX() == -1)
        return false;

    return checkIfMoveIsValidBoardwise(curMove, player);


}

bool RPSManager::checkIfMoveIsValidBoardwise(unique_ptr<Move> &curMove, int player) {
    if (curGame->board.board[curMove->getFrom().getY()][curMove->getFrom().getX()]==' ')
        return false;
    if (player==1){
        if(isupper(curGame->board.board[curMove->getTo().getY()][curMove->getTo().getX()]))
            return false;
        if (curGame->board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] == 'F'
            || curGame->board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] == 'B')
            return false; //can't move un-moving pieces
    }
    if (player==2){
        if(islower(curGame->board.board[curMove->getTo().getY()][curMove->getTo().getX()]))
            return false;
        if (curGame->board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] == 'f'
            || curGame->board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] == 'b')
            return false; //can't move un-moving pieces
    }
//    if (curMove->getJoker()->getJokerNewRep() != '\0') //if there is a joker change in this move
//        return checkIfJokerChangeIsValid(curMove, player);

    return true; //all good
}



//bool RPSManager::checkIfJokerChangeIsValid(unique_ptr<RPSMove> &curMove, int player) {
//    if (player == 1) {
//        if (find(curGame->player1->playerJokers.begin(), curGame->player1->playerJokers.end(),
//                 curMove->getJoker()->getJokerChangePosition()) != curGame->player1->playerJokers.end())
//            //if there is no joker in this position on the board
//            return false;
//    } else {
//        if (find(curGame->player2->playerJokers.begin(), curGame->player2->playerJokers.end(),
//                 curMove->getJoker()->getJokerChangePosition()) != curGame->player2->playerJokers.end())
//            //if there is no joker in this position on the board
//            return false;
//    }
//    return true;
//}

//
//void RPSManager::checkIfMoveIsNull(unique_ptr<Move> &curMove, bool &moreMoves) {
//    if (curMove->getFrom().getX() == -1) //the move is empty, meaning there are no more moves in the moves file
//        moreMoves = false;
//
//}
