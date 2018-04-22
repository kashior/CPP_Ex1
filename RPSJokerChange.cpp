//
// Created by Or Kashi on 22/04/2018.
//

#include "RPSJokerChange.h"

RPSJokerChange::RPSJokerChange(char jokerRep, Point jokerPosition): _jokerRep(jokerRep), _jokerPosition(jokerPosition) {}

const Point &RPSJokerChange::getJokerChangePosition() const { return _jokerPosition ;}

char RPSJokerChange::getJokerNewRep() const {return _jokerRep;}

int RPSJokerChange::getPlayer() const {return }

