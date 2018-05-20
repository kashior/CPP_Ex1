#ifndef RPSPARSER_H
#define RPSPARSER_H

#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include "RPSMove.h"
#include "RPSPiecePosition.h"
#include "RPSPoint.h"

using namespace std;

#define M 10
#define N 10
#define R 2
#define P 5
#define S 1
#define J 2
#define B 2
#define F 1

/**
 * A class of static functions that meant to parse a command from a user file.
 */
class RPSParser {

public:


    /**
     *
     * RPSParserParseLineInit parses a line from an initialization file of one of the players.
     * The functions checks if the line has 3 or 4 tokens (separated with space), if not returns 1=error: to many or to few
     * tokens in the line, if yes- sends the array of tokens to other function and returns its return value.
     *
     * @param line - the line to parse
     * @param initPos
     * @param Y
     * @param tool
     * @param isJoker
     * @return 0 if the line is valid
     *         1/2/3 otherwise.
     *
     */
    static int parseLineInit(const string &line, unique_ptr<RPSPiecePosition> &initPos);


    /**
     *
     * Parses a line from some moves file to an array of tokens separated with space. If the size of the array is
     * 4 or 8, the function will send the tokens to other function to check validation, otherwise the line is invalid.
     *
     * @param line - the line to parse
     * @param newMove - if the line is valid newMove will be updated.
     * @return 0 - if line is valid
     *         1/2/3 - otherwise.
     *
     */
    static int
    parseLineMove(const string &line, unique_ptr<RPSMove> &newMove, unique_ptr<RPSJokerChange> &newJokerChange);

/**
 * This function decides if the arguments given in command line are OK or corrupted
 * @param isPlayer1Auto - update this boolean to be FilePlayerAlgo or AutoPlayerAlgo of player 1
 * @param isPlayer2Auto - pdate this boolean to be FilePlayerAlgo or AutoPlayerAlgo of player 2
 * @param args - the line to parse
 * @return true if the arguments are good, and updates by reference the 2 booleans
 */
    static bool parseArguments(bool &isPlayer1Auto, bool &isPlayer2Auto, string args);

private:
    /**
     *
     * Checks if the tokes array of size 3 represents and initiation line of the format "<PIECE_CHAR> <X> <Y>". If yes -
     * it updates the parameters given to the function by reference.
     * @param initPos - x coordinate on board.
     * @param Y - y coordinate on board.
     * @param tool - the tool to place om board.
     * @return 0 - if all tokens are valid
     *         2 - if the tool is invalid.
     *         3 - if the position on board is invalid.
     *
     */
    static int parse3TokensInitLine(vector<string> tokens, unique_ptr<RPSPiecePosition> &initPos);

    /**
     *
     * Checks if the tokes array of size 4 represents an initiation line of the format "J <X> <Y> <PIECE_CHAR>". If yes -
     * it updates the parameters given to the function by reference.
     * @param initPos - x coordinate on board.
     * @param Y - y coordinate on board.
     * @param tool - the tool to place om board.
     * @return 0 - if all tokens are valid
     *         2 - if the tool is invalid.
     *         3 - if the position on board is invalid.
     *
     */
    static int parse4TokensInitLine(vector<string> tokens, unique_ptr<RPSPiecePosition> &initPos);


    /**
     *
     * Checks if a move line with 4 tokens is of the format: "<FROM_X> <FROM_Y> <TO_X> <TO_Y>".If it is, updates newMove
     * accordingly.
     * @param newMove
     * @param tokens
     * @return 0 - on success.
     *         3 - on fail (the coordinates are invalid).
     *
     */
    static int parse4TokensMoveLine(unique_ptr<RPSMove> &newMove, vector<string> tokens);

    /**
     *
     * Checks if a move line with 8 tokens is of the format: "<FROM_X> <FROM_Y> <TO_X> <TO_Y> J: <JOKER_X>
     *  <JOKER_Y> <NEW_REP>".If it is, updates newMove
     * accordingly.
     * @param newMove
     * @param tokens
     * @return 0 - on success.
     *         1 - if the fifth tokes is not "J"
     *         2 - if <NEW_REP> is not a valid piece
     *         3 - if the position on board is invalid
     */
    static int parse8TokensMoveLine(unique_ptr<RPSMove> &newMove, vector<string> tokens,
                                    unique_ptr<RPSJokerChange> &newJokerChange);

    /**
     *
     * Checks if the X and Y coordinates are valid coordinates on the game board.
     *
     * @param X - x coordinate.
     * @param Y - y coordinate.
     * @return true - if coordinates are valid.
     *         false - otherwise.
     */
    static bool checkIfPositionValid(string X, string Y);




};

#endif //RPSPARSER_H
