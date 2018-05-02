//
// Created by Maria Klimkin on 01-May-18.
//

#include "RPSManager.h"


RPSManager::RPSManager(bool isPlayer1Auto, bool isPlayer2Auto, int player1Points, int player2Points) {
    if(!isPlayer1Auto)
        player1= make_unique<RPSFilePlayerAlgorithm>(1,"/");
    else
        player1=make_unique<RPSAutoPlayerAlgorithm>(1,"/");
    if(!isPlayer2Auto)
        player2= make_unique<RPSFilePlayerAlgorithm>(2,"/");
    else
        player2=make_unique<RPSAutoPlayerAlgorithm>(2,"/");

    curGame = make_unique<RPSGame>();

    player1Points = 0;
    player2Points = 0;
}



void RPSManager::gameHandler(bool isPlayer1Auto, bool isPlayer2Auto) {
    bool file1Good, file2Good;
    int reason; // the "reason" for output file
    // 1=all flags of the opponent are captures
    // 2=All moving PIECEs of the opponent are eaten
    // 3=A tie - both Moves input files done without a winner
    // 4=bad positioning input file for one player or both
    // 5=bad moves input file for some player
    int winner; // 0=tie, 1=player1 wins, 2=player2 wins

    // checking the board game input files
    if (!isPlayer1Auto)
        file1Good = curGame->RPSGameInitFileCheck(player1, 1);
    if (!isPlayer2Auto)
        file2Good = curGame->RPSGameInitFileCheck(player2, 2);

    if (!file1Good || !file2Good) { //at least one of the positioning input files is bad
        updateWinner(file1Good, file2Good, winner);
        makeOutputFile(4, file1Good, file2Good, winner);
        return;
    }

    // input file/s are valid, now before setting the moves we want to check if maybe there is already a winner...
    if (curGame.RPSGameCheckIfPlayer1Lose() || curGame.RPSGameCheckIfPlayer2Lose()) {// someone wins or both wins
        RPSMainAuxFinalCheckOfGameBoard(winner, reason, newGame);
        makeOutputFile(reason, 0, 0, winner);
        return;
    }

    // now lets read moves files
    param1 = newGame.RPSGameMoveFileCheck("player1.rps_moves", "player2.rps_moves", param2);
    if (param1 == -1) {// tie, both moves files don't exist
        winner = 0;
        RPSMainAuxMakeOutputFile(3, 0, 0, winner, newGame);
    } else if (param1 == 1 || param1 == 2) { //"bad moves input file"
        if (param1 == 1)
            winner = 2;
        else
            winner = 1;
        RPSMainAuxMakeOutputFile(5, param1, param2, winner, newGame); // here param2 == bad line number
    } else { //param1=0 , moves files are OK, we need to check if there is a winner
        RPSMainAuxFinalCheckOfGameBoard(winner, reason, newGame);
        RPSMainAuxMakeOutputFile(reason, param1, param2, winner, newGame);
    }
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



void RPSManager::makeOutputFile(int reason, bool param1, bool param2, int winner) {
    ofstream fout("rps.output");
    int loser;
    int badLine;

    fout << "Winner: " << winner << endl;
    fout << "Reason: ";

    if (reason == 4) { //"bad positioning input file"
        if (!param1 && !param2) //both files are bad
            fout << "Bad Positioning input file for both players - player 1: line " << param1 <<
                 ", player 2: line " << param2 << endl;
        else {
            updateLoserAndBadLine(winner, loser, param1, param2, badLine);
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
        updateLoserAndBadLine(winner, loser, param1, param2, badLine);
        fout << "Bad Moves input file for player " << loser << " - line " << param2 << endl;
    }

    fout << endl;
    // printing the game board state
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            fout << curGame->board->getPiece(i, j);
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


void RPSManager::finalCheckOfGameBoard(int &winner, int &reason) {
    if (curGame.RPSGameCheckIfPlayer1Lose() && curGame.RPSGameCheckIfPlayer2Lose()) { // both wins
        winner = 0;
    } else if (curGame.RPSGameCheckIfPlayer1Lose()) { //player 2 wins
        winner = 2;
        player2Points += 1;
    } else if (curGame.RPSGameCheckIfPlayer2Lose()) {// player 1 wins
        winner = 1;
        player1Points += 1;
    } else { // no one wins
        winner = 0;
        reason = 3; // "A tie - both moves input files done without a winner"
        return;
    }
    checkAndUpdateReasonForWinner(reason);
}


void RPSManager::checkAndUpdateReasonForWinner(int &reason) {
    if (player1->playerToolCounters['F'] == F || player2->playerToolCounters['F'] == F)
        reason = 1; // "all flags of the opponent are captured"
    else
        reason = 2; // "all moving pieces of the opponent are eaten"
}