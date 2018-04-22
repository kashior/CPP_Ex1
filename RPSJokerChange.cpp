//
// Created by Or Kashi on 22/04/2018.
//

#include "RPSJokerChange.h"

RPSJokerChange::RPSJokerChange(Point jokerPosition, char jokerRep='#'): _jokerRep(jokerRep), _jokerPosition(jokerPosition) {}

const Point &RPSJokerChange::getJokerChangePosition() const { return _jokerPosition ;}

char RPSJokerChange::getJokerNewRep() const {return _jokerRep;}

void setJokerChangePosition(RPSPoint pos) {_jokerPosition = pos;}

void setJokerNewRep(char piece) {_jokerRep = piece;}

//int RPSJokerChange::getPlayer() const {return }

