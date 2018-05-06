
#include "RPSJokerChange.h"

RPSJokerChange::RPSJokerChange(RPSPoint jokerPosition, char jokerRep = '#'): _jokerRep(jokerRep), _jokerPosition(jokerPosition) {}

RPSJokerChange::RPSJokerChange():  _jokerRep('#'){
    _jokerPosition.setX(-1);
    _jokerPosition.setY(-1);

}
const Point &RPSJokerChange::getJokerChangePosition() const { return _jokerPosition ;}

char RPSJokerChange::getJokerNewRep() const {return _jokerRep;}

void RPSJokerChange::setJokerChangePosition(RPSPoint pos) {_jokerPosition = pos;}

void RPSJokerChange::setJokerNewRep(char piece) {_jokerRep = piece;}


//int RPSJokerChange::getPlayer() const {return }

