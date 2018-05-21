
#ifndef EX1_RPSFIGHTINFO_H
#define EX1_RPSFIGHTINFO_H


#include "FightInfo.h"
#include "RPSPoint.h"

/**
 * class RPSFightInfo
 *
 * Represents a fight that occurred during the game.
 *
 * playerWinner - =1 if players 1 tool won the fight, =2 if a tool of player 2 won the fight
 *
 * player1Piece - the char representation of player 1s tool in the fight
 *
 * player2Piece - the char representation of player 2s tool in the fight
 *
 * fightPosition - the point on the board where the fight took place
 *
 * isFight - =true if there was a fight, =false otherwise
 *
 */

class RPSFightInfo : public FightInfo {
    int playerWinner;
    char player1Piece;
    char player2Piece;
    RPSPoint fightPosition;
    bool isFight; //to know if there was a fight for the fights counter

public:

    /**
     * C'tor.
     *
     * @param winner
     * @param player1Piece
     * @param player2Piece
     * @param position
     * @param isFight
     */
    RPSFightInfo(int winner, char player1Piece, char player2Piece, RPSPoint position, bool isFight);

    /**
     * Default c'tor for an empty fight (no fight).
     */
    RPSFightInfo();

    /**
     *
     * @return the point on board where the fight occurred
     */
    virtual const Point &getPosition() const;

    /**
     * Returns the piece of player number "player" in this fight.
     *
     * @param player
     * @return
     */
    virtual char getPiece(int player) const;

    /**
     *
     * @return - 1 if player 1s piece won the fight
     *         - 2 if player 2s piece won the fight
     *         - 0 if both pieces "died"
     */
    virtual int getWinner() const;

    /**
     * Sets the position of the fight to be "newPosition"
     *
     * @param newPosition
     */
    void setPosition(const RPSPoint &newPosition);

    /*
     * Sets player1Piece to be "newPiece"
     *
     */
    void setPlayer1Piece(char newPiece);

    /*
     * Sets player2Piece to be "newPiece"
     *
     */
    void setPlayer2Piece(char newPiece);

    /**
     * Sets playerWinner to be "newWinner"
     *
     * @param newWinner
     */
    void setWinner(int newWinner);

    /**
     * Sets isFight to be "b"
     *
     * @param b
     */
    void setIsFight(bool b);

    /**
     *
     * @return the value of isFight of this fight
     */
    bool getIsFight();

    virtual ~RPSFightInfo() {}

};


#endif //EX1_RPSFIGHTINFO_H
