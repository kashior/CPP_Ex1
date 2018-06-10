
#ifndef EX1_TOURMANAGER_H
#define EX1_TOURMANAGER_H


#include "RPSManager.h"
#include <mutex>
#include <thread>
#include <iostream>
#include <list>
#include <string>
#include <dlfcn.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <atomic>


#define VICTORY_SCORE 3
#define TIE_SCORE 1
#define BUF_SIZE 1024


class RPSTourManager{

    static RPSTourManager theTourManager;
    string _directory;
    vector<pair<string, pair<string,bool>>> _gamesQueue;
    list<void *> _my_dl_list; // list to hold handles for dynamic libs
    int _num_of_threads;
    mutex _mutex;
    map<string, atomic<int>> _scores;
    map<string, function<unique_ptr<PlayerAlgorithm>()>> _algorithms;
    vector<thread> _list_of_threads;

    RPSTourManager(){}
    ~RPSTourManager();

public:

    static RPSTourManager& getTourManager(){return theTourManager;}

    void setThreads(int t){_num_of_threads=t;}

    void setDirectory(string d){_directory=d;}

    void registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod);

    void executeSingleGame(pair<string, pair<string,bool>> players);

    void makeGamesQueue();

    string getRandomPlayer(vector<string> list);

    void printTheScores();

    void loadSOFiles();

    void threadFunction();

    void playTheTournament();

    void singleThreadTournament();

    void START();







};





#endif //EX1_TOURMANAGER_H
