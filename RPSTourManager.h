
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

/**
 * Registers a (possibly) new player algorithm to the tournament
 * @param id
 * @param factoryMethod
 */
    void registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod);

/**
 * Executes a single RPS game by the game manager with a pair of player algorythms.
 * Gets the game result and updates the players total scores so far according to the current game
 * result.
 *
 * @param players - a pair of the players algorithms. First string represents the ID of player1, second string
 * represents the ID of player2, the bool represents whether the score of player2 should be added to its
 * total score or not.
 */
    void executeSingleGame(pair<string, pair<string,bool>> players);

/**
 * Makes a queue that contains all the games that would be performed during the tournament. The queue will contain
 * pairs of the IDs of the players that would play one against the other.
 */
    void makeGamesQueue();

/**
 * returns a random string from a vector of strings
 * @param list - the vector
 * @return the random string
 */
    string getRandomPlayer(vector<string> list);

/**
 * Prints the final score of each player in the tournament from top to low score.
 * First this function sorts all the scores with a special comperator.
 */
    void printTheScores();

    void loadSOFiles();

    void threadFunction();

    void playTheTournament();

    void singleThreadTournament();

    void START();







};





#endif //EX1_TOURMANAGER_H
