

#include "RPSFightInfo.h"

RPSFightInfo::RPSFightInfo(int winner, char player1Piece, char player2Piece, RPSPoint position)
        : playerWinner(winner), player1Piece(player1Piece), player2Piece(player2Piece),
          fightPosition(position) {}

RPSFightInfo::RPSFightInfo(): playerWinner(0), player1Piece('\0'), player2Piece('\0')
{
    fightPosition.setX(-1);
    fightPosition.setY(-1);
}

const Point &RPSFightInfo::getPosition() const { return fightPosition; }

char RPSFightInfo::getPiece(int player) const {
    if (player==1)
        return player1Piece;
    return player2Piece;
}

int RPSFightInfo::getWinner() const { return playerWinner; }

void RPSFightInfo::setPosition(const RPSPoint &newPosition) { fightPosition=newPosition;}

void RPSFightInfo::setPlayer1Piece(char newPiece) {player1Piece = newPiece;}

void RPSFightInfo::setPlayer2Piece(char newPiece) {player2Piece = newPiece;}

void RPSFightInfo::setWinner(int newWinner) {playerWinner=newWinner;}



