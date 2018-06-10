

#include <set>
#include "RPSTourManager.h"

RPSTourManager RPSTourManager::theTourManager;


void RPSTourManager::executeSingleGame(pair<string, pair<string,bool>> players) {

    bool countPoints = players.second.second;
    unique_ptr<PlayerAlgorithm> player1 =move(_algorithms[players.first]());
    unique_ptr<PlayerAlgorithm> player2 = move(_algorithms[players.second.first]());

    RPSManager game = RPSManager(move(player1), move(player2));
    int score = game.gameHandler();

    if (score == 1) //player1 won
        _scores[players.first] += VICTORY_SCORE;
    else if (score == 2){ //player2 won
        if (countPoints)
            _scores[players.second.first] += VICTORY_SCORE;
    }
    else { //tie
        _scores[players.first] += TIE_SCORE;
        if (countPoints)
            _scores[players.second.first] += TIE_SCORE;
    }
}


void RPSTourManager::registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod){

    if (_algorithms[id] != NULL) //algorithm already registered
        cout << "Warning: RSPPlayer_" << id << " already registered!" << endl;

    else{
        _algorithms[id] = factoryMethod; //add to the map of <id,playerAlgorithm>
        _scores[id] = 0; //add to the map of <id,score>, sets score to 0
    }
}


void RPSTourManager::makeGamesQueue(){

    map<string, int> gamesCounter;
    vector<string> players;
    vector<string> tmpPlayers;

    for (auto &p : _algorithms) {
        players.insert(players.end(), p.first);
        tmpPlayers.insert(tmpPlayers.end(), p.first);
        gamesCounter[p.first] = 0;
    }

    string player1;
    string player2;
    bool count = true;
    for (auto &p : players){
        player1 = p;
        auto it =find(tmpPlayers.begin(), tmpPlayers.end(),p);
        tmpPlayers.erase(it); //because we want to choose a different algorithm to play against player1
        while (gamesCounter[p] < 30){
                player2 = getRandomPlayer(tmpPlayers);
                if (gamesCounter[player2] >= 30)
                    count = false; //player2 already plays in 30 games
                _gamesQueue.insert(_gamesQueue.end(), {player1, {player2, count}});
                gamesCounter[player1]++;
                gamesCounter[player2]++;
                count = true;
        }
        tmpPlayers.push_back(player1); //return the player to the vector
    }


    cout << "Done making the games queue!! there are total " << _gamesQueue.size() << " games to play!" << endl;
}


string RPSTourManager::getRandomPlayer(vector<string> list) {
    auto it = list.begin();
    std::advance(it, rand() % list.size());
    return *it;
}


void RPSTourManager::printTheScores() {

    typedef function<bool(pair<string, int>, pair<string, int>)> Comparator;

    Comparator comparisonFunc = [](pair<string, int> param1, pair<string, int> param2){
        return param1.second > param2.second;
    };

    set<pair<string, int>, Comparator> sortedScores(_scores.begin(), _scores.end(), comparisonFunc);

    for (pair<string, int> score : sortedScores){
        cout << score.first << " " << score.second << endl;
    }
}

/**
 * In case there is more then one thread that can be used, each thread (eccept the main thread) will execute
 * this function, to do the games of the tournament.
 */
void RPSTourManager::threadFunction() {


    cout << "entered threadFunction!" << endl;



    while (true){
        _mutex.lock();
        if (!_gamesQueue.empty()){ //check if there are still more games to play in the tournament
            pair<string, pair<string, bool>> playersPair = _gamesQueue.back();
            _gamesQueue.pop_back(); //get this game out of the queue
            _mutex.unlock();
            executeSingleGame(playersPair);
        }
        else{ //no more games to play in the tournament
            _mutex.unlock();
            break;
        }
    }
}


void RPSTourManager::singleThreadTournament() {

    while (!(_gamesQueue.empty())){
        pair<string, pair<string, bool>> playersPair = _gamesQueue.back();
        _gamesQueue.pop_back();
        executeSingleGame(playersPair);
    }
}


void RPSTourManager::playTheTournament() {

    for (int i=0 ; i<_num_of_threads-1 ; i++){
        _list_of_threads.push_back(thread([this] {threadFunction();}));

        cout << "1 thread statrted working now!" << endl;
    }

    //make the main thread to work to:
    threadFunction();

    for (auto &t : _list_of_threads){
        if (t.joinable())
            t.join();
    }
}



void RPSTourManager::loadSOFiles() {

    FILE *dl;   // handle to read directory
    char* dir=(char*)malloc(_directory.size()+1);
    copy(_directory.begin(),_directory.end(),dir);
    char *command_str = (char*)malloc(_directory.size()+10);
    strcat(command_str,"ls ");
    strcat(command_str ,dir);
    strcat(command_str," *.so");  // command string to get dynamic lib names
    char in_buf[BUF_SIZE]; // input buffer for lib names

    // get the names of all the dynamic libs (.so  files) in the current dir
    dl = popen(command_str, "r");
    if(!dl){
        perror("popen");
        exit(-1);
    }
    void *dlib;
    char name[BUF_SIZE];
    while(fgets(in_buf, BUF_SIZE, dl)){
        // trim off the whitespace
        char *ws = strpbrk(in_buf, " \t\n");
        if(ws) *ws = '\0';
        // append ./ to the front of the lib name
        sprintf(name, "./%s", in_buf);
        dlib = dlopen(name, RTLD_NOW);
        if(dlib == NULL){
            cerr << dlerror() << endl;
            exit(-1);
        }
        // add the handle to our list
        _my_dl_list.insert(_my_dl_list.end(), dlib);
    }


    cout << "done loading " << _my_dl_list.size() << " SO files!" << endl;


}


void RPSTourManager::START() {


    cout << "started the tournament!"<< endl;


    loadSOFiles();
    makeGamesQueue();

    if (_num_of_threads == 1)
        singleThreadTournament();
    else
        playTheTournament();

    printTheScores();
}



RPSTourManager::~RPSTourManager(){
    // close all the dynamic libs we opened
    for (list<void *>::iterator itr = _my_dl_list.begin(); itr != _my_dl_list.end(); itr++)
        dlclose(*itr);
}