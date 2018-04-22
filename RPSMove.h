

#ifndef EX1_RPSMOVE_H
#define EX1_RPSMOVE_H


#include "Move.h"
#include "JokerChange.h"
#include "RPSPoint.h"

class RPSMove : public Move{
    int _player;
    RPSPoint _from;
    RPSPoint _to;
    char _piece;
    bool isJoker; // true iff it's a joker in init stage
//    JokerChange joker;
public:
    RPSMove(Point from, Point to, char piece, int player);
    virtual const Point& getFrom()const;
    virtual const Point& getTo()const;
    const int & getPlayer()const;
    const char & getPiece()const;
    void setPiece(char piece);
    void setFrom(int x, int y);
    void setTo(int x, int y);
    void setJoker();


};


#endif //EX1_RPSMOVE_H
