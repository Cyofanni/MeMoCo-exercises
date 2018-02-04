#ifndef TSPGENETICRUNNER_H
#define TSPGENETICRUNNER_H

#include <vector>
#include "TSPPopulation.h"
#include "TSPCrossover.h"


class TSPGeneticRunner{
	private:
		TSP tsp;
		int population_size;
		uint max_iter_n;    //max number of iterations (external loop)
		double mut_prob;  	//mutation probabilty
		bool choose_init;   //0 for random initialization, 1 for Sim. Ann. initial. 
		uint sim_ann_max_iter;  //max number of iterations for Simulated Annealing
		std::default_random_engine rg;
		std::uniform_real_distribution<double> distr;
	
	public:
	    /* generate a population, select and mate couples,
	     * get the best individual for every generation,
	     * then get the best individual among all generations
	     */
	    TSPGeneticRunner(TSP tsp, int population_size, uint max_iter_n,  
								   double mut_prob, bool choose_init, 
								   uint sim_ann_max_iter, const std::default_random_engine&,  
								   const std::uniform_real_distribution<double>&);
		TSPSolution run();
		
};

#endif 
