

#ifndef EX1_GAMEBOARD_H
#define EX1_GAMEBOARD_H

#include "RPSParser.h"
#include "Board.h"
#include "RPSPiecePosition.h"
#include <vector>
using namespace std;

class RPSBoard: public Board {
    char board[N][M];
    vector<RPSPiecePosition> player1Jokers;
    vector<RPSPiecePosition> player2Jokers;
public:
    RPSBoard();
    virtual int getPlayer(const Point & pos) const;


};


#endif //EX1_GAMEBOARD_H
