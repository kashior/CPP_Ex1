cmake_minimum_required(VERSION 3.8)
project(Ex1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp RPSGame.cpp RPSParser.h RPSParser.cpp RPSGame.h RPSMainAux.cpp RPSMainAux.h Point.h PlayerAlgorithm.h PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h RPSPoint.cpp RPSPoint.h RPSPiecePosition.cpp RPSPiecePosition.h RPSBoard.cpp RPSBoard.h RPSMove.cpp RPSMove.h RPSJokerChange.cpp RPSJokerChange.h)
add_executable(Ex1 ${SOURCE_FILES})