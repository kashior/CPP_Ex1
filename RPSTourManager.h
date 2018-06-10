//
// Created by Maria Klimkin on 09-Jun-18.
//

#ifndef EX1_TOURMANAGER_H
#define EX1_TOURMANAGER_H


#include "PlayerAlgorithm.h"
#include "RPSManager.h"
#include <mutex>
#include <thread>
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define VICTORY_SCORE 3;
#define TIE_SCORE 1;
#define BUF_SIZE 1024;

using namespace std;

class RPSTourManager{

    string _directory;
    vector<pair<string, pair<string,bool>>> _gamesQueue;
    list<void *> _my_dl_list; // list to hold handles for dynamic libs
    int _num_of_threads;
    mutex _mutex;
    vector<thread> _list_of_threads;

public:

    static map<string, atomic<int>> _scores;
    static map<string, function<unique_ptr<PlayerAlgorithm>()>> _algorithms;

    RPSTourManager(string dir, int threads){};

    void executeSingleGame(pair<string, pair<string,bool>> players);

    void makeGamesQueue();

    string getRandomPlayer(vector<string> list);

    void printTheScores();

    void loadSOFiles();

    void threadFunction();

    void playTheTournament();

    void singleThreadTournament();

    void START();

    ~RPSTourManager();





};





#endif //EX1_TOURMANAGER_H
