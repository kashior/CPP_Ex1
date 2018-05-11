

#include "RPSPoint.h"

RPSPoint::RPSPoint(int x, int y): _x(x),_y(y) {}
RPSPoint::RPSPoint() {
    _x=-1;
    _y=-1;
}

//RPSPoint::RPSPoint(RPSPoint& pos): _x(pos.getX()), _y(pos.getY()) {}

int RPSPoint::getX() const { return _x; }

int RPSPoint::getY() const { return _y; }

void RPSPoint::setX(int x) { _x=x; }

void RPSPoint::setY(int y) { _y=y; }

RPSPoint::RPSPoint(const RPSPoint &p2) {
    {_x = p2._x; _y = p2._y; }

}

bool RPSPoint::operator==(const RPSPoint &p2)const {
    return p2._x==_x && p2._y==_y;
}

bool RPSPoint::operator<(const RPSPoint &p2)const {
    if(*this==p2)
        return false;
    if(p2._x>_x)
        return true;
    return p2._y>_y;

}

