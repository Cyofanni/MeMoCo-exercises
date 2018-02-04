#ifndef TSPCROSSOVER_H
#define TSPCROSSOVER_H

#include <map>
#include <stdio.h>
#include <stdlib.h>
#include "TSPSolution.h"
#include <algorithm>
#include <random>

class TSPCrossover{
	private:
	  TSPSolution parent_1;
	  TSPSolution parent_2;
	  double mutation_probability;
	  std::default_random_engine rg;  
	  std::uniform_real_distribution<double> distr;
	  
	  bool is_feasible(TSPSolution sol);
	
	public:
	  TSPCrossover(const TSPSolution& p1, const TSPSolution& p2, double mp, const std::default_random_engine&,  
						const std::uniform_real_distribution<double>&);
	  
	  static void do_mutation(TSPSolution& individual){
		  int sol_size = individual.solutionSize();
		  int fstnode_index = (rand() % ((sol_size - 3)/2)) + 1;
		  int sndnode_index = fstnode_index + 1 + (rand() % (sol_size-2-fstnode_index));
		  TSPSolution tmpSol(individual);
		  for (int i = fstnode_index; i <= sndnode_index; i++) {
			individual.sequence[i] = tmpSol.sequence[sndnode_index-(i-fstnode_index)];
		  } 
	  }
	  			
	  std::vector<TSPSolution> generateOffspring(bool& feasible_flag);
	  std::vector<TSPSolution> generateOffspringTrials(); //implements Partially-Mapped Crossover	  
};

#endif
