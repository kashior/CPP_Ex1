COMP = g++-5.3.0
OBJS = main.o RPSGame.o RPSParser.o RPSMainAux.o
EXEC = ex1
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
#use g++ -MM main.cpp to see dependencies
main.o: main.cpp RPSMainAux.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSGame.o: RPSGame.cpp RPSGame.h RPSParser.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSParser.o: RPSParser.cpp RPSParser.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSMainAux.o: RPSMainAux.cpp RPSMainAux.h RPSGame.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)