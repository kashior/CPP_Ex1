

#ifndef EX1_GAMEBOARD_H
#define EX1_GAMEBOARD_H

#include "RPSParser.h"
#include "Board.h"
#include "RPSPiecePosition.h"
#include <vector>
#include <map>

using namespace std;

/**
 * class RPSBoard
 *
 * Represents the board of the game with a 2-dim array of chars.
 *
 */

class RPSBoard: public Board {

    char board[N][M];

public:

    friend class RPSGame;
    friend class RPSManager;

/**
 * Default c'tor.
 * Sets each point on the board to an empty point represented by the char '_'.
 *
 */
    RPSBoard();

/**
 * Returns the number of the player (1 or 2) that its piece occupies the position "pos" on board, or 0
 * this point is empty.
 *
 * @param pos - position on the gameboard
 * @return 0,1 or 2
 */
    virtual int getPlayer(const Point & pos) const;

    /**
     * Return the char representation of the tool on game board at the point (X,Y).
     *
     * @param X
     * @param Y
     * @return
     */
    char getPiece(int X, int Y);

    virtual ~RPSBoard() {}

};


#endif //EX1_GAMEBOARD_H
