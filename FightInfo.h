//
// Created by Or Kashi on 22/04/2018.
//

#ifndef EX1_FIGHTINFO_H
#define EX1_FIGHTINFO_H


#include "Point.h"

class FightInfo {
public:
    virtual const Point & getPosition() const = 0;
    virtual char getOpponentPiece() const = 0;
    virtual int getWinner() const = 0;

};


#endif //EX1_FIGHTINFO_H
