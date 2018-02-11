/**
 * @file TSPPopulation.cpp
 * @brief TSP Population (neighborhood search)
 *
 */

#include "TSPPopulation.h"

double TSPPopulation::total_pop_fitness(){
	double total = 0;
	for(uint i = 0; i < population.size(); i++){
		total += TSPSolver::evaluate(population[i], tsp);
	}

	return total;
}

//think about it in the future
void TSPPopulation::find_k_worst(int k){
}

TSPPopulation::TSPPopulation(const TSP& tsp, int dimPop, const std::default_random_engine& rg,
							 const std::uniform_real_distribution<double>& distr, double mut_prob){
	this -> tsp = tsp;
	this -> dimPop = dimPop;
	this -> rg = rg;
	this -> distr = distr;
	this -> mut_prob = mut_prob;
	for (int k = 0; k < dimPop; k++){
		TSPSolution individual(tsp);
		population.push_back(individual);
	}
}

void TSPPopulation::set_dimPop(int dim){
	dimPop = dim;
}

int TSPPopulation::get_dimPop(){
	return dimPop;
}

void TSPPopulation::set_mutProb(double mp){
	mut_prob = mp;
}

void TSPPopulation::initPopulation_random(){   // generate an initial random population
	for (int i = 0; i < dimPop; i++){
		for (int j = 1; j < population[i].solutionSize(); j++) {
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
	uint temp_thresh_1 = floor(maxTrials/3);
	uint temp_thresh_2 = floor(maxTrials/2);
	uint temp_thresh_3 = floor(2*maxTrials/3);
	uint temp_thresh_4 = floor(4*maxTrials/5);

	/* after random initialization, start a simulated annealing for each individual,
	   and replace the random individual with the "annealed" one */
	for (uint i = 0; i < dimPop/10.0; i++){    //for each individual
		for (uint k = 0; k < (uint)maxTrials; k++){
			// generate a random (2-opt) neighbour
			int rand_from = rand() % (population[i].solutionSize()-3) + 1;
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
			double neighbourSolVal = TSPSolver::evaluate(neighbourSol, tsp);   //evaluate neighbour solution (neighbourSol)

			if (neighbourSolVal < currentSolVal){
				double exponentialVal = exp(-(neighbourSolVal-currentSolVal)/temperature);
				double probability = 1;
				if (exponentialVal < 1){
					probability = exponentialVal;      //assign correct value to probability
				}

				probability = probability * 100;    //easier to deal with percents
				int prob_indicator = distr(rg);
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
		}   //end of Sim. Ann. loop
	}
}

std::vector<TSPSolution> TSPPopulation::getPopulation() const{
	return population;
}

bool hasBit_one(const TSPSolution& sol){
	return sol.selectedForMating;
}

//pick up higher fitness (lower value) solution with higher probability, use Montecarlo formula
std::vector<TSPSolution> TSPPopulation::selectPairMC(int copy_size){
	double min_eval_1 = INT_MAX;
	double min_eval_2 = INT_MAX;

	TSPSolution best_parent_1 = population[0];
	TSPSolution best_parent_2 = population[0];
	int best_parent1_index = 0;
	int best_parent2_index = 0;

	std::vector<TSPSolution> return_pair;
	double total_fitness = total_pop_fitness();


	for(int i = 0; i < dimPop; i++){
			double curr_sol_fitness = TSPSolver::evaluate(population[i], tsp);   //current solution's fitness
			double prob_val = 1 - (curr_sol_fitness/total_fitness);  //Montecarlo formula
			prob_val = prob_val * 100;  //becomes handier
			double rand_val = distr(rg);
			if (curr_sol_fitness <= min_eval_1){  //we are minimizing
				if (rand_val < prob_val){
					best_parent_2 = best_parent_1;
					best_parent2_index = best_parent1_index;
					best_parent_1 = population[i];
					best_parent1_index = i;
					min_eval_1 = curr_sol_fitness;
				}
			}
			else if (curr_sol_fitness <= min_eval_2){  //we are minimizing
				if (rand_val < prob_val){
					best_parent_2 = population[i];
					best_parent2_index = i;
					min_eval_2 = curr_sol_fitness;
				}
			}
	}

	return_pair.push_back(best_parent_1);
	return_pair.push_back(best_parent_2);

	return return_pair;
}

std::vector<TSPSolution> TSPPopulation::selectPairNT(){   //n-tournament
	std::vector<TSPSolution> small_set;
	std::vector<TSPSolution> return_pair;
	TSPSolution best_parent_1 = population[0];
	TSPSolution best_parent_2 = population[1];

	//double total_fitness = total_pop_fitness();

	double prob = 20;    //add to small set with probability = prob

	int step = 2;
	int count_thresh1 = 0;
	int count_thresh2 = 0;

	for (int i = 0; i < dimPop; i+=step){
		if (count_thresh1 <= 1 && i >= dimPop/2 && i < 2*dimPop/3){
			count_thresh1++;
			if (count_thresh1 == 1){
				step = 3;
			}
		}
		if (count_thresh2 <= 1 && i >= 2*dimPop/3){
			count_thresh2++;
			if (count_thresh2 == 1){
				step = 4;
			}
		}

		double rand_val = distr(rg);
		if (population[i].selectedForMating == 0 && (rand_val < prob)){
			small_set.push_back(population[i]);
			population[i].selectedForMating = 1;
		}
		else if (population[i].selectedForMating == 1 && (rand_val < prob/2.0)){
			small_set.push_back(population[i]);
		}

		double rand_mut = distr(rg);
		if (rand_mut < mut_prob){
			TSPCrossover::do_mutation(population[i]);
		}
		/*if (i >= 90*dimPop/100){
			if (rand_mut < mut_prob*2){
				TSPCrossover::do_mutation(population[i]);
			}
		}*/
		//prob = 99*prob/100;
	}

	/*TSPSolutionComparator tsp_sol_comp;
	tsp_sol_comp.set_tsp(tsp);
	std::sort(small_set.begin(), small_set.end(), tsp_sol_comp);*/

	int parent_index = 0;
	for (uint i = 0; i < small_set.size(); i++){
		if (parent_index == 2){
			break;
		}
		double rand_val = distr(rg);
		if (rand_val < prob){
			if (parent_index == 0){
				best_parent_1 = small_set[i];
			}
			else if (parent_index == 1){
				best_parent_2 = small_set[i];
			}
			parent_index++;
			prob = (prob*(100.0-prob)) / 100.0;
		}
	}

	return_pair.push_back(best_parent_1);
	return_pair.push_back(best_parent_2);

	return return_pair;
}

/*implements Best Individuals strategy:
  generate R new individuals from N old ones; keep
  the best N among the N + R */
TSPSolution TSPPopulation::replacePopulation(std::vector<TSPSolution> offspring){
	population.insert(population.end(), offspring.begin(), offspring.end());    //append offspring to population
	TSPSolutionComparator tsp_sol_comp;
	tsp_sol_comp.set_tsp(tsp);
	std::sort(population.begin(), population.end(), tsp_sol_comp);   //sort them in descending order (ascending according to value, we are minimizing)
	TSPSolution best = population[0];
	//discard the items past the nth (dim_pop)
	/*for (int i = 5; i < dimPop; i+=2){    //leave the first 1/nth best
		double swap_prob = 1.0;       //probability of swapping
		double randv = distr(rg);
		if (randv < swap_prob){
			//include some lower fitness(higher value) solutions after dimPop, to improve diversification
			int rand_index = dimPop + 1 + rand() % (offspring.size()-2);
			population[i] = population[rand_index];
		}
	}*/
	/*if (tsp.n >= 90){
		this -> set_dimPop(ceil(94*dimPop/100));
	}*/
	//else{
		this -> set_dimPop(ceil(97*dimPop/100));
	//}
	population.resize(dimPop);
	for (int i = 0; i < dimPop; i+=1){
		population[i].selectedForMating = 0;
	}

	return best;
}

void TSPPopulation::copyPop(const TSPPopulation& pop){
	std::vector<TSPSolution> get_pop = pop.getPopulation();
	std::copy(get_pop.begin(), get_pop.end(), population.begin());
}
