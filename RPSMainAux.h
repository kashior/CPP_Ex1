//
//#include "RPSGame.h"
//
//
//#ifndef EX1_RPSMAINAUX_H
//#define EX1_RPSMAINAUX_H
//
///**
// * In case there is a winner (or two winners) the function checks if the reason for the winning is
// * because all opponents flags are eaten or because all moving tool are eaten, and updates the reason.
// *
// * @param game - the current game
// * @param reason - will be 1 if all flags of the opponent are captured
// *                 or 2 if all moving pieces of the opponent are eaten
// *
// */
//void RPSMainAuxCheckAndUpdateReasonForWinner(RPSGame &game, int &reason);
///**
// * checks if at this point of the game there is winner, and updates the reason and winner parameters
// * @param winner 0, 1 or 2
// * @param reason 1,2,3,4 or 5
// * @param game - the current game
// */
//void RPSMainAuxFinalCheckOfGameBoard(int &winner, int &reason, RPSGame &game);
///**
// * Updates the winner according to the value of param1 and param2. This is done after
// * reading the positioning input files.
// *
// * @param param1
// * @param param2
// * @param winner
// */
//void RPSMainAuxUpdateWinner(RPSGame &game, int param2, int &winner, int param1);
//
///**
// * This function is called after reading the moves input files, and when one of the lines in one of
// * the files is invalid. Then it updates the loser (according to the winner), and the "bad" line (number of invalid line).
// *
// * @param winner
// * @param loser
// * @param param1
// * @param param2
// * @param badLine
// */
//void RPSMainAuxUpdateLoserAndBadLine(int winner, int &loser, int param1, int param2, int &badLine);
///**
// * Writes the output file "rps.output"
// *
// * @param reason
// * @param param1
// * @param param2
// * @param winner
// * @param game
// */
//void RPSMainAuxMakeOutputFile(int reason, int result1, int result2, int winner, RPSGame &game);
///**
// * The handler of the game
// *
// */
//void RPSMainAuxGameHandler(RPSGame &newGame);
//
//
//#endif //EX1_RPSMAINAUX_H
