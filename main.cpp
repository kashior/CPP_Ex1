
#include "RPSMainAux.h"
#include "RPSManager.h"


int main(int argc, char* argv[]) {
    if (argc!=2){
        printf("Error: not enough arguments for the game. Exiting\n");
        exit(0);
    }
    RPSManager gameManager;
    bool isPlayer1Auto, isPlayer2Auto;
    if(gameManager.parseArguments(isPlayer1Auto, isPlayer2Auto, argv[1]))
        gameManager.gameHandler(isPlayer1Auto,isPlayer2Auto);
    else{
        printf("Error: Invalid argument for game option. Exiting...\n");
        exit(0);
    }

    return 0;
}