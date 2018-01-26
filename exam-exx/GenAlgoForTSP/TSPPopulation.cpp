/**
 * @file TSPPopulation.cpp
 * @brief TSP Population (neighborhood search)
 *
 */

#include "TSPPopulation.h"

TSPPopulation::TSPPopulation(const TSP& tsp, int dimPop){
	this -> tsp = tsp;
	for (int k = 0; k < dimPop; k++){
		TSPSolution individual(tsp);
		population.push_back(individual);
	}
}

void TSPPopulation::initPopulation_random(){   // generate an inital random population
	for (int i = 0; i < population.size(); i++){
		srand(time(NULL));
		for (uint i = 1; i < population[i].solutionSize(); ++i) {
			// initial and final position are fixed (initial/final node remains 0)
			// random swaps 
			int idx1 = rand() % (population[i].solutionSize()-2) + 1;
			int idx2 = rand() % (population[i].solutionSize()-2) + 1;
			int tmp = population[i].sequence[idx1];
			population[i].sequence[idx1] = population[i].sequence[idx2];
			population[i].sequence[idx2] = tmp;
		}
	}
}

void TSPPopulation::initPopulation_simAnn(int maxTrials){
	this -> initPopulation_random();
	double temperature = INT_MAX;
	// impose some thresholds for temperature function (depends on iteration index k)
	int temp_thresh_1 = floor(maxTrials/3);    
	int temp_thresh_2 = floor(maxTrials/2);
	int temp_thresh_3 = floor(2*maxTrials/3);
	int temp_thresh_4 = floor(4*maxTrials/5);
	
	/* after random initialization, start a simulated annealing for each individual, 
	   and replace the random individual with the "annealed" one */
	for (uint i = 0; i < population.size(); i++){    //for each individual
		// run Simulated Annealing
		for (uint k = 0; k < maxTrials; k++){
			// generate a random (2-opt) neighbour
			srand(time(NULL));
			int rand_from = rand() % (population[i].solutionSize()-2) + 1;   // +1 because position 0 is fixed
			int rand_to = rand() % (population[i].solutionSize()-2) + (rand_from+1);
			TSPMove move;
			move.from = rand_from;
			move.to = rand_to;
			TSPSolver tsps;     //object used to call some handy methods in TSPSolver
			TSPSolution neighbourSol = tsps.swap(population[i], move);
			double currentSolVal = TSPSolver::evaluate(population[i], tsp);   //evaluate current solution (population[i])
			double neighbourSolVal = TSPSolver::evaluate(neighbourSol, tsp);   //evaluate neighbour solution (neighbourSol)
			double probability;
			
			if (neighbourSolVal < currentSolVal){
				double exponentialVal = exp(-(neighbourSolVal-currentSolVal)/temperature); 
				double probability = 1;
				if (exponentialVal < 1){
					probability = exponentialVal;      //assign correct value to probability
				}
				
				srand(time(NULL));
				int prob_indicator = rand() % static_cast<int>(probability);
				
				if (prob_indicator < probability){      //replace current solution with the computed given probability value
					population[i] = neighbourSol;
				}
				
			}
			
			
			// temperature drop
			if (k == temp_thresh_1){
				temperature = temperature / 2;
			}
			else if (k == temp_thresh_2){
				temperature = temperature / 3;
			}
			else if (k == temp_thresh_3){
				temperature = temperature / 4;
			}
			else if (k == temp_thresh_4){
				temperature = temperature / 5;
			}		
		}	
	}
}


std::vector<TSPSolution> TSPPopulation::getPopulation(){
	return population;
}
