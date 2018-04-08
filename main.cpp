

#include "RPSMainAux.h"


int main() {


    return RPSMainAuxGameHandler();


    /**
     *
    RPSGame newGame;
    newGame.RPSGameInitFileCheck("player1.rps_board",1,newGame.player1ToolCounters);
    newGame.RPSGameInitFileCheck("player2.rps_board",2,newGame.player2ToolCounters);
    if(newGame.RPSGameCheckIfPlayer1Lose() && newGame.RPSGameCheckIfPlayer2Lose()){
        cout << "It's a tie!" << endl;
    }
    else if(newGame.RPSGameCheckIfPlayer1Lose()) {
        newGame.player2Points += 1;
        cout << "Player 2 wins" << endl;
    }
    else if(newGame.RPSGameCheckIfPlayer1Lose()) {
        newGame.player1Points += 1;
        cout << "Player 1 wins" << endl;
    }
    int lineNum;
    int res=newGame.RPSGameMoveFileCheck("player1.rps_moves","player2.rps_moves",lineNum);
    switch(res){
        case -1:
            cout << "tie. both move files are shit" << endl;
            break;
        case 0:
            cout << "tie, the move files left both players up and running" << endl;
            break;
        case 1:
            cout << "player 2 win. player 1's file corrupted" << endl;
            break;
        case 2:
            cout << "player 1 win. player 1's file corrupted" << endl;
            break;
        case 3:
            cout << "player 1 did better moves." << endl;
            break;
        case 4:
            cout << "player2 did better moves." << endl;
            break;
        case 5:
            cout << "tie...both players lost" << endl;
            break;
        default:
            cout << "weird..." << endl;
    }

    map<string, int>::iterator it;
    for (it = newGame.player1ToolCounters.begin(); it != newGame.player1ToolCounters.end(); it++) {
        printf("%d",it->second);
        cout << it->first <<  "\t";

    }
    printf("\n");
    for (it = newGame.player2ToolCounters.begin(); it != newGame.player2ToolCounters.end(); it++) {
        printf("%d",it->second);
        cout << it->first<<  "\t";

    }
     */
}