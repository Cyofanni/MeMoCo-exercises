CC = g++
CPPFLAGS = -g -Wall -O2
LDFLAGS =

CPX_INCDIR  =  /opt/ibm/ILOG/CPLEX_Studio1271/cplex/include
CPX_LIBDIR  = /opt/ibm/ILOG/CPLEX_Studio1271/cplex/lib/x86-64_linux/static_pic
CPX_LDFLAGS = -lcplex -lm -pthread

OBJ = main.o knapsack.o cs1d.o

%.o: %.cpp
		$(CC) $(CPPFLAGS) -I$(CPX_INCDIR) -c $^ -o $@

main: $(OBJ)
		$(CC) $(CPPFLAGS) $(OBJ) -o main -L$(CPX_LIBDIR) $(CPX_LDFLAGS)

clean:
		rm -rf $(OBJ) main

.PHONY: clean
