

#ifndef EX1_POINTONBOARD_H
#define EX1_POINTONBOARD_H


#include "Point.h"
/**
 * a class that inherits from Point abstract class. Implementing our version of a point on the game board
 */
class RPSPoint: public Point {
    int _x, _y;

public:
    //<constructors>
    RPSPoint(int x, int y);

    RPSPoint();

    RPSPoint(const RPSPoint &p2);//copy constructor
    //</constructors>

    /**
     * overriding operator == in order to have the ability to compare between two objects in this class
     */
    bool operator==(const RPSPoint& p2)const;
    /**
     * overriding operator < for comparision and also to have the ability to create maps and vectors of this class type
     */
    bool operator<(const RPSPoint& p2)const;

    /**
     * get the X rep of this Point
     */
    virtual int getX() const;

    /**
     * Set the X rep of this Point
     */
    void setX(int x);

    /**
     * get the Y rep of this Point
     */
    virtual int getY() const ;

    /**
     * Set the Y rep of this Point
     */
    void setY(int y);

    //distructor
    virtual ~RPSPoint() {}
};


#endif //EX1_POINTONBOARD_H
