

#include "RPSFightInfo.h"

RPSFightInfo::RPSFightInfo(int winner, char piece, Point position) : playerWinner(winner), opponentPiece(piece),
                                                                     fightPosition(position) {}

const Point &RPSFightInfo::getPosition() const { return fightPosition; }

char RPSFightInfo::getOpponentPiece() const { return opponentPiece; }

int RPSFightInfo::getWinner() const { return playerWinner; }

void RPSFightInfo::setPosition(Point newPosition) {fightPosition=newPosition;}

void RPSFightInfo::setOpponentPiece(char newPiece) {opponentPiece=newPiece;}

void RPSFightInfo::setWinner(int newWinner) {playerWinner=newWinner;}


