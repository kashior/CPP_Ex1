//
// Created by Or Kashi on 22/04/2018.
//

#include "RPSMove.h"

RPSMove::RPSMove( RPSPoint from,  RPSPoint to, char piece, int player):  _piece(piece), _player(player) {
    _from.setX(from.getX());
    _from.setY(from.getY());
    _to.setX(to.getX());
    _to.setY(to.getY());
}

RPSMove::RPSMove(){
    _piece='\0';
    _from.setX(-1);
    _from.setY(-1);
    _to.setX(-1);
    _to.setY(-1);
    _player=0;

}

const Point &RPSMove::getFrom() const {return _from;}

void RPSMove::setFrom(int x, int y) {_from.setX(x);_from.setY(y);}

void RPSMove::setTo(int x, int y) {_to.setX(x);_to.setY(y);}

const Point &RPSMove::getTo() const {return _to;}

const int &RPSMove::getPlayer() const {return _player;}

const char &RPSMove::getPiece() const {return _piece;}


void RPSMove::setPiece(char piece) { _piece=piece; }


