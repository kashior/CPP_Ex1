
#ifndef EX1_RPSFIGHTINFO_H
#define EX1_RPSFIGHTINFO_H


#include "FightInfo.h"
#include "RPSPoint.h"

class RPSFightInfo : public FightInfo {
    int playerWinner;
    char player1Piece;
    char player2Piece;
    RPSPoint fightPosition;
    bool isFight; //to know if there was a fight for the fights counter

public:
    RPSFightInfo(int winner, char player1Piece, char player2Piece, RPSPoint position, bool isFight);

    RPSFightInfo();

    virtual const Point &getPosition() const;

    virtual char getPiece(int player) const;

    virtual int getWinner() const;

    void setPosition(const RPSPoint &newPosition);

    void setPlayer1Piece(char newPiece);

    void setPlayer2Piece(char newPiece);

    void setWinner(int newWinner);

    void setIsFight(bool b);

    bool getIsFight();

    virtual ~RPSFightInfo() {}

};


#endif //EX1_RPSFIGHTINFO_H
