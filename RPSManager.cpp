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
    this->curGame = make_unique<RPSGame>();
    player1Points = 0;
    player2Points = 0;
}



void RPSManager::gameHandler(bool isPlayer1Auto, bool isPlayer2Auto) {
    bool file1Good, file2Good;
    int reason; // the "reason" for output file
    // 1=all flags of the opponent are captures
    // 2=All moving PIECEs of the opponent are eaten
    // 3=A tie - both Moves input files done without a winner
    // 4=bad positioning input file for onr player or both
    // 5=bad moves input file for some player
    int winner; // 0=tie, 1=player1 wins, 2=player2 wins

    // checking the board game input files
    if (!isPlayer1Auto)
        file1Good = curGame->RPSGameInitFileCheck(player1, 1);
    if (!isPlayer2Auto)
        file2Good = curGame->RPSGameInitFileCheck(player2, 2);

    if (!file1Good || !file2Good) { //at least one of the positioning input files is bad
        RPSMainAuxUpdateWinner(newGame, param2, winner, param1);
        RPSMainAuxMakeOutputFile(4, param1, param2, winner, newGame);
        return;
    }

    // input files are valid, now before setting the moves we want to check if maybe there is already a winner...
    if (newGame.RPSGameCheckIfPlayer1Lose() || newGame.RPSGameCheckIfPlayer2Lose()) {// someone wins or both wins
        RPSMainAuxFinalCheckOfGameBoard(winner, reason, newGame);
        RPSMainAuxMakeOutputFile(reason, 0, 0, winner, newGame);
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
        curGame.player2Points += 1;
    }
    else { // bad input file for player 2
        winner = 1;
        curGame.player1Points += 1;
    }
}
