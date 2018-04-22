//
// Created by Or Kashi on 22/04/2018.
//

#ifndef EX1_PIECEPOSITION_H
#define EX1_PIECEPOSITION_H


#include "Point.h"

class PiecePosition {
    public:
    virtual const Point & getPosition() const =0;
    virtual char getPiece() const =0;
    virtual char getJokerRep() const = 0;

};


#endif //EX1_PIECEPOSITION_H
