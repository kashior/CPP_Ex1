

#include "RPSAutoPlayerAlgorithm.h"


using namespace std;

RPSPlayerAlgorithm::RPSPlayerAlgorithm(int player) : _player(player) {
    playerToolCounters['R'] = R;
    playerToolCounters['P'] = P;
    playerToolCounters['S'] = S;
    playerToolCounters['B'] = B;
    playerToolCounters['J'] = J;
    playerToolCounters['F'] = F;
}

RPSPlayerAlgorithm::getPlayer(){return _player;}

void RPSPlayerAlgorithm::addJokerPositionToVector(unique_ptr<PiecePosition> &pos) {
    playerJokers.push_back(make_unique<RPSPoint>(pos->getPosition()));
}
