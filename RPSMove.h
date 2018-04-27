

#ifndef EX1_RPSMOVE_H
#define EX1_RPSMOVE_H


#include "Move.h"
#include "RPSJokerChange.h"
#include "RPSPoint.h"
#include <memory>
using namespace std;

class RPSMove : public Move{
    int _player;
    RPSPoint _from;
    RPSPoint _to;
    char _piece;
    unique_ptr<RPSJokerChange> _joker;

public:
    RPSMove(Point from, Point to, char piece, int player);
    RPSMove();
    virtual const Point& getFrom()const;
    virtual const Point& getTo()const;
    const int & getPlayer()const;
    const char & getPiece()const;
    unique_ptr<JokerChange> getJoker() const;
    void setPiece(char piece);
    void setFrom(int x, int y);
    void setTo(int x, int y);
    void setJoker(char rep, RPSPoint* pos);
    virtual ~RPSMove() {}

};


#endif //EX1_RPSMOVE_H
