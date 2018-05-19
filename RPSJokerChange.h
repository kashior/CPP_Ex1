

#ifndef EX1_RPSJOKERCHANGE_H
#define EX1_RPSJOKERCHANGE_H


#include "JokerChange.h"
#include "RPSPoint.h"

class RPSJokerChange: public JokerChange  {
    char _jokerRep;
    RPSPoint _jokerPosition;

public:

    RPSJokerChange(RPSPoint jokerPosition, char jokerRep);
    RPSJokerChange();
    virtual const Point & getJokerChangePosition() const;
    virtual char getJokerNewRep() const;

    void setJokerChangePosition(RPSPoint pos);

    void setJokerNewRep(char piece);

    virtual ~RPSJokerChange() {}

};


#endif //EX1_RPSJOKERCHANGE_H
