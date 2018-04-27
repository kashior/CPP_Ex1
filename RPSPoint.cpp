

#include "RPSPoint.h"

RPSPoint::RPSPoint(int x, int y): _x(x),_y(y) {}
RPSPoint::RPSPoint() {}

RPSPoint::RPSPoint(Point pos):_x(pos.getX()), _y(pos.getY()) {}

int RPSPoint::getX() const { return _x; }

int RPSPoint::getY() const { return _y; }

void RPSPoint::setX(int x) { _x=x; }

void RPSPoint::setY(int y) { _y=y; }
