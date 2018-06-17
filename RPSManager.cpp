
#include "RPSManager.h"


RPSManager::RPSManager(function<unique_ptr<PlayerAlgorithm>()> _player1,
                       function<unique_ptr<PlayerAlgorithm>()> _player2) {
    player1 = _player1();
    player2 = _player2();
    movesCounter = 0;

}


RPSMove RPSManager::setMoveToBoard(unique_ptr<Move> curMove, int player, RPSFightInfo &curFight) {

    char fromPiece = board.board[curMove->getFrom().getY()][curMove->getFrom().getX()];
    char toPiece = board.board[curMove->getTo().getY()][curMove->getTo().getX()];
    RPSPoint fromPoint(curMove->getFrom().getX(), curMove->getFrom().getY());
    RPSPoint toPoint(curMove->getTo().getX(), curMove->getTo().getY());
    unique_ptr<RPSMove> resultMove =
            make_unique<RPSMove>(fromPoint, toPoint, fromPiece, player);
    RPSMove moveRes(fromPoint, toPoint, fromPiece, player);
    if (toPiece != ' ') { //fight!
        curFight.setIsFight(true);
        vector<unique_ptr<FightInfo>> fights;
        curFight.setPlayer1Piece(player==1? fromPiece : toPiece);
        curFight.setPlayer2Piece(player==1? toPiece : fromPiece);
        curFight.setPosition(toPoint);
        doFight(fights, move(resultMove));
        curFight.setWinner(fights.at(0)->getWinner());
    } else {
        board.board[toPoint.getY()][toPoint.getX()] = fromPiece;
        board.board[fromPoint.getY()][fromPoint.getX()] = ' ';
    }
    return moveRes;
}


void RPSManager::doFight(vector<unique_ptr<FightInfo>> &fights, unique_ptr<RPSMove> fightMove) {
    char attackerTool = fightMove->getPiece();
    char defenderTool = board.board[fightMove->getTo().getY()][fightMove->getTo().getX()];
    if ((attackerTool == defenderTool) || (attackerTool == 'B') || (defenderTool == 'B') ||
        (attackerTool == 'b') || (defenderTool == 'b') || (char) toupper(defenderTool) == attackerTool ||
        (char) toupper(attackerTool) == defenderTool)
        removeBothPiecesFromGame(fightMove, fights);
    else if (attackerTool == 'f' || attackerTool == 'F')//possible only in init stage
        fightAttackerLoses(fightMove, fights);
    else if (defenderTool == 'F' || defenderTool == 'f')
        fightAttackerWins(fightMove, fights);
    else
        fightInner(fightMove, fights);
}


void RPSManager::fightInner(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights) {
    int x = curMove->getTo().getX();
    int y = curMove->getTo().getY();
    char piece = curMove->getPiece();
    if ((board.board[y][x] == 'R' || board.board[y][x] == 'r') && (piece == 'P' || piece == 'p'))
        fightAttackerWins(curMove, fights);

    else if ((board.board[y][x] == 'P' || board.board[y][x] == 'p') && (piece == 'R' || piece == 'r'))
        fightAttackerLoses(curMove, fights);

    else if ((board.board[y][x] == 'R' || board.board[y][x] == 'r') && (piece == 'S' || piece == 's'))
        fightAttackerLoses(curMove, fights);

    else if ((board.board[y][x] == 'S' || board.board[y][x] == 's') && (piece == 'R' || piece == 'r'))
        fightAttackerWins(curMove, fights);

    else if ((board.board[y][x] == 'P' || board.board[y][x] == 'p') && (piece == 'S' || piece == 's'))
        fightAttackerWins(curMove, fights);

    else if ((board.board[y][x] == 'S' || board.board[y][x] == 's') && (piece == 'P' || piece == 'p'))
        fightAttackerLoses(curMove, fights);
}

void RPSManager::removeBothPiecesFromGame(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights) {

    updateFightVectors(0, curMove, fights);
    board.board[curMove->getTo().getY()][curMove->getTo().getX()] = ' ';
    if (curMove->getFrom().getX() != -1) //not in init stage, need to change 'from' position on board to ' '
        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] = ' ';

}

void RPSManager::fightAttackerWins(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights) {
    char attackerPiece = curMove->getPiece();
    updateFightVectors(curMove->getPlayer(), curMove, fights);
    board.board[curMove->getTo().getY()][curMove->getTo().getX()] = attackerPiece;
    if (curMove->getFrom().getX() != -1) //not init stage
        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] = ' ';
}

void RPSManager::fightAttackerLoses(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights) {
    updateFightVectors(curMove->getPlayer() == 1 ? 2 : 1, curMove, fights);
    if (curMove->getFrom().getX() != -1) //not init stage
        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] = ' ';
}

void RPSManager::updateFightVectors(int winner, unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights) {

    unique_ptr<FightInfo> fight;

    auto attackedPiece = (char) toupper(board.board[curMove->getTo().getY()][curMove->getTo().getX()]);
    auto attackerPiece = (char) toupper(curMove->getPiece());
    RPSPoint fightPoint(curMove->getTo().getX(), curMove->getTo().getY());

    if (curMove->getPlayer() == 1)
        fight = make_unique<RPSFightInfo>(winner, attackerPiece, attackedPiece, fightPoint, true);
    else
        fight = make_unique<RPSFightInfo>(winner, attackedPiece, attackerPiece, fightPoint, true);

    fights.push_back(move(fight));
}


void RPSManager::updateInitialBoard(vector<unique_ptr<FightInfo>> &fights) {
    int x, y;
    char piece;

    //update player 1 tools to the board
    for (auto &pos: player1Positioning) {
        board.board[pos->getPosition().getY()][pos->getPosition().getX()] = pos->getJokerRep() == '#' ?
                                                                            pos->getPiece() : pos->getJokerRep();
    }


    // update player 2 tools to board , fight if necessary
    for (auto &pos: player2Positioning) {
        x = pos->getPosition().getX();
        y = pos->getPosition().getY();
        piece = pos->getJokerRep() == '#' ?
                (char) tolower(pos->getPiece()) : (char) (tolower(pos->getJokerRep()));
        if (board.board[pos->getPosition().getY()][pos->getPosition().getX()] != ' ')//FIGHT!
        {
            RPSPoint nullPoint;
            RPSPoint fightPoint(x, y);
            unique_ptr<RPSMove> fightMove = make_unique<RPSMove>(nullPoint, fightPoint, piece, 2);
            doFight(fights, move(fightMove));
        } else
            board.board[pos->getPosition().getY()][pos->getPosition().getX()] = piece;
    }
}


int RPSManager::gameHandler() {

    int reason; // the "reason" for output file
    // 1=all flags of the opponent are captured
    // 2=All moving PIECEs of the opponent are eaten
    // 6=100 moves done without a fight
    int winner; // 0=tie, 1=player1 wins, 2=player2 wins, 3= continue the game
    vector<unique_ptr<FightInfo>> fights;
    player1->getInitialPositions(1, player1Positioning);
    player2->getInitialPositions(2, player2Positioning);
    updateInitialBoard(fights);

    // lets notify on initial positions to the players algorithms
    player1->notifyOnInitialBoard(board, fights);
    player2->notifyOnInitialBoard(board, fights);

    if ((winner = checkWinner(reason)) != 3)
        return winner;

    unique_ptr<Move> curMovePtr;
    unique_ptr<JokerChange> curJokerChangePtr;
    RPSMove curMove;
    RPSFightInfo curFight;

    while ((winner == 3) && (movesCounter < 100)) {

        // player 1's turn

        curMovePtr = player1->getMove(); // get the move from player algorithm

        if (!checkIfMoveIsValid(curMovePtr, 1)) {
            winner = 2;
            reason = 5;
            break;
        }


        curMove = setMoveToBoard(move(curMovePtr), 1, curFight);// apply move to board after it has been checked
        if (curFight.getIsFight()) {// if there was a fight during this move
            movesCounter = 0;
            player1->notifyFightResult(curFight);// in case there was a fight, notify player algorithm
            player2->notifyFightResult(curFight);
        } else {
            movesCounter++;
            if (movesCounter >= 100) {
                winner = 0;
                cout << "100 moves done without a fight! It's a tie\n" << endl;
                reason = 6;
                break;
            }
        }
        winner = checkWinner(reason);
        if (winner != 3)
            break;
        player2->notifyOnOpponentMove(curMove);
        curJokerChangePtr = player1->getJokerChange(); // in case there was joker change, get it
        if (curJokerChangePtr != nullptr) {
            if (checkIfJokerChangeIsValid(curJokerChangePtr, 1)) {
                board.board[curJokerChangePtr->getJokerChangePosition().getY()]
                [curJokerChangePtr->getJokerChangePosition().getX()] = curJokerChangePtr->getJokerNewRep();
            } else {
                winner = 2;
                reason = 5;
                break;
            }
        }

//player 2's turn

        curMovePtr = player2->getMove(); // get the move from player algorithm

        if (!checkIfMoveIsValid(curMovePtr, 2)) {
            winner = 1;
            reason = 5;
            break;
        }

        curMove = setMoveToBoard(move(curMovePtr), 2, curFight);
        if (curFight.getIsFight()) {// if there was a fight during this move
            movesCounter = 0;
            player2->notifyFightResult(curFight);// in case there was a fight, notify player algorithm
            player1->notifyFightResult(curFight);
        } else {
            movesCounter++;
            if (movesCounter >= 100) {
                winner = 0;
                cout << "100 moves done without a fight! It's a tie\n" << endl;
                reason = 6;
                break;
            }
        }

        winner = checkWinner(reason);// first check for winner
        if (winner != 3)
            break;


        player1->notifyOnOpponentMove(curMove);

        curJokerChangePtr = player2->getJokerChange(); // in case there was joker change, get it
        if (curJokerChangePtr != nullptr) {
            if (checkIfJokerChangeIsValid(curJokerChangePtr, 2)) {
                char jokerPiece = (char) tolower(curJokerChangePtr->getJokerNewRep());
                board.board[curJokerChangePtr->getJokerChangePosition().getY()]
                [curJokerChangePtr->getJokerChangePosition().getX()] = jokerPiece;
            } else {
                winner = 1;
                reason = 5;
                break;
            }

        }
    }


    return winner;
}

int RPSManager::checkWinner(int &reason) {
    int flags1 = 0, flags2 = 0;
    int moving1 = 0, moving2 = 0;
    char piece;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            piece = board.getPiece(j, i);
            if (piece == 'R' || piece == 'P' || piece == 'S')
                moving1++;
            else if (piece == 'r' || piece == 'p' || piece == 's')
                moving2++;
            else if (piece == 'F')
                flags1++;
            else if (piece == 'f')
                flags2++;
        }
    }
    if ((flags1 == 0 || moving1 == 0) && (flags2 == 0 || moving2 == 0)) {
        if (flags1 == 0 || flags2 == 0)
            reason = 1;
        else
            reason = 2;
        return 0;
    }
    if (flags1 == 0 || moving1 == 0) {
        if (flags1 == 0)
            reason = 1;
        else
            reason = 2;
        return 2;
    }
    if (flags2 == 0 || moving2 == 0) {
        if (flags2 == 0)
            reason = 1;
        else
            reason = 2;
        return 1;
    }
    return 3;
}


bool RPSManager::checkIfMoveIsValid(unique_ptr<Move> &curMove, int player) {
    if (board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] == ' ')
        return false;
    if (player == 1) {
        if (islower(board.board[curMove->getFrom().getY()][curMove->getFrom().getX()]))
            return false;
        if (isupper(board.board[curMove->getTo().getY()][curMove->getTo().getX()]))
            return false;
        if (board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] == 'F'
            || board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] == 'B')
            return false; //can't move un-moving pieces
    }
    if (player == 2) {
        if (isupper(board.board[curMove->getFrom().getY()][curMove->getFrom().getX()]))
            return false;
        if (islower(board.board[curMove->getTo().getY()][curMove->getTo().getX()]))
            return false;
        if (board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] == 'f'
            || board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] == 'b')
            return false; //can't move un-moving pieces
    }

    return true; //all good
}


bool RPSManager::checkIfJokerChangeIsValid(unique_ptr<JokerChange> &curChange, int player) {
    RPSPoint jokerPoint(curChange->getJokerChangePosition().getX(), curChange->getJokerChangePosition().getY());
    if (board.board[jokerPoint.getY()][jokerPoint.getX()] == ' ')
        return false;
    if (player == 1) {
        if (islower(board.board[jokerPoint.getY()][jokerPoint.getX()]))
            return false;
    } else {//player 2 have small letters
        if (isupper(board.board[jokerPoint.getY()][jokerPoint.getX()]))
            return false;
    }
    return true;
}

