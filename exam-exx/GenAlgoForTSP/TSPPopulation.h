/**
* @file TSPPopulation.h
* @brief TSP population
*
*/
#ifndef TSPPOPULATION_H
#define TSPPOPULATION_H

#include "TSPSolver.h"
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
		double total_pop_fitness();
		void find_k_worst(int k);  //finds the k worst individuals and puts them as population's first elements
		
	public:
		TSPPopulation(const TSP& tsp, int dimPop);
	
		void initPopulation_random();
		void initPopulation_simAnn(int maxTrials);    //initialize with Simulated Annealing (thus 'max trials')
		std::vector<TSPSolution> getPopulation() const;
		std::vector<TSPSolution> selectPair();
		TSPSolution replacePopulation(std::vector<TSPSolution> offspring);  //use Best Individuals strategy and return best solution
		std::list<TSPSolution> copyPop(const TSPPopulation& pop);
};

#endif
