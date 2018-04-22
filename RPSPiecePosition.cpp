//
// Created by Or Kashi on 22/04/2018.
//

#include "RPSPiecePosition.h"

RPSPiecePosition::RPSPiecePosition(Point pos,char piece,char jokerRep='#'):_pos(pos.getX(),pos.getY()),_piece(piece), _jokerRep(jokerRep) {}

const Point &RPSPiecePosition::getPosition() const {return _pos;}

char RPSPiecePosition::getPiece() const {return _piece;}

char RPSPiecePosition::getJokerRep() const {return _jokerRep;}

void setPosition(RPSPoint* pos) const {_pos = *pos;}

void setPiece(char piece) const {_piece = piece;}

void setJokerRep(char jokerRep) const {_jokerRep = jokerRep;}



bool RPSPiecePosition::operator<(const RPSPiecePosition &rhs) const 
    {
        if (_pos.getX() < rhs.getPosition().getX())
            return true;
        if (_pos.getX() > rhs.getPosition().getX())
            return false;
        return _pos.getY()  < rhs.getPosition().getY();
    }

bool RPSPiecePosition::operator==(const RPSPiecePosition &rhs) const 
    {
        return _pos.getY() == rhs.getPosition().getY() && _pos.getX() == rhs.getPosition().getX();
    }



