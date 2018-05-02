//
// Created by Maria Klimkin on 01-May-18.
//

#ifndef EX1_RPSMANAGER_H
#define EX1_RPSMANAGER_H

#include "RPSGame.h"
#include "RPSAutoPlayerAlgorithm.h"
#include "RPSFilePlayerAlgorithm.h"


class RPSManager{

    unique_ptr<RPSGame> curGame;
    unique_ptr<RPSPlayerAlgorithm> player1;
    unique_ptr<RPSPlayerAlgorithm> player2;
    int player1Points;
    int player2Points;

public:

    friend class RPSGame;

    RPSManager(bool isPlayer1Auto, bool isPlayer2Auto, int player1Points, int player2Points);

    void gameHandler(bool isPlayer1Auto, bool isPlayer2Auto);

    void updateWinner(bool param1, bool param2, int &winner);

    void makeOutputFile(int reason, bool param1, bool param2, int winner);

    void updateLoserAndBadLine(int winner, int &loser, int param1, int param2, int &badLine);

    void finalCheckOfGameBoard(int &winner, int &reason);

    void checkAndUpdateReasonForWinner(int &reason);

};



#endif //EX1_RPSMANAGER_H
