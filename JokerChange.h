//
// Created by Or Kashi on 22/04/2018.
//

#ifndef EX1_JOKERCHANGE_H
#define EX1_JOKERCHANGE_H


#include "Point.h"

class JokerChange {
public:

    virtual const Point & getJokerChangePosition() const = 0;
    virtual char getJokerNewRep() const = 0;
};


#endif //EX1_JOKERCHANGE_H
