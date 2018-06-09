
#include "RPSManager.h"
#include <ctime>


int main(int argc, char* argv[]) {
    srand(time(NULL));
    unique_ptr<PlayerAlgorithm> player1 = make_unique<RSPPlayer_203022041>(1);
    unique_ptr<PlayerAlgorithm> player2 = make_unique<RSPPlayer_203022041>(2);
    RPSManager *manager=new RPSManager(player1, player2);
    int x;
    int winners[3]={0,0,0};
    for(int i =0 ; i<5000;i++){
        x=manager->gameHandler();
        winners[x]++;
        manager=new RPSManager(player1, player2);
    }
    cout << winners[0] << ":" << winners[1]<< ":" << winners[2]<<endl;

    return 0;
}