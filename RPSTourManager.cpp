

#include "RPSTourManager.h"


RPSTourManager::RPSTourManager(string dir, int threads) : _directory(dir), _num_of_threads(threads){}


void RPSTourManager::executeSingleGame(pair<string, pair<string,bool>> players) {

    bool countPoints = players.second.second;
    unique_ptr<PlayerAlgorithm> player1 = _algorithms[players.first];
    unique_ptr<PlayerAlgorithm> player2 = _algorithms[players.second];

    RPSManager game = RPSManager(player1, player2);
    int score = game.gameHandler();

    if (score == 1) //player1 won
        _scores[players.first] += VICTORY_SCORE;
    else if (score == 2){ //player2 won
        if (countPoints)
            _scores[players.second] += VICTORY_SCORE;
    }
    else { //tie
        _scores[player1] += TIE_SCORE;
        if (countPoints)
            _scores[players.second] += TIE_SCORE;
    }
}


void RPSTourManager::makeGamesQueue(){

    map<string, int> gamesCounter;
    vector<string> players;
    vector<string> tmpPlayers;

    for (auto &p : gamesCounter) {
        players.insert(players.end(), p.first);
        tmpPlayers.insert(tmpPlayers.end(), p.first);
        gamesCounter[p.first] = 0;
    }

    string player1;
    string player2;
    bool count = true;
    for (auto &p : players){
        player1 = p;
        tmpPlayers.erase(p);
        while (gamesCounter[p] < 30){
            for (int i=0 ; i<30 ; i++){
                player2 = getRandomPlayer();
                if (gamesCounter[player2] >= 30)
                    count = false;
                _gamesQueue.insert(_gamesQueue.end(), {player1, {player2, count}});
                gamesCounter[player1]++;
                gamesCounter[player2]++;
                count = true;
            }
        }
        tmpPlayers.insert(player1);
    }
}


string RPSTourManager::getRandomPlayer(vector<string> list) {
    auto it = list.begin();
    std::advance(it, rand() % list.size());
    return it;
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


void RPSTourManager::threadFunction() {

    unique_lock<mutex> my_lock(_mutex);

    while (!(_gamesQueue.empty())){
        pair<string, pair<string, bool>> playersPair = _gamesQueue.back();
        _gamesQueue.pop_back();
        my_lock.unlock();
        executeSingleGame(playersPair);
        my_lock.lock();
    }
    my_lock.unlock();
}


void RPSTourManager::singleThreadTournament() {

    while (!(_gamesQueue.empty())){
        pair<string, pair<string, bool>> playersPair = _gamesQueue.back();
        _gamesQueue.pop_back();
        executeSingleGame(playersPair);
    }
}


void RPSTourManager::playTheTournament() {

    for (int i=0 ; i<_num_of_threads ; i++){
        _list_of_threads.push_back(thread([this] {threadFunction();}));
    }

    for (auto &t : _list_of_threads){
        if (t.joinable())
            t.join();
    }
}



void RPSTourManager::loadSOFiles() {

    FILE *dl;   // handle to read directory
    const char *command_str = "ls " + _directory +" *.so";  // command string to get dynamic lib names
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
}


void RPSTourManager::START() {
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
    for (list<void *>::iterator itr = my_dl_list.begin(); itr != my_dl_list.end(); itr++)
        dlclose(*itr);
}