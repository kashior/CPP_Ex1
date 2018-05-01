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
    unique_ptr<PlayerAlgorithm> player1;
    unique_ptr<PlayerAlgorithm> player2;
    int player1Points;
    int player2Points;

public:

    friend class RPSGame;

    RPSManager(bool isPlayer1Auto, bool isPlayer2Auto, int player1Points, int player2Points);

    void gameHandler(bool isPlayer1Auto, bool isPlayer2Auto);

    void updateWinner(bool param1, bool param2, int &winner);


};



#endif //EX1_RPSMANAGER_H
