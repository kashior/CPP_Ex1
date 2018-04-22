

#ifndef EX1_RPSJOKERCHANGE_H
#define EX1_RPSJOKERCHANGE_H


#include "JokerChange.h"

class RPSJokerChange: public JokerChange  {
    char _jokerRep;
    Point _jokerPosition;
public:
    RPSJokerChange( Point jokerPosition, char jokerRep);
    virtual const Point & getJokerChangePosition() const;
    virtual char getJokerNewRep() const;

    void setJokerChangePosition(RPSPoint pos);

    void setJokerNewRep(char piece);

    //int getPlayer()const;

};


#endif //EX1_RPSJOKERCHANGE_H
