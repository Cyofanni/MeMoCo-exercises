#ifndef TSPCROSSOVER_H
#define TSPCROSSOVER_H

#include <map>
#include <stdio.h>
#include <stdlib.h>
#include "TSPSolution.h"
#include <algorithm>

class TSPCrossover{
	private:
	  TSPSolution parent_1;
	  TSPSolution parent_2;
	  double mutation_probability;
	  
	  void do_mutation(TSPSolution& individual);
	
	public:
	  TSPCrossover(const TSPSolution& p1, const TSPSolution& p2, double mp);
	  std::vector<TSPSolution> generateOffspring(); //implements Partially-mapped crossover	  
};

#endif
