

#include "RPSBoard.h"



RPSBoard::RPSBoard() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            board[i][j] = ' ';
        }
    }

}
int RPSBoard::getPlayer(const Point &pos) const {
    if(board[pos.getY()][pos.getX()]==' ')
        return 0;
    if(islower(board[pos.getX()][pos.getY()]))
        return 2;
    return 1;
}


char RPSBoard::getPiece(int X, int Y){
    return board[Y][X];
}