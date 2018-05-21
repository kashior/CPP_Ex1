COMP = g++-5.3.0
OBJS = main.o RPSGame.o RPSParser.o RPSManager.o RPSPoint.o RPSPlayerAlgorithm.o RPSPiecePosition.o RPSMove.o RPSJokerChange.o RPSFilePlayerAlgorithm.o RPSFightInfo.o RPSBoard.o RPSAutoPlayerAlgorithm.o
EXEC = ex2
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
#use g++ -MM main.cpp to see dependencies
main.o: main.cpp RPSManager.h RPSParser.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSGame.o: RPSGame.cpp RPSGame.h RPSParser.h RPSPiecePosition.h RPSFightInfo.h RPSBoard.h RPSFilePlayerAlgorithm.h RPSAutoPlayerAlgorithm.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSManager.o: RPSManager.cpp RPSManager.h RPSGame.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSParser.o: RPSParser.cpp RPSParser.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSPoint.o: RPSPoint.cpp RPSPoint.h Point.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSPlayerAlgorithm.o: RPSPlayerAlgorithm.cpp RPSPlayerAlgorithm.h PlayerAlgorithm.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSPiecePosition.o: RPSPiecePosition.cpp RPSPiecePosition.h RPSPoint.h PiecePosition.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSMove.o: RPSMove.cpp RPSMove.h RPSJokerChange.h RPSPoint.h Move.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSJokerChange.o: RPSJokerChange.cpp RPSJokerChange.h RPSPoint.h JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSFilePlayerAlgorithm.o: RPSFilePlayerAlgorithm.cpp RPSFilePlayerAlgorithm.h RPSPlayerAlgorithm.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSFightInfo.o: RPSFightInfo.cpp RPSFightInfo.h RPSPoint.h FightInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSBoard.o: RPSBoard.cpp RPSBoard.h RPSParser.h RPSPiecePosition.h Board.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSAutoPlayerAlgorithm.o: RPSAutoPlayerAlgorithm.cpp RPSAutoPlayerAlgorithm.h RPSPlayerAlgorithm.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)