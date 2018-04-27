

#ifndef EX1_POSITIONNEW_H
#define EX1_POSITIONNEW_H


#include "RPSPoint.h"
#include "PiecePosition.h"

class RPSPiecePosition: public PiecePosition {
    RPSPoint _pos;
    char _piece;
    char _jokerRep;
public:

    RPSPiecePosition(Point pos,char piece,char jokerRep='#');

    RPSPiecePosition();

    virtual const Point & getPosition() const;

    virtual char getPiece() const;

    virtual char getJokerRep() const;

    void setPosition(RPSPoint* pos) ;

    void setPosition(int x, int y) ;

    void setPiece(char piece);

    void setJokerRep(char jokerRep);

    bool operator<(const RPSPiecePosition &rhs) const ;

    bool operator==(const RPSPiecePosition &rhs) const ;

    virtual ~RPSPiecePosition() {delete _pos;}

};


#endif //EX1_POSITIONNEW_H
