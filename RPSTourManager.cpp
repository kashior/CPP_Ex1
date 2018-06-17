

#include <set>
#include "RPSTourManager.h"


RPSTourManager RPSTourManager::theTourManager;

void RPSTourManager::executeSingleGame(pair<string, pair<string,bool>> players) {
    bool countPoints = players.second.second;
    auto player1 = _algorithms[players.first];
    auto player2 = _algorithms[players.second.first];


    RPSManager game = RPSManager(player1, player2);

    int score = game.gameHandler();
    cout << "game finished!" << endl;

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


void RPSTourManager::registerAlgorithm(string id,function<unique_ptr<PlayerAlgorithm>()> factoryMethod){

    if (_algorithms.count(id) >0) //algorithm already registered
        cout << "Warning: RSPPlayer_" << id << " already registered!" << endl;

    _algorithms[id] = factoryMethod; //add to the map of <id,playerAlgorithm>

    _scores[id] = 0; //add to the map of <id,score>, sets score to 0
}


void RPSTourManager::makeGamesQueue(){

    map<string, int> gamesCounter;
    vector<string> players;
    vector<string> tmpPlayers;

    for (auto &p : _algorithms) {
        players.push_back( p.first);
        tmpPlayers.push_back( p.first);
        gamesCounter[p.first] = 0;
    }

    string player1;
    string player2;
    bool count = true;
    for (auto &p : players){
        player1 = p;
        auto it =find(tmpPlayers.begin(), tmpPlayers.end(),player1);
        tmpPlayers.erase(it); //because we want to choose a different algorithm to play against player1
        while (gamesCounter[p] < 30){
                player2 = getRandomPlayer(tmpPlayers);
                if (gamesCounter[player2] >= 30)
                    count = false; //player2 already plays in 30 games
                    _gamesQueue.push_back( {player1, {player2, count}});
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
    cout << "printing scores"<<endl;
    for (pair<string, int> score : sortedScores){
        cout << score.first << " " << score.second << endl;
    }
    cout << "done printing scores"<<endl;
}

/**
 * In case there is more then one thread that can be used, each thread (eccept the main thread) will execute
 * this function, to do the games of the tournament.
 */
void RPSTourManager::threadFunction() {



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

    }

    //make the main thread to work to:
    threadFunction();

    for (auto &t : _list_of_threads){
        if (t.joinable())
            t.join();
    }
}



int RPSTourManager::loadSOFiles() {


    FILE *dl;   // handle to read directory

    char* dir=(char*)malloc(_directory.size()+1);
    copy(_directory.begin(),_directory.end(),dir);
    char *command_str = (char*)malloc(_directory.size()+10);
    strcat(command_str,"ls ");
    if(strcmp("/",dir)!=0)
        strcat(command_str ,dir);
    strcat(command_str," *.so");  // command string to get dynamic lib names

    //string command_str = "ls "+ _directory + "*.so";
    char in_buf[BUF_SIZE]; // input buffer for lib names

    // get the names of all the dynamic libs (.so  files) in the current dir
    dl = popen(command_str, "r");
    if(!dl){
        perror("popen");
        exit(-1);
    }
    void *dlib;
    char name[BUF_SIZE];
    unsigned int num_of_SOs = _algorithms.size();

    while(fgets(in_buf, BUF_SIZE, dl)){
        // trim off the whitespace
        if (in_buf[0]!='R')
            break;
        char *ws = strpbrk(in_buf, " \t\n");
        if(ws) *ws = '\0';
        // append ./ to the front of the lib name
        sprintf(name, "./%s", in_buf);
        dlib = dlopen(name, RTLD_NOW);
        if(dlib == NULL){
            cout << "Error in opening the file " << name << endl;
            cerr << dlerror() << endl;
            exit(-1);
        }

        if (_algorithms.size() != num_of_SOs+1){
            cout << "Error while loading SO file" << endl;
            return -1;
        }

        num_of_SOs++;

        // add the handle to our list

        _my_dl_list.push_back( dlib);
    }

    if (_algorithms.size() < 2){
        cout << "Cannot play a tournament with less then two players!" << endl;
        return -1;
    }

    cout << "done loading " << _my_dl_list.size() << " SO files!" << endl;
    return 0;
}


void RPSTourManager::START() {



    int SOResult;
    SOResult = loadSOFiles();
    if (SOResult == -1)
        return;

    makeGamesQueue();

    if (_num_of_threads == 1)
        singleThreadTournament();
    else
        playTheTournament();

    printTheScores();
}



RPSTourManager::~RPSTourManager(){
    // close all the dynamic libs we opened
//    for (auto itr = _my_dl_list.begin(); itr != _my_dl_list.end(); itr++)
//        dlclose(itr);
}