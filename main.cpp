
#include "RPSParser.h"
#include "RPSManager.h"


int main(int argc, char* argv[]) {
    if (argc!=2){
        printf("Error: not enough arguments for the game. Exiting\n");
        exit(0);
    }
//    RPSFilePlayerAlgorithm * a=new RPSFilePlayerAlgorithm(1,"/");
    bool isPlayer1Auto, isPlayer2Auto;
    if(RPSParser::parseArguments(isPlayer1Auto,isPlayer2Auto,argv[1])){
        RPSManager gameManager(isPlayer1Auto, isPlayer2Auto);
        gameManager.gameHandler();
    }
    else{
        printf("Error: Invalid argument for game option. Exiting...\n");
        exit(0);
    }

    return 0;
}