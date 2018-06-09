
#include "RPSTourManager.h"
#include <ctime>


int main(int argc, char* argv[]) {
    srand(time(NULL));
//    unique_ptr<PlayerAlgorithm> player1 = make_unique<RSPPlayer_203022041>(1);
//    unique_ptr<PlayerAlgorithm> player2 = make_unique<RSPPlayer_203022041>(2);
//    RPSManager *manager=new RPSManager(player1, player2);
//    int x;
//    int winners[3]={0,0,0};
//    for(int i =0 ; i<5000;i++){
//        x=manager->gameHandler();
//        winners[x]++;
//        manager=new RPSManager(player1, player2);
//    }
//    cout << winners[0] << ":" << winners[1]<< ":" << winners[2]<<endl;

    string directory = "";
    int threads = 4;

    switch (argc) {
        case 1:
            break;
        case 3:
            if (argv[1] == "-threads")
                threads = stoi(argv[2]);
            else if (argv[1] == "-path")
                directory = argv[2];
        case 5:
            if (argv[1] == "-threads" && argv[3] == "-path"){
                threads = stoi(argv[2]);
                directory = argv[4];
            }
            else if (argv[3] == "-threads" && argv[1] == "-path"){
                threads = stoi(argv[4]);
                directory = argv[2];
            }
        default:
            cout << "Usage: ./ex3 [-threads threadsNumber] [-path path]" << endl;
            return 0;
    }

    if (directory.back() != '/')
        directory.push_back('/');

    // initiate static members
    RPSTourManager::_scores;
    RPSTourManager::_algorithms;
    RPSTourManager* manager = new RPSTourManager(directory, threads);
    manager->START();

    return 0;
}