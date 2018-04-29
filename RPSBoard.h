

#ifndef EX1_GAMEBOARD_H
#define EX1_GAMEBOARD_H

#include "RPSParser.h"
#include "Board.h"
#include "RPSPiecePosition.h"
#include <vector>
#include <map>

using namespace std;

class RPSBoard: public Board {
    char board[N][M];

public:
    friend class RPSGame;

    RPSBoard();

    virtual int getPlayer(const Point & pos) const;

    virtual ~RPSBoard() {}

};


#endif //EX1_GAMEBOARD_H
