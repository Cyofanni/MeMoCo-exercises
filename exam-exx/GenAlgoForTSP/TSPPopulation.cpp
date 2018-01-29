/**
 * @file TSPPopulation.cpp
 * @brief TSP Population (neighborhood search)
 *
 */

#include "TSPPopulation.h"

double TSPPopulation::total_pop_fitness(){
	double total = 0;
	for(int i = 0; i < population.size(); i++){
		total += TSPSolver::evaluate(population[i], tsp);
	}
	
	return total;
}

//think about it in the future
void TSPPopulation::find_k_worst(int k){
}

TSPPopulation::TSPPopulation(const TSP& tsp, int dimPop){
	this -> tsp = tsp;
	this -> dimPop = dimPop;
	for (int k = 0; k < dimPop; k++){
		TSPSolution individual(tsp);
		population.push_back(individual);
	}
}

void TSPPopulation::initPopulation_random(){   // generate an initial random population
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
		srand(time(NULL));
		for (uint k = 0; k < maxTrials; k++){
			// generate a random (2-opt) neighbour	
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
				
				probability = probability * 100;    //easier to deal with percents    
				srand(time(NULL));
				int prob_indicator = rand() % 101;
				
				if (prob_indicator < probability){      //replace current solution with the computed given probability value
					population[i] = neighbourSol;
				}				
			}
						
			// temperature's drop
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

std::vector<TSPSolution> TSPPopulation::getPopulation() const{
	return population;
}


//pick up higher fitness solution with higher probability, use Montecarlo formula
std::vector<TSPSolution> TSPPopulation::selectPair(){    
	TSPSolution best_parent_1 = population[0];
	TSPSolution best_parent_2 = population[0];
	int best_parent1_index = 0;
	int best_parent2_index = 0;
	
	std::vector<TSPSolution> return_pair;
	double total_fitness = total_pop_fitness();
	
	srand(time(NULL));
	for(int i = 0; i < population.size(); i++){  
		//if it hadn't been selected before
		if (population[i].selectedForMating == 0){
			if (TSPSolver::evaluate(population[i], tsp) <= TSPSolver::evaluate(best_parent_1, tsp)){  //we are minimizing
				double curr_sol_fitness = TSPSolver::evaluate(population[i], tsp);   //current solution's fitness
				double prob_val = curr_sol_fitness / total_fitness;  //Montecarlo formula
				prob_val = prob_val * 100;  //becomes handier
				int rand_val = rand() % 101;
			
				if (rand_val < prob_val){
					best_parent_2 = best_parent_1;
					best_parent2_index = i;
					best_parent_1 = population[i];
					best_parent1_index = i;
				}	
			}
			else if (TSPSolver::evaluate(population[i], tsp) <= TSPSolver::evaluate(best_parent_2, tsp)){  //we are minimizing
				double curr_sol_fitness = TSPSolver::evaluate(population[i], tsp);   //current solution's fitness
				double prob_val = curr_sol_fitness / total_fitness;  //Montecarlo formula
				prob_val = prob_val * 100;  //becomes handier with percents
				int rand_val = rand() % 101;
			
				if (rand_val < prob_val){
					best_parent_2 = population[i];
					best_parent2_index = i;
				}
			}
		}
			
	}
	
	/*lock the 2 solutions, can't be selected anymore*/
	population[best_parent1_index].selectedForMating = 1;
	population[best_parent2_index].selectedForMating = 1;	
	
	return_pair.push_back(best_parent_1);
	return_pair.push_back(best_parent_2);
	
	return return_pair;
}

/*implements Best Individuals strategy:
  generate R new individuals from N old ones; keep
  the best N among the N + R */
TSPSolution TSPPopulation::replacePopulation(std::vector<TSPSolution> offspring){
	int dim_pop = population.size();  
	population.insert(population.end(), offspring.begin(), offspring.end());    //append offspring to population
	TSPSolutionComparator tsp_sol_comp;
	tsp_sol_comp.set_tsp(tsp);
	std::sort(population.begin(), population.end(), tsp_sol_comp);   //sort them in descending order (ascending according to value, we are minimizing)
	//discard the items past the nth (dim_pop)
	population.resize(dim_pop);
	return population[0];
}

void TSPPopulation::copyPop(const TSPPopulation& pop){
	std::vector<TSPSolution> get_pop = pop.getPopulation();
	std::copy(get_pop.begin(), get_pop.end(), population.begin());
}
