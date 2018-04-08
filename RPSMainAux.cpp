
#include "RPSMainAux.h"

/**
 * In case there is a winner (or two winners) the function checks if the reason for the winning is
 * because all opponents flags are eaten or because all moving tool are eaten, and updates the reason.
 *
 * @param game - the current game
 * @param reason - will be 1 if all flags of the opponent are captured
 *                 or 2 if all moving pieces of the opponent are eaten
 *
 */
void RPSMainAuxCheckAndUpdateReasonForWinner(RPSGame* game, int &reason){
    if (game->player1ToolCounters["F"] == F || game->player1ToolCounters["f"] == F)
        reason = 1; // "all flags of the opponent are captured"
    else
        reason = 2; // "all moving pieces of the opponent are eaten"
}


/**
 * checks if at this point of the game there is winner, and updates the reason and winner parameters
 * @param winner 0, 1 or 2
 * @param reason 1,2,3,4 or 5
 * @param game - the current game
 */
void RPSMainAuxFinalCheckOfGameBoard(int &winner, int &reason, RPSGame* game){
    if (game->RPSGameCheckIfPlayer1Lose() && game->RPSGameCheckIfPlayer2Lose()) { // both wins
        winner = 0;
    }
    else if (game->RPSGameCheckIfPlayer1Lose()) //player 2 wins
        winner = 2;
    else if (game->RPSGameCheckIfPlayer2Lose())// player 1 wins
        winner = 1;
    else { // no one wins
        winner = 0;
        reason = 3; // "A tie - both moves input files done without a winner"
        return;
    }
    RPSMainAuxCheckAndUpdateReasonForWinner(game, reason);

}


/**
 * Updates the winner according to the value of param1 and param2. This is done after
 * reading the positioning input files.
 *
 * @param param1
 * @param param2
 * @param winner
 */
void RPSMainAuxUpdateWinner(int param1, int param2, int &winner){
    if (param1!=0 && param2!=0)
        winner = 0; //tie
    else if (param1 != 0) //bad input file for player 1
        winner = 2;
    else // bad input file for player 2
        winner = 1;
}


int RPSMaimAuxTerminateTheGame(RPSGame* game){
    delete(game);
    return 0;
}


/**
 * This function is called after reading the moves input files, and when one of the lines in one of
 * the files is invalid. Then it updates the loser (according to the winner), and the "bad" line (number of invalid line).
 *
 * @param winner
 * @param loser
 * @param param1
 * @param param2
 * @param badLine
 */
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


/**
 * Writes the output file "rps.output"
 *
 * @param reason
 * @param param1
 * @param param2
 * @param winner
 * @param game
 */
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

    else{ //(reason == 5)
        RPSMainAuxUpdateLoserAndBadLine(winner, loser, param1, param2, badLine);
        fout << "Bad Moves input file for player " << loser << " - line " << param2 << endl;
    }

    fout << endl;
    // printing the game board state
    for (int i=0 ; i<N ; i++){
        for (int j=0 ; j<M ; j++){
            if (game->board[i][j] == "_")
                fout << " ";
            else
                fout << game->board[i][j];
        }
        fout << endl;
    }
    fout.close();
}


/**
 * The handler of the game
 *
 * @return 0
 */
int RPSMainAuxGameHandler(){

    RPSGame* newGame = new RPSGame();
    int param1;
    int param2;
    int reason; // the "reason" for output file
                // 1=all flags of the opponent are captures
                // 2=All moving PIECEs of the opponent are eaten
                // 3=A tie - both Moves input files done without a winner
                // 4=bad positioning input file for onr player or both
                // 5=bad moves input file for some player
    int winner; // 0=tie, 1=player1 wins, 2=player2 wins
    bool flagsEaten = false;

    // checking the board game input files
    param1 = newGame->RPSGameInitFileCheck("player1.rps_board", 1, newGame->player1ToolCounters);
    param2 = newGame->RPSGameInitFileCheck("player2.rps_board", 2, newGame->player2ToolCounters);

    if (param1!=0 || param2!=0){ //at least one of the positioning input files is bad
        RPSMainAuxUpdateWinner(param1, param2, winner);
        RPSMainAuxMakeOutputFile(4, param1, param2, winner, newGame);
        return RPSMaimAuxTerminateTheGame(newGame);
    }

    // input files are valid, now before setting the moves we want to check if maybe there is already a winner...
    if (newGame->RPSGameCheckIfPlayer1Lose() || newGame->RPSGameCheckIfPlayer2Lose()){// someone wins or both wins
        RPSMainAuxFinalCheckOfGameBoard(winner, reason, newGame);
        RPSMainAuxMakeOutputFile(reason, 0, 0, winner, newGame);
        return RPSMaimAuxTerminateTheGame(newGame);
    }

    // now lets read moves files
    param1 = newGame->RPSGameMoveFileCheck("player1.rps_moves", "player2.rps_moves", param2);
    if (param1 == -1){// tie, both moves files don't exist
        winner = 0;
        RPSMainAuxMakeOutputFile(3, 0, 0, winner, newGame);
    }
    else if (param1 == 1 || param1 == 2){ //"bad moves input file"
        if (param1 == 1)
            winner = 2;
        else
            winner = 1;
        RPSMainAuxMakeOutputFile(5, param1, param2, winner, newGame); // here param2 == bad line number
    }
    else{ //param1=0 , moves files are OK, we need to check if there is a winner
        RPSMainAuxFinalCheckOfGameBoard(winner, reason, newGame);
        RPSMainAuxMakeOutputFile(reason, param1, param2, winner, newGame);
    }

    return RPSMaimAuxTerminateTheGame(newGame);
}












