/**
* @file TSPPopulation.h
* @brief TSP population
*
*/

#include "TSPSolution.h"
#include "TSPSolver.h"
#include <cmath>
#include <climits>

class TSPPopulation{
	private: 
		std::vector<TSPSolution> population;
		TSP tsp;
		
	public:
		TSPPopulation(const TSP& tsp, int dimPop);
	
		void initPopulation_random();
		void initPopulation_simAnn(int maxTrials);    //initialize with Simulated Annealing (thus 'max trials')
		std::vector<TSPSolution> getPopulation();
		std::vector<TSPSolution> selectPair();
		void replacePopulation();
};
