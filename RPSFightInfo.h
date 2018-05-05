
#ifndef EX1_RPSFIGHTINFO_H
#define EX1_RPSFIGHTINFO_H


#include "FightInfo.h"
#include "RPSPoint.h"

class RPSFightInfo : public FightInfo {
    int playerWinner;
    char player1Piece;
    char player2Piece;
    RPSPoint fightPosition;

public:
    RPSFightInfo(int winner, char player1Piece, char player2Piece, Point position);

    RPSFightInfo();

    virtual const Point &getPosition() const;

    virtual char getPiece(int player) const;

    virtual int getWinner() const;

    void setPosition(Point newPosition);

    void setPlayer1Piece(char newPiece);

    void setPlayer2Piece(char newPiece);

    void setWinner(int newWinner);

    virtual ~RPSFightInfo() {}

};


#endif //EX1_RPSFIGHTINFO_H
