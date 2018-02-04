CC = g++
CPPFLAGS = -g -std=c++0x -Wall -O2
LDFLAGS =

OBJ = TSPSolver.o TSPPopulation.o TSPCrossover.o TSPGeneticRunner.o main.o

%.o: %.cpp
		$(CC) $(CPPFLAGS) -c $^ -o $@

main: $(OBJ)
		$(CC) $(CPPFLAGS) $(OBJ) -o main 

clean:
		rm -rf $(OBJ) main

.PHONY: clean
