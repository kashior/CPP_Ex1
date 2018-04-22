//
// Created by Or Kashi on 22/04/2018.
//

#include "RPSMove.h"

RPSMove::RPSMove(Point from, Point to, char piece, int player): _from(from),_to(to),_piece(piece),_player(player) {}

const Point &RPSMove::getFrom() const {return _from;}

void RPSMove::setFrom(int x, int y) {_from.setX(x);_from.setY(y);}

void RPSMove::setTo(int x, int y) {_to.setX(x);_to.setY(y);}

const Point &RPSMove::getTo() const {return _to;}

const int &RPSMove::getPlayer() const {return _player;}

const char &RPSMove::getPiece() const {return _piece;}

void RPSMove::setPiece(char piece) { _piece=piece; }

void RPSMove::setJoker(char rep, RPSPoint* pos) {
    _joker.setJokerChangePosition(*pos);
    _joker.setJokerNewRep(rep);
}
