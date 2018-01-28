#ifndef TSPGENETICRUNNER_H
#define TSPGENETICRUNNER_H

#include <vector>
#include "TSPPopulation.h"
#include "TSPCrossover.h"


class TSPGeneticRunner{
	private:
		TSP tsp;
		int population_size;  //accepts only even numbers, because just pairs mate
		uint max_iter_n;  //max number of iterations (external loop)
		double mut_prob;  //mutation probabilty
		uint sim_ann_max_iter;  //max number of iterations for Simulated Annealing
		bool choose_init;   //0 for random initialization, 1 for Sim. Ann. initial. 
	
	public:
	    /* generate a population, select and mate couples,
	     * get the best individual for every generation,
	     * then get the best individual among all generations
	     */
	    TSPGeneticRunner(TSP tsp, int population_size, uint max_iter_n,  
								   double mut_prob, uint sim_ann_max_iter,
								   bool choose_init);
		TSPSolution run();
		
};

#endif 
