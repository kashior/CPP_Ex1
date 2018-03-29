cmake_minimum_required(VERSION 3.8)
project(Ex1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp RPSGame.cpp RPSParser.h RPSParser.cpp RPSGame.h)
add_executable(Ex1 ${SOURCE_FILES})