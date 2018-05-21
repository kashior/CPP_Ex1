

#ifndef EX1_RPSJOKERCHANGE_H
#define EX1_RPSJOKERCHANGE_H


#include "JokerChange.h"
#include "RPSPoint.h"

/**
 * class RPSJokerChange
 * Represents a joker change that one of the players wants to execute.
 *
 * _jokerRep - the new joker representation
 *
 * _jokerPosition - the point on the board where the joker the player wishes to change
 */

class RPSJokerChange: public JokerChange  {
    char _jokerRep;
    RPSPoint _jokerPosition;

public:

    /**
     * C'tor for RPSJokerChange. Sets the position and the char rep.
     *
     * @param jokerPosition
     * @param jokerRep
     */
    RPSJokerChange(RPSPoint jokerPosition, char jokerRep);

    /*
     * Default c'tor. This is mainly for a move without a joker change ('#')- to know that
     * there is no joker change in a certain move during the game.
     */
    RPSJokerChange();

    /**
     *
     * @return the point on board of the joker.
     */
    virtual const Point & getJokerChangePosition() const;

    /**
     *
     * @return the char representation of the joker
     */
    virtual char getJokerNewRep() const;

    /**
     * Sets the joker position to be "pos"
     * @param pos
     */
    void setJokerChangePosition(RPSPoint pos);

    /**
     * Sets the char representation of the joker to be "piece"
     *
     * @param piece
     */
    void setJokerNewRep(char piece);

    virtual ~RPSJokerChange() {}

};


#endif //EX1_RPSJOKERCHANGE_H
