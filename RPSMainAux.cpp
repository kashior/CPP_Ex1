
#include "RPSMainAux.h"


void RPSMainAuxFinalCheckOfGameBoard(int &winner, int &reason, RPSGame* game){
    if (game->RPSGameCheckIfPlayer1Lose() && game->RPSGameCheckIfPlayer2Lose()) // both wins
        winner = 0;
    else if (game->RPSGameCheckIfPlayer1Lose()) //player 2 wins
        winner = 2;
    else if (game->RPSGameCheckIfPlayer2Lose())// player 1 wins
        winner = 1;
    else { // no one wins
        winner = 0;
        reason = 3; // "A tie - both moves input files done without a winner"
        return;
    }
    if (game->player1ToolCounters["F"] == F || game->player1ToolCounters["f"] == F)
        reason = 1; // "all flags of the opponent are captured"
    else
        reason = 2; // "all moving pieces of the opponent are eaten"
}


int RPSMainAuxGameHandler(){

    RPSGame* newGame = new RPSGame();
    int param1;
    int param2;
    int reason; //"reason" for output file
                // 1=all flags of the opponent are captures
                // 2= , 3=, 4=, 5=, 6=bad moves input file for player...
    int winner; // 0=tie, 1=player1, 2=player2
    bool flagsEaten = false;

    // checking the board game input files
    param1 = newGame->RPSGameInitFileCheck("player1.rps_board", 1, newGame->player1ToolCounters);
    param2 = newGame->RPSGameInitFileCheck("player2.rps_board", 2, newGame->player2ToolCounters);
    if (param1 != 0 && param2 != 0){ //at least one of the files is bad
        RPSMainAuxUpdateWinner(param1, param2, winner);
        RPSMainAuxMakeOutputFile(4, param1, param2, winner, newGame);
        return 0;
    }
    // input files are valid, now before setting the moves we want to check if maybe there is a winner...
    if (newGame->RPSGameCheckIfPlayer1Lose() || newGame->RPSGameCheckIfPlayer2Lose()){// someone wins or both wins
        RPSMainAuxFinalCheckOfGameBoard(winner, reason, newGame);
        RPSMainAuxMakeOutputFile(reason, 0, 0, winner, newGame);
        return 0;
    }
    // now lets read moves files
    param1 = newGame->RPSGameMoveFileCheck("player1.rps_moves", "player2.rps_moves", param2);
    if (param1 == 0){// tie, both moves files don't exist
        winner = 0;
        RPSMainAuxMakeOutputFile(3, 0, 0, winner, newGame);
        return 0;
    }
    else if (param1 == 1 || param1 == 2){ //"bad moves input file"
        if (param1 == 1)
            winner = 2;
        else
            winner = 1;
        RPSMainAuxMakeOutputFile(5, param1, param2, winner, newGame); // here param2 == bad line number
        return 0;
    }
    else{ //param1=3 , moves files are OK, we need to check if there is a winner
        RPSMainAuxFinalCheckOfGameBoard(winner, reason, newGame);
        RPSMainAuxMakeOutputFile(reason, param1, param2, winner, newGame);
        return 0;
    }




}


void RPSMainAuxUpdateWinner(int param1, int param2, int &winner){
    if (param1 != 0) //bad input file for player 1
        winner = 2;
    else // bad input file for player 2
        winner = 1;
}



void RPSMainAuxUpdateLoserAndBadLine(int winner, int &loser, int param1, int param2 , int &badLine){
    if (winner == 1) {
        loser = 2;
        badLine = param2;
    }
    else{
        loser = 1;
        badLine = param1;
    }
}


void RPSMainAuxMakeOutputFile(int reason, int param1, int param2, int winner, RPSGame* game){
    ofstream fout("rps.output");
    int loser;
    int badLine;

    fout << "Winner: " << winner << endl;

    if (reason == 4){ //"bad positioning input file"
        if (param1 != 0 && param2 != 0) //both files are bad
            fout << "Bad Positioning input file for both players - player 1: line " << param1 <<
                 ", player 2: line " << param2 << endl;
        else{
            RPSMainAuxUpdateLoserAndBadLine(winner, loser, param1, param2 , badLine);
            fout << "Bad Positioning input file for player "<< loser << " - line " << badLine << endl;
        }
    }

    else if (reason == 1 || reason == 2){
        if (reason == 1)
            fout << "All flags of the opponent are captured" << endl;
        else
            fout << "All moving PIECEs of the opponent are eaten" << endl;
    }

    else if (reason == 3)
        fout << "A tie - both Moves input files done without a winner" << endl;

    else if (reason == 5){
        RPSMainAuxUpdateLoserAndBadLine(winner, loser, param1, param2, badLine);
        fout << "Bad Moves input file for player " << loser << " - line" << param2 << endl;
    }

}


