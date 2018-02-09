/**
* @file TSPPopulation.h
* @brief TSP population
*
*/
#ifndef TSPPOPULATION_H
#define TSPPOPULATION_H

#include "TSPSolver.h"
#include "TSPCrossover.h"
#include <cmath>
#include <climits>
#include <algorithm>
#include <functional>
#include <random>
#include <list>

class TSPPopulation{
	private: 
		std::vector<TSPSolution> population;
		int dimPop;
		TSP tsp;
		std::default_random_engine rg;
		std::uniform_real_distribution<double> distr;
		double mut_prob;
		double total_pop_fitness();
		void find_k_worst(int k);  //finds the k worst individuals and puts them as population's first elements
		
	public:
		TSPPopulation(const TSP& tsp, int dimPop, const std::default_random_engine& rg,  
							 const std::uniform_real_distribution<double>& distr, double mut_prob);
							 
		void set_dimPop(int dim);		
		int get_dimPop();
		void set_mutProb(double mp);				
					 
		void initPopulation_random();
		void initPopulation_simAnn(int maxTrials);    //initialize with Simulated Annealing (thus 'max trials')
		std::vector<TSPSolution> getPopulation() const;
		std::vector<TSPSolution> selectPairMC(int copy_size);   //montecarlo
		std::vector<TSPSolution> selectPairNT();	 //n-tournament
		TSPSolution replacePopulation(std::vector<TSPSolution> offspring);  //use Best Individuals strategy and return best solution
		void copyPop(const TSPPopulation& pop);
};

#endif
