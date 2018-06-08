
#include "RPSManager.h"
#include <ctime>


int main(int argc, char* argv[]) {
    srand(time(NULL));
    RPSManager *manager=new RPSManager();
    int x;
    for(int i =0 ; i<200;i++){
        x=manager->gameHandler();
        manager=new RPSManager();
    }

    return x;
}