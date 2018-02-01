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
		for (uint j = 1; j < population[i].solutionSize(); j++) {
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
		//srand(time(NULL));      //looks like the wrong place
		/*std::cout << "FROM SOLUTION: ";
		population[i].print();
		std::cout << std::endl;
		std::cout << "WITH VALUE: " << TSPSolver::evaluate(population[i], tsp);
		std::cout << std::endl;*/
		
		for (uint k = 0; k < maxTrials; k++){
			// generate a random (2-opt) neighbour	
			int rand_from = rand() % (population[i].solutionSize()-3) + 1;   // +1 because position 0 is fixed
			int rand_to = rand_from + 1 + (rand() % (population[i].solutionSize()-2-rand_from));
			
			TSPMove move;
			move.from = rand_from;
			move.to = rand_to;
			TSPSolver tsps;     //object used to call some handy methods in TSPSolver
			//evaluate current solution (population[i])			
			double currentSolVal = TSPSolver::evaluate(population[i], tsp);
			TSPSolution oldCurrSol = population[i];    //keep the old one because swap causes dirty side-effects
			TSPSolution neighbourSol = tsps.swap(population[i], move);
			population[i] = oldCurrSol;   //keep the old one because swap causes dirty side-effects
			/*neighbourSol.print();
			std::cout << "WITH VALUE: " << TSPSolver::evaluate(neighbourSol, tsp);
			std::cout << std::endl;*/
			double neighbourSolVal = TSPSolver::evaluate(neighbourSol, tsp);   //evaluate neighbour solution (neighbourSol)
			double probability;
			
			//std::cout << " " << currentSolVal << " " << neighbourSolVal << std::endl;
			
			if (neighbourSolVal < currentSolVal){
				double exponentialVal = exp(-(neighbourSolVal-currentSolVal)/temperature); 
				double probability = 1;
				if (exponentialVal < 1){
					probability = exponentialVal;      //assign correct value to probability
				}
				
				probability = probability * 100;    //easier to deal with percents    
				//srand(time(NULL));
				int prob_indicator = rand() % 101;
						
				if (prob_indicator < probability){      //replace current solution with the computed given probability value				
					population[i] = neighbourSol;
					//std::cout << " Move to neighbour " << std::endl;
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
		}   //end of Sim. Ann. loop
		/*std::cout << "TO SOLUTION: "; 
		population[i].print();
		std::cout << std::endl;
		std::cout << "WITH VALUE: " << TSPSolver::evaluate(population[i], tsp);
		std::cout << std::endl;
		std::cout << std::endl;*/
	}
}

std::vector<TSPSolution> TSPPopulation::getPopulation() const{
	return population;
}


//pick up higher fitness solution with higher probability, use Montecarlo formula
std::vector<TSPSolution> TSPPopulation::selectPair(){    
	double min_eval_1 = INT_MAX;
	double min_eval_2 = INT_MAX;
	
	TSPSolution best_parent_1 = population[0];
	TSPSolution best_parent_2 = population[0];	
	int best_parent1_index = 0;
	int best_parent2_index = 0;
	
	std::vector<TSPSolution> return_pair;
	double total_fitness = total_pop_fitness();
	
	//srand(time(NULL));
	for(int i = 0; i < population.size(); i++){  
		//if it hadn't been selected before
		if (population[i].selectedForMating == 0){
			double curr_sol_fitness = TSPSolver::evaluate(population[i], tsp);   //current solution's fitness		
			double prob_val = curr_sol_fitness / total_fitness;  //Montecarlo formula
			prob_val = prob_val * 100;  //becomes handier
			if (prob_val * 2 * 100 < 100){   //try to increase that little probability
				prob_val = prob_val * 2 * 100;
			}
			int rand_val = rand() % 101;
			if (curr_sol_fitness <= min_eval_1){  //we are minimizing		
			//	if (rand_val < prob_val){
					//std::cout << "ENTERED PROBABLE REGION" << std::endl;
					best_parent_2 = best_parent_1;
					best_parent2_index = best_parent1_index;
					best_parent_1 = population[i];	
					best_parent1_index = i;	
					min_eval_1 = curr_sol_fitness;	
			//	}	
			}
			else if (curr_sol_fitness <= min_eval_2){  //we are minimizing
			//	if (rand_val < prob_val){
				//	std::cout << "ENTERED PROBABLE REGION" << std::endl;
					best_parent_2 = population[i];
					best_parent2_index = i;
					min_eval_2 = curr_sol_fitness;	
			//	}		
			}
		}		
	}

	//if (!(best_parent_1 == best_parent_2)){    //return them only if they are different
		/*lock the 2 solutions, can't be selected anymore*/ 
		//since montecarlo doesn't work, apply random exchange with some probability value
		int rand_index_1 = 0;   //random index 
		while (population[rand_index_1].selectedForMating == 1){
			rand_index_1 = rand() % (dimPop-1);
		}
		int rand_index_2 = rand_index_1;
		while (rand_index_2 == rand_index_1 || population[rand_index_2].selectedForMating == 1){
			rand_index_2 = rand() % (dimPop-2);
		}

		int swap_prob = 80;       //probability of swapping
		int randv = rand() % 101;
		if (randv < swap_prob){
		     best_parent_1 = population[rand_index_1]; 
	    	 best_parent1_index = rand_index_1;
	    	 best_parent_2 = population[rand_index_2];      
		     best_parent2_index = rand_index_2;
		}				

		population[best_parent1_index].selectedForMating = 1;
		population[best_parent2_index].selectedForMating = 1;
	
		best_parent_1.selectedForMating = 1;
		best_parent_2.selectedForMating = 1;	
		//std::cout << best_parent1_index << " " << best_parent2_index << std::endl;

		return_pair.push_back(best_parent_1);
		return_pair.push_back(best_parent_2);
	
		return return_pair;
	//}
	/*else{   //otherwise, retry by recursion
		return selectPair();
	}*/
}

/*implements Best Individuals strategy:
  generate R new individuals from N old ones; keep
  the best N among the N + R */
TSPSolution TSPPopulation::replacePopulation(std::vector<TSPSolution> offspring){
	population.insert(population.end(), offspring.begin(), offspring.end());    //append offspring to population
	TSPSolutionComparator tsp_sol_comp;
	tsp_sol_comp.set_tsp(tsp);
	std::sort(population.begin(), population.end(), tsp_sol_comp);   //sort them in descending order (ascending according to value, we are minimizing)
	//discard the items past the nth (dim_pop)
	for (int i = 1; i < dimPop; i++){    //leave the first 
		int swap_prob = 40;       //probability of swapping
		int randv = rand() % 101; 
		if (randv < swap_prob){
			//include some lower fitness solutions after dimPop, to improve diversification
			int rand_index = dimPop + 1 + rand() % (offspring.size()-2);
			population[i] = population[rand_index];
		}
	}
	
	population.resize(dimPop);	
	
	for (int i = 0; i < dimPop; i++){
		population[i].selectedForMating = 0;
	}
	return population[0];
}

void TSPPopulation::copyPop(const TSPPopulation& pop){
	std::vector<TSPSolution> get_pop = pop.getPopulation();
	std::copy(get_pop.begin(), get_pop.end(), population.begin());
}
