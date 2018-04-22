//
// Created by Or Kashi on 22/04/2018.
//

#ifndef EX1_BOARD_H
#define EX1_BOARD_H


#include "Point.h"

class Board {
public:

    virtual int getPlayer(const Point & pos) const = 0;
};


#endif //EX1_BOARD_H
