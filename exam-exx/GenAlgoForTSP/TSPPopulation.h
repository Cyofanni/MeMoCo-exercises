/**
* @file TSPPopulation.h
* @brief TSP population
*
*/
#ifndef TSPPOPULATION_H
#define TSPPOPULATION_H

//#include "TSPSolution.h"
#include "TSPSolver.h"
#include <cmath>
#include <climits>
#include <algorithm>
#include <functional>

class TSPPopulation{
	private: 
		std::vector<TSPSolution> population;
		TSP tsp;
		double total_pop_fitness();
		void find_k_worst(int k);  //finds the k worst individuals and puts them as population's first elements
		
	public:
		TSPPopulation(const TSP& tsp, int dimPop);
	
		void initPopulation_random();
		void initPopulation_simAnn(int maxTrials);    //initialize with Simulated Annealing (thus 'max trials')
		std::vector<TSPSolution> getPopulation();
		std::vector<TSPSolution> selectPair();
		void replacePopulation(std::vector<TSPSolution> offspring);  //use Best Individuals strategy
};

#endif
