

#ifndef EX1_RPSJOKERCHANGE_H
#define EX1_RPSJOKERCHANGE_H


#include "JokerChange.h"
#include "RPSPoint.h"

class RPSJokerChange: public JokerChange  {
    char _jokerRep;
    Point _jokerPosition;
public:
    RPSJokerChange( Point jokerPosition, char jokerRep);
    RPSJokerChange();
    virtual const Point & getJokerChangePosition() const;
    virtual char getJokerNewRep() const;

    void setJokerChangePosition(RPSPoint pos);

    void setJokerNewRep(char piece);

    virtual ~RPSJokerChange() {}

    //int getPlayer()const;
};


#endif //EX1_RPSJOKERCHANGE_H
