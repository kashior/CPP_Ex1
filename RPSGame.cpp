#include "RPSGame.h"

using namespace std;

RPSGame::RPSGame() : winner(0), player1Error(""), player2Error("") {
    player1ToolCounters["R"]=R;
    player1ToolCounters["P"]=P;
    player1ToolCounters["S"]=S;
    player1ToolCounters["B"]=B;
    player1ToolCounters["J"]=J;
    player1ToolCounters["F"]=F;
    player2ToolCounters["r"]=R;
    player2ToolCounters["p"]=P;
    player2ToolCounters["s"]=S;
    player2ToolCounters["b"]=B;
    player2ToolCounters["j"]=J;
    player2ToolCounters["f"]=F;
}

int RPSGame::RPSGamePlayer1Init(string filename) {
    ifstream fin(filename);
    if (fin.fail()) {
        cout << "Error: There's no such file or directory, ???";//TODO : update the sentence
        return -1;
    }
    string lineToParse;
    int X = 0, Y = 0;
    int lineNum=1;
    int parserResult ;
    string tool;
    while (true) {
        getline(fin, lineToParse);
        if (fin == NULL) { break; } //TODO: check where to put it
        parserResult=RPSParserParseLineInit(lineToParse,X,Y,tool);
        switch(parserResult){
            case 1:
                cout << "Error: Invalid number of arguments in line " << lineNum << " of player 1's file" <<endl;
                return lineNum;
            case 2:
                cout << "Error: Invalid piece argument in line " << lineNum << " of player 1's file" << endl;
                return lineNum;
            case 3:
                cout << "Error: Invalid position on board in line "<< lineNum << " of player 1's file" << endl;
                return lineNum;
            default:
                if (!RPSGameUpdateBoardInitStage(X,Y,tool,lineNum)) {
                    return lineNum;
                }
                lineNum++;
        }

    }
    if (player1ToolCounters["F"] != 0){
        cout << "Error: Missing Flags - Flags are not positioned according to their number in line " <<
                                        lineNum-1 << " of player 1's file" << endl;
        return lineNum-1;
    }


    return 0;
}

bool RPSGame::RPSGameUpdateBoardInitStage(int X, int Y, string tool, int lineNum){
    if (!board[X][Y].empty()){
        cout << "Error: Two or more pieces are positioned on the same location in line " << lineNum <<
             " of player 1's file" << endl;
        return false;
    }
    if (player1ToolCounters[tool]==0){
        cout << "Error: A piece type appears in file more than it's number in line " << lineNum <<
             " of player 1's file" << endl;
        return false;
    }
    player1ToolCounters[tool]--;
    board[X][Y] = tool;
    return true;
}



