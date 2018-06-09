cmake_minimum_required(VERSION 3.8)
project(Ex1)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp Point.h PlayerAlgorithm.h PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h RPSPoint.cpp RPSPoint.h RPSPiecePosition.cpp RPSPiecePosition.h RPSBoard.cpp RPSBoard.h RPSMove.cpp RPSMove.h RPSJokerChange.cpp RPSJokerChange.h RPSFightInfo.cpp RPSFightInfo.h  RSPPlayer_203022041.cpp RSPPlayer_203022041.h RPSManager.h RPSManager.cpp AlgorithmRegistration.h gameDefinitions.h)
add_executable(Ex1 ${SOURCE_FILES})