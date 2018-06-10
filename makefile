COMP = g++-5.3.0
OBJS = main.o RPSManager.o RPSPoint.o RSPPlayer_203022041.o RPSPiecePosition.o RPSMove.o RPSJokerChange.o \
        RPSFightInfo.o RPSBoard.o RPSTourManager.o AlgorithmRegistration.o
EXEC = ex3
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -ldl -pthread


$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -ldl -pthread -o  $@

#use g++ -MM main.cpp to see dependencies
main.o: main.cpp RPSTourManager.h PlayerAlgorithm.h Point.h \
          PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h RPSManager.h \
          RSPPlayer_203022041.h gameDefinitions.h RPSBoard.h RPSMove.h \
          RPSPoint.h RPSJokerChange.h RPSFightInfo.h RPSPiecePosition.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp


RPSManager.o: RPSManager.cpp RPSManager.h RSPPlayer_203022041.h \
  gameDefinitions.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h \
  FightInfo.h Move.h JokerChange.h RPSBoard.h RPSMove.h RPSPoint.h \
  RPSJokerChange.h RPSFightInfo.h RPSPiecePosition.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp


RPSPoint.o: RPSPoint.cpp RPSPoint.h Point.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

RPSTourManager.o: RPSTourManager.cpp RPSTourManager.h PlayerAlgorithm.h \
  Point.h PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h \
  RPSManager.h RSPPlayer_203022041.h gameDefinitions.h RPSBoard.h \
  RPSMove.h RPSPoint.h RPSJokerChange.h RPSFightInfo.h \
  RPSPiecePosition.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

RPSPiecePosition.o: RPSPiecePosition.cpp RPSPiecePosition.h PiecePosition.h RPSPoint.h Point.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSMove.o: RPSMove.cpp RPSMove.h Move.h RPSPoint.h Point.h gameDefinitions.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

RPSJokerChange.o: RPSJokerChange.cpp RPSJokerChange.h JokerChange.h RPSPoint.h Point.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

RPSFightInfo.o: RPSFightInfo.cpp RPSFightInfo.h FightInfo.h RPSPoint.h Point.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

RPSBoard.o: RPSBoard.cpp RPSBoard.h Point.h Board.h gameDefinitions.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

RSPPlayer_203022041.o: RSPPlayer_203022041.cpp RSPPlayer_203022041.h \
  gameDefinitions.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h \
  FightInfo.h Move.h JokerChange.h RPSBoard.h RPSMove.h RPSPoint.h \
  RPSJokerChange.h RPSFightInfo.h RPSPiecePosition.h AlgorithmRegistration.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

AlgorithmRegistration.o: AlgorithmRegistration.cpp \
  AlgorithmRegistration.h PlayerAlgorithm.h Point.h PiecePosition.h \
  Board.h FightInfo.h Move.h JokerChange.h RPSTourManager.h RPSManager.h \
  RSPPlayer_203022041.h gameDefinitions.h RPSBoard.h RPSMove.h \
  RPSPoint.h RPSJokerChange.h RPSFightInfo.h RPSPiecePosition.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

clean:
	rm -f $(OBJS) $(EXEC)