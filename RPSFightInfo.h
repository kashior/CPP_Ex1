
#ifndef EX1_RPSFIGHTINFO_H
#define EX1_RPSFIGHTINFO_H


#include "FightInfo.h"
#include "RPSPoint.h"

class RPSFightInfo : public FightInfo {
    int playerWinner;
    char opponentPiece;
    RPSPoint fightPosition;
public:
    RPSFightInfo(int winner, char piece, Point position);

    virtual const Point &getPosition() const;

    virtual char getOpponentPiece() const;

    virtual int getWinner() const;

    void setPosition(Point newPosition);

    void setOpponentPiece(char newPiece);

    void setWinner(int newWinner);

    virtual ~RPSFightInfo() {}

};


#endif //EX1_RPSFIGHTINFO_H
