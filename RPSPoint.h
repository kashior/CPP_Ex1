

#ifndef EX1_POINTONBOARD_H
#define EX1_POINTONBOARD_H


#include "Point.h"

class RPSPoint: public Point {
    int _x, _y;
public:
    RPSPoint(int x, int y);
    RPSPoint();
    RPSPoint(Point pos);
    virtual int getX() const;
    void setX(int x);
    virtual int getY() const;
    void setY(int y);
    virtual ~RPSPoint() {}

};


#endif //EX1_POINTONBOARD_H
