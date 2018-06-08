

#ifndef EX1_RPSMOVE_H
#define EX1_RPSMOVE_H


#include "Move.h"
#include "RPSPoint.h"
#include "gameDefinitions.h"

/**
 * class RPSMove
 * Represents a move a player wants to execute in the game.
 *
 * _player - 1 or 2
 *
 * _from - the source point that of the tool that the player wants to move
 *
 * _to - the destination point of the tool that the player wants to move
 *
 * _piece - the representation of the tool to move {R,P,S}
 */

class RPSMove : public Move{
    char _piece;
    int _player;
    RPSPoint _from;
    RPSPoint _to;

public:

/**
 * C'tor
 *
 * @param from
 * @param to
 * @param piece
 * @param player
 */
    RPSMove(RPSPoint from,RPSPoint to, char piece, int player);

/**
 * Default c'tor
 */
    RPSMove();

    /**
     *
     * @return the source point of the move
     */
    virtual const Point& getFrom()const;

    /**
     *
     * @return the destination point of the move
     */
    virtual const Point& getTo()const;

    /**
     *
     * @return the player that executes the move
     */
    const int & getPlayer()const;

    /**
     *
     * @return the char representation of the tool to move {R,P,S}
     */
    const char & getPiece()const;

    /**
     * Sets the _piece of the move to "piece"
     * @param piece
     */
    void setPiece(char piece);

    /**
     * Sets the source point to (x,y)
     * @param x
     * @param y
     */
    void setFrom(int x, int y);

    /**
     * Sets the destination point of the move to (x,y)
     * @param x
     * @param y
     */
    void setTo(int x, int y);

    virtual ~RPSMove() {}

};


#endif //EX1_RPSMOVE_H
