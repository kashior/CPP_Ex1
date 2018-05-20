

#ifndef EX1_POSITIONNEW_H
#define EX1_POSITIONNEW_H


#include "RPSPoint.h"
#include "PiecePosition.h"

/**
 * Class that inherits from PiecePosition abstract class
 * Represents a position in our game, contains a RPSPoint in the board, the piece in it and if it's a joker, so it's rep
 */

class RPSPiecePosition: public PiecePosition {
    RPSPoint _pos;
    char _piece;
    char _jokerRep;

public:
    //<constructors>
    RPSPiecePosition(RPSPoint pos, char piece, char jokerRep = '#');

    RPSPiecePosition();
    //</constructors>

    /**
     *
     * @return the Point of this object(dynamically will get RPSPoint)
     */
    virtual const Point & getPosition() const;
    /**
     *
     * @return the piece of this object
     */
    virtual char getPiece() const;
    /**
     *
     * @return the joker rep of this object
     */
    virtual char getJokerRep() const;
    /**
     * Set new Position given a point
     * @param pos
     */
    void setPosition(RPSPoint* pos) ;
    /**
     * Set new piece
     * @param piece
     */
    void setPiece(char piece);
    /**
     * Set new joker representation
     * @param jokerRep
     */
    void setJokerRep(char jokerRep);
    /**
     *
     * overriding operator < for comparision and also to have the ability to create maps and vectors of this class type
     */
    bool operator<(const RPSPiecePosition &rhs) const ;
    /**
     * overriding operator == in order to have the ability to compare between two objects in this class
     */
    bool operator==(const RPSPiecePosition &rhs) const ;

    //distructor
    virtual ~RPSPiecePosition() {}

};


#endif //EX1_POSITIONNEW_H
