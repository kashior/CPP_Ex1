

#ifndef EX1_POSITIONNEW_H
#define EX1_POSITIONNEW_H


#include "RPSPoint.h"
#include "PiecePosition.h"

class RPSPiecePosition: public PiecePosition {
    RPSPoint _pos;
    char _piece;
    char _jokerRep;
public:

    RPSPiecePosition(Point pos,char piece,char jokerRep);

    virtual const Point & getPosition() const;
    virtual char getPiece() const;
    virtual char getJokerRep() const;

    void setPosition(RPSPoint* pos) const;
    void setPiece(char piece) const;
    void setJokerRep(char jokerRep) const;

    bool operator<(const RPSPiecePosition &rhs) const ;

    bool operator==(const RPSPiecePosition &rhs) const ;

};


#endif //EX1_POSITIONNEW_H