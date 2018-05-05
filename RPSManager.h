

#ifndef EX1_RPSMANAGER_H
#define EX1_RPSMANAGER_H

#include "RPSGame.h"
#include "RPSAutoPlayerAlgorithm.h"
#include "RPSFilePlayerAlgorithm.h"


class RPSManager{

    unique_ptr<RPSGame> curGame;
    int player1Points;
    int player2Points;
    vector<unique_ptr<PiecePosition>> player1Positioning;
    vector<unique_ptr<PiecePosition>> player2Positioning;
//    RPSBoard board_temp;
//    vector<unique_ptr<FightInfo>> fights_temp;
//    unique_ptr<PlayerAlgorithm> player1_temp;
//    unique_ptr<PlayerAlgorithm> player2_temp;

public:

    friend class RPSGame;

    RPSManager();

    bool initCheck(int playerNum);

    void gameHandler(bool isPlayer1Auto, bool isPlayer2Auto);

    void updateWinner(bool param1, bool param2, int &winner);

    void makeOutputFile(int reason, bool param1, bool param2, int winner, int lineNum1, int lineNum2);

    void updateLoserAndBadLine(int winner, int &loser, int param1, int param2, int &badLine);

    void checkWinner(int &winner, int &reason);

    void checkAndUpdateReasonForWinner(int &reason);

    bool parseArguments(bool &isPlayer1Auto, bool &isPlayer2Auto, string args);

    bool checkIfMoveIsValid(unique_ptr<Move> &curMove, int player);

    bool checkIfMoveIsValidBoardwise(unique_ptr<Move> &curMove, int player);

//    bool checkIfJokerChangeIsValid(unique_ptr<RPSMove> &curMove, int player);

//    void checkIfMoveIsNull(unique_ptr<Move> &curMove, bool &moreMoves);

};




#endif //EX1_RPSMANAGER_H
