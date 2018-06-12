COMP = g++-5.3.0
OBJS = main.o RPSManager.o RPSPoint.o RPSPiecePosition.o RPSMove.o RPSJokerChange.o \
        RPSFightInfo.o RPSBoard.o RPSTourManager.o AlgorithmRegistration.o
EXEC = ex3
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -mcmodel=large -ldl -pthread


$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -ldl -pthread -fPIC -o  $@

#use g++ -MM main.cpp to see dependencies
main.o: main.cpp RPSTourManager.h RPSManager.h PlayerAlgorithm.h Point.h \
  PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h \
  gameDefinitions.h RPSBoard.h RPSMove.h RPSPoint.h RPSFightInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp


RPSManager.o: RPSManager.cpp RPSManager.h PlayerAlgorithm.h Point.h \
  PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h \
  gameDefinitions.h RPSBoard.h RPSMove.h RPSPoint.h RPSFightInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp


RPSPoint.o: RPSPoint.cpp RPSPoint.h Point.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

RPSTourManager.o: RPSTourManager.cpp RPSTourManager.h RPSManager.h \
  PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
  JokerChange.h gameDefinitions.h RPSBoard.h RPSMove.h RPSPoint.h \
  RPSFightInfo.h
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
  AlgorithmRegistration.h PlayerAlgorithm.h Point.h PiecePosition.h \
  Board.h FightInfo.h Move.h JokerChange.h gameDefinitions.h RPSBoard.h \
  RPSMove.h RPSPoint.h RPSJokerChange.h RPSFightInfo.h \
  RPSPiecePosition.h
	$(COMP) $(CPP_COMP_FLAG) -fPIC  -c $*.cpp

RSPPlayer_203022042.o: RSPPlayer_203022042.cpp RSPPlayer_203022042.h \
  AlgorithmRegistration.h PlayerAlgorithm.h Point.h PiecePosition.h \
  Board.h FightInfo.h Move.h JokerChange.h gameDefinitions.h RPSBoard.h \
  RPSMove.h RPSPoint.h RPSJokerChange.h RPSFightInfo.h \
  RPSPiecePosition.h
	$(COMP) $(CPP_COMP_FLAG) -fPIC  -c $*.cpp

AlgorithmRegistration.o: AlgorithmRegistration.cpp \
  AlgorithmRegistration.h PlayerAlgorithm.h Point.h PiecePosition.h \
  Board.h FightInfo.h Move.h JokerChange.h RPSTourManager.h RPSManager.h \
  gameDefinitions.h RPSBoard.h RPSMove.h RPSPoint.h RPSFightInfo.h \
  RPSJokerChange.h RPSPiecePosition.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

clean:
	rm -f $(OBJS) $(EXEC) RSPPlayer_203022041.o

rps_lib: RSPPlayer_203022041.o $(OBJS)
	$(COMP) -shared -Wl,-soname,RSPPlayer_203022041.so -o RSPPlayer_203022041.so $(OBJS) RSPPlayer_203022041.o

rps_lib2: RSPPlayer_203022042.o $(OBJS)
	$(COMP) -shared -Wl,-soname,RSPPlayer_203022042.so -o RSPPlayer_203022042.so $(OBJS) RSPPlayer_203022042.o

all: $(OBJS) ex3

rps_tournament:
	make all
