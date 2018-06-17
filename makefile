COMP = g++-5.3.0
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG

EXEC = ex3
SO	 = RSPPlayer_203022041.so


OBJS = main.o RPSManager.o RPSPoint.o RPSPiecePosition.o RPSMove.o RPSJokerChange.o \
        RPSFightInfo.o RPSBoard.o RPSTourManager.o AlgorithmRegistration.o
SO_OBJS = RSPPlayer_203022041.o RPSBoard.o RPSMove.o RPSPoint.o RPSJokerChange.o RPSFightInfo.o RPSPiecePosition.o


$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -ldl -pthread -rdynamic -o  $@
$(SO): $(SO_OBJS)
	$(COMP) $(SO_OBJS) -shared -Wl,-soname,RSPPlayer_203022041.so -o $@


	
main.o: main.cpp RPSTourManager.h RPSManager.h PlayerAlgorithm.h Point.h \
  PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h \
  gameDefinitions.h RPSBoard.h RPSMove.h RPSPoint.h RPSFightInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSManager.o: RPSManager.cpp RPSManager.h PlayerAlgorithm.h Point.h \
  PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h \
  gameDefinitions.h RPSBoard.h RPSMove.h RPSPoint.h RPSFightInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSPoint.o: RPSPoint.cpp RPSPoint.h Point.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
RPSTourManager.o: RPSTourManager.cpp RPSTourManager.h RPSManager.h \
  PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
  JokerChange.h gameDefinitions.h RPSBoard.h RPSMove.h RPSPoint.h \
  RPSFightInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSPiecePosition.o: RPSPiecePosition.cpp RPSPiecePosition.h PiecePosition.h RPSPoint.h Point.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
RPSMove.o: RPSMove.cpp RPSMove.h Move.h RPSPoint.h Point.h gameDefinitions.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
RPSJokerChange.o: RPSJokerChange.cpp RPSJokerChange.h JokerChange.h RPSPoint.h Point.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
RPSFightInfo.o: RPSFightInfo.cpp RPSFightInfo.h FightInfo.h RPSPoint.h Point.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
RPSBoard.o: RPSBoard.cpp RPSBoard.h Point.h Board.h gameDefinitions.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
RSPPlayer_203022041.o: RSPPlayer_203022041.cpp RSPPlayer_203022041.h \
  AlgorithmRegistration.h PlayerAlgorithm.h Point.h PiecePosition.h \
  Board.h FightInfo.h Move.h JokerChange.h gameDefinitions.h RPSBoard.h \
  RPSMove.h RPSPoint.h RPSJokerChange.h RPSFightInfo.h \
  RPSPiecePosition.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp

AlgorithmRegistration.o: AlgorithmRegistration.cpp \
  AlgorithmRegistration.h PlayerAlgorithm.h Point.h PiecePosition.h \
  Board.h FightInfo.h Move.h JokerChange.h RPSTourManager.h RPSManager.h \
  gameDefinitions.h RPSBoard.h RPSMove.h RPSPoint.h RPSFightInfo.h \
  RPSJokerChange.h RPSPiecePosition.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

.PHONY: rps_tournament
rps_tournament: $(EXEC)
.PHONY: rps_lib
rps_lib: $(SO)
.PHONY: all
all: $(EXEC) $(SO)

clean:
	rm -f $(OBJS) $(EXEC) $(SO) $(SO_OBJS)