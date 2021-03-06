

#include "RPSPoint.h"

RPSPoint::RPSPoint(int x, int y): _x(x),_y(y) {}
RPSPoint::RPSPoint() {
    _x=-1;
    _y=-1;
}

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
    if(_x==p2._x)
        return _y<p2._y;
    return _x <p2._x;

}


