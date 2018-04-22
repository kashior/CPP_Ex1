//
// Created by Or Kashi on 22/04/2018.
//

#ifndef EX1_MOVE_H
#define EX1_MOVE_H


#include "Point.h"

class Move {
public:
    virtual const Point& getFrom() const = 0;
    virtual const Point & getTo() const = 0;

};


#endif //EX1_MOVE_H
