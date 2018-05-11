
#include "RPSGame.h"
//#include "RPSFilePlayerAlgorithm.h"
//#include "RPSAutoPlayerAlgorithm.h"
#include "RPSBoard.h"
#include "RPSFightInfo.h"


RPSGame::RPSGame(bool &isPlayer1Auto, bool &isPlayer2Auto) {
    movesCounter = 0;
    if (!isPlayer1Auto)
        player1 = make_unique<RPSFilePlayerAlgorithm>(1, "/");
    else
        player1 = make_unique<RPSAutoPlayerAlgorithm>(1);

    if (!isPlayer2Auto)
        player2 = make_unique<RPSFilePlayerAlgorithm>(2, "/");
    else
        player2 = make_unique<RPSAutoPlayerAlgorithm>(2);
}



bool RPSGame::UpdateBoardPlayer1InitStage(int &lineNum, vector<unique_ptr<PiecePosition>> & playersPositioning,
                                          unique_ptr<RPSPlayerAlgorithm> &playerAlg) {
    int x,y;
    for (unique_ptr<PiecePosition> &pos : playersPositioning){
        char piece=pos->getPiece();
        x = pos->getPosition().getX();
        y = pos->getPosition().getY();
        if (board.board[y][x] != ' ') {
            cout << "Error: Two or more pieces are positioned on the same location in line " << lineNum <<
             " of player 1's positioning" << endl;
            return false;
        }
        if (playerAlg->playerToolCounters[piece] == 0) {
            cout << "Error: A piece type appears more than it's number in line " << lineNum <<
                 " of player 1's positioning" << endl;
            return false;
        }
        if (pos->getJokerRep() !=  '#') {
            playerAlg->addJokerPositionToVector(pos);
            board.board[y][x] = pos->getJokerRep();
        }
        else
            board.board[y][x] = pos->getPiece();
        playerAlg->playerToolCounters[pos->getPiece()]--;
        lineNum++;
    }
    return true;
}


bool RPSGame::UpdateBoardPlayer2InitStage(int &lineNum, vector<unique_ptr<PiecePosition>> &playersPositioning,
                                          unique_ptr<RPSPlayerAlgorithm> &player1Alg,
                                          unique_ptr<RPSPlayerAlgorithm> &player2Alg,
                                          vector<unique_ptr<FightInfo>> &fights) {
    int x,y;
    char piece;
    for (unique_ptr<PiecePosition>& pos : playersPositioning){
        x = pos->getPosition().getX();
        y = pos->getPosition().getY();
        piece = (char)tolower(pos->getPiece());
        if ( (int) board.board[y][x] > 96) { //the position already contains player 2 tool
            cout << "Error: Two or more pieces are positioned on the same location in line " << lineNum <<
                 " of player 2's positioning" << endl;
            return false;
        }
        if (player2Alg->playerToolCounters[pos->getPiece()] == 0) {
            cout << "Error: A piece type appears more than it's number in line " << lineNum <<
                 " of player 2's positioning" << endl;
            return false;
        }
        if (pos->getJokerRep() !=  '#')
            player2Alg->addJokerPositionToVector(pos);

        player2Alg->playerToolCounters[pos->getPiece()]--;
        if (board.board[y][x] == ' ')
            board.board[y][x] = pos->getJokerRep()=='#' ? piece : (char) tolower(pos->getJokerRep());
        else {
            RPSPoint nullPoint(-1,-1);
            RPSPoint fightPoint(pos->getPosition().getX(),pos->getPosition().getY());
            unique_ptr<RPSMove> initMove=make_unique<RPSMove>(nullPoint, fightPoint, piece, 2);
            if (pos->getJokerRep() !=  '#')
                initMove->setPiece(pos->getJokerRep());
            fightOuter(initMove, fights, player1Alg, player2Alg);
        }
        lineNum++;
    }
    return true;
}


void RPSGame::fightOuter(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                         unique_ptr<RPSPlayerAlgorithm> &player1Alg, unique_ptr<RPSPlayerAlgorithm> &player2Alg) {
    //test//
    if(curMove->getTo().getX()==8 &&curMove->getTo().getY()==8)
        cout << "hey" << endl;
    //end of test//

    char attackerTool = curMove->getPlayer() == 1 ? curMove->getPiece(): (char)tolower(curMove->getPiece());
    char defenderTool = board.board[curMove->getTo().getY()][curMove->getTo().getX()];
    if ((attackerTool == defenderTool) || (attackerTool == 'B') || (defenderTool == 'B') ||
            (attackerTool == 'b') || (defenderTool == 'b') || (char)toupper(defenderTool)==attackerTool ||
            (char)toupper(attackerTool)==defenderTool)
        removeBothPiecesFromGame(curMove, fights, player1Alg, player2Alg);
    else if (curMove->getPiece() == 'f' ) //possible only in init stage
        fightAttackerLoses(curMove, fights, player2Alg, player2Alg);
    else if (defenderTool == 'F')
        fightAttackerWins(curMove, fights, player1Alg, player2Alg);
    else if (defenderTool == 'f')
        fightAttackerWins(curMove, fights, player1Alg, player2Alg);
    else
        fightInner(curMove, fights, player1Alg, player2Alg);
}

// remove both playertoolscounter and joker locations if there is , update board
void RPSGame::removeBothPiecesFromGame(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                                       unique_ptr<RPSPlayerAlgorithm> &player1,
                                       unique_ptr<RPSPlayerAlgorithm> &player2) {

    auto attackedPiece = (char) toupper(board.board[curMove->getTo().getY()][curMove->getTo().getX()]);
    auto attackerPiece = (char) toupper(curMove->getPiece());

    if (curMove->getPlayer() == 1) {
        removeToolsFromVectors(player1, curMove, attackerPiece);
        removeToolsFromVectors(player2, curMove, attackedPiece);
    } else {
        removeToolsFromVectors(player2, curMove, attackerPiece);
        removeToolsFromVectors(player1, curMove, attackedPiece);
    }

    updateFightVectors(0, curMove, fights);

    board.board[curMove->getTo().getY()][curMove->getTo().getX()] = ' ';
    if (curMove->getFrom().getX() != -1) //not in init stage, need to change 'from' position on board to ' '
        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] = ' ';
}




void RPSGame::fightInner(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                         unique_ptr<RPSPlayerAlgorithm> &player1,
                         unique_ptr<RPSPlayerAlgorithm> &player2) {
    if ((board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'R' ||
            board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'r') &&
        (curMove->getPiece() == 'P' || curMove->getPiece() == 'p'))
        fightAttackerWins(curMove, fights, player1, player2);

    else if ((board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'P' ||
            board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'p') &&
               (curMove->getPiece() == 'R' || curMove->getPiece() == 'r'))
        fightAttackerLoses(curMove, fights, player1, player2);

    else if ((board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'R' ||
            board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'r') &&
               (curMove->getPiece() == 'S' || curMove->getPiece() == 's'))
        fightAttackerLoses(curMove, fights, player1, player2);

    else if ((board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'S'
              || board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 's') &&
               (curMove->getPiece() == 'R' || curMove->getPiece() == 'r'))
        fightAttackerWins(curMove, fights, player1, player2);

    else if ((board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'P' ||
            board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'p') &&
               (curMove->getPiece() == 'S' || curMove->getPiece() == 's'))
        fightAttackerWins(curMove, fights, player1, player2);

    else if ((board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 'S' ||
            board.board[curMove->getTo().getY()][curMove->getTo().getX()] == 's') &&
               (curMove->getPiece() == 'P' || curMove->getPiece() == 'p'))
        fightAttackerLoses(curMove, fights, player1, player2);
}


void RPSGame::fightAttackerWins(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                                unique_ptr<RPSPlayerAlgorithm> &player1, unique_ptr<RPSPlayerAlgorithm> &player2) {
    auto attackedPiece= (char) toupper(board.board[curMove->getTo().getY()][curMove->getTo().getX()]);
    if (curMove->getPlayer() == 1)
        removeToolsFromVectors(player2, curMove, attackedPiece);
    else
        removeToolsFromVectors(player1, curMove, attackedPiece);
    updateFightVectors(curMove->getPlayer(), curMove, fights);
    board.board[curMove->getTo().getY()][curMove->getTo().getX()] = curMove->getPiece();
    if (curMove->getFrom().getX() != -1) //not init stage
        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] = ' ';
}


void RPSGame::fightAttackerLoses(unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights,
                                 unique_ptr<RPSPlayerAlgorithm> &player1, unique_ptr<RPSPlayerAlgorithm> &player2) {
    auto attackedPiece= (char) toupper(curMove->getPiece());
    if (curMove->getPlayer() == 1)
        removeToolsFromVectors(player1, curMove, attackedPiece);
    else
        removeToolsFromVectors(player2, curMove, attackedPiece);
    updateFightVectors(curMove->getPlayer() == 1 ? 2 : 1, curMove, fights);
    if (curMove->getFrom().getX() != -1) //not init stage
        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] = ' ';
}


void RPSGame::removeToolsFromVectors(unique_ptr<RPSPlayerAlgorithm> &player, unique_ptr<RPSMove> &curMove,
                                     char pieceToRemove) {
        auto it = find_if( player->playerJokers.begin(), player->playerJokers.end(),
                          [&](unique_ptr<RPSPoint> &obj) {
                              return obj->getX() == curMove->getTo().getX()
                                     && obj->getY() == curMove->getTo().getY();
                          });
        if (it !=  player->playerJokers.end()) {
            player->playerToolCounters['J']++;
            player->playerJokers.erase(it);
        } else {
            player->playerToolCounters[pieceToRemove]++;
        }

}

void RPSGame::updateFightVectors(int winner, unique_ptr<RPSMove> &curMove, vector<unique_ptr<FightInfo>> &fights) {

    unique_ptr<FightInfo> fight;

    auto attackedPiece= (char) toupper(board.board[curMove->getTo().getY()][curMove->getTo().getX()]);
    auto attackerPiece = (char) toupper(curMove->getPiece());
    RPSPoint fightPoint(curMove->getTo().getX(),curMove->getTo().getY());

    if (curMove->getPlayer() == 1)
        fight = make_unique<RPSFightInfo>(winner, attackerPiece, attackedPiece, fightPoint);
    else
        fight = make_unique<RPSFightInfo>(winner, attackedPiece, attackerPiece, fightPoint);

    fights.push_back(move(fight));
}




bool RPSGame::CheckIfPlayerLose(unique_ptr<RPSPlayerAlgorithm> &player) {
    if (player->playerToolCounters['F'] == F)
        return true;
    return player->playerToolCounters['R'] == R && player->playerToolCounters['P'] == P
           && player->playerToolCounters['S'] == S && player->playerToolCounters['J'] == J;
}



RPSMove RPSGame::setMoveToBoard(unique_ptr<Move> curMove, int player, RPSFightInfo &curFight) {

    char fromPiece = board.board[curMove->getFrom().getY()][curMove->getFrom().getX()];
    char toPiece = board.board[curMove->getTo().getY()][curMove->getTo().getX()];
    RPSPoint fromPoint(curMove->getFrom().getX(),curMove->getFrom().getY());
    RPSPoint toPoint(curMove->getTo().getX(),curMove->getTo().getY());
    unique_ptr<RPSMove> resultMove=make_unique<RPSMove>(fromPoint, toPoint, board.board[fromPoint.getY()][fromPoint.getX()],player);

    if(toPiece!=' ') { //fight!
        vector<unique_ptr<FightInfo>> fights;
        curFight.setPlayer1Piece(isupper(fromPiece) == 0 ? toPiece : fromPiece);
        curFight.setPlayer2Piece(isupper(fromPiece) == 0 ? fromPiece : toPiece);
        curFight.setPosition(RPSPoint(curMove->getTo().getX(),curMove->getTo().getY()));
        fightOuter(resultMove, fights, player1, player2);
        curFight.setWinner(fights.at(0)->getWinner());
        if (player == 1 && fights.at(0)->getWinner() == 1) //attacker wins maybe it was a joker
            changeJokerPosition(player1, curMove);
        else if (player == 2 && fights.at(0)->getWinner() == 2)
            changeJokerPosition(player2, curMove);
    }

    else {
        board.board[curMove->getTo().getY()][curMove->getTo().getX()] = fromPiece;
        board.board[curMove->getFrom().getY()][curMove->getFrom().getX()] = ' ';
        if (player == 1)
            changeJokerPosition(player1, curMove);
        else
            changeJokerPosition(player2, curMove);
    }

    RPSMove moveRes(fromPoint,toPoint,resultMove->getPiece(),resultMove->getPlayer());
    return moveRes;
}



void RPSGame::changeJokerPosition(unique_ptr<RPSPlayerAlgorithm> &playerAlg, unique_ptr<Move> &curMove) {
    for ( auto&& point : playerAlg->playerJokers){
        if (point->getX() == curMove->getFrom().getX() && point->getY() == curMove->getFrom().getY()){
        // if the moved piece is a joker!
            point->setX(curMove->getTo().getX());
            point->setY(curMove->getTo().getY());
            // updated the new position of the joker
            break;
        }
    }
}


void RPSGame::printBoardToScreen() {
    cout << "" << endl;
    cout << "turn number " << movesCounter << " ,current board:" << endl;

    for (int j = 0; j < N; j++) {
        for (int i = 0; i < M; i++) {
            cout << board.getPiece(i, j);
        }
        cout << endl;
    }
}


