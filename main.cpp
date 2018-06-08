
#include "RPSManager.h"
#include <ctime>


int main(int argc, char* argv[]) {
    srand(time(NULL));
    RPSManager *manager=new RPSManager();
    int x;
    int winners[3]={0,0,0};
    for(int i =0 ; i<5000;i++){
        x=manager->gameHandler();
        winners[x]++;
        manager=new RPSManager();
    }
    cout << winners[0] << ":" << winners[1]<< ":" << winners[2]<<endl;

    return 0;
}