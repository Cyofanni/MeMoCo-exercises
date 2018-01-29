#include "TSPGeneticRunner.h"

TSPGeneticRunner::TSPGeneticRunner(TSP tsp, int population_size, uint max_iter_n,  
								   double mut_prob, uint sim_ann_max_iter,
								   bool choose_init){								   
	this -> tsp = tsp;
	this -> population_size = population_size;
	this -> max_iter_n = max_iter_n;
	this -> mut_prob = mut_prob;
	this -> sim_ann_max_iter = sim_ann_max_iter;
	this -> choose_init = choose_init;	
}

TSPSolution TSPGeneticRunner::run(){
	TSPPopulation init_population(tsp, population_size);   //initial population
	if (!choose_init){   //chosen random initialization
		init_population.initPopulation_random();
	}
	else{    //chosen Simulated Annealing
		init_population.initPopulation_simAnn(sim_ann_max_iter);
	}
		
	int generation = 0;
	int num_couples_pergeneration = floor(population_size / 4);
	TSPSolution bestGeneralSolution(tsp);   //best solution overall
	std::vector<TSPSolution> bestSolutions;  //vector containing the best solution for each generation
	
	TSPPopulation curr_population(tsp, population_size);   //current population, will evolve through generations
	curr_population.copyPop(init_population);//copy initial to current
	
	/*main loop through generations*/
	while(generation < max_iter_n){	
		std::vector<TSPSolution> curr_offspring;    //total offspring of current generation
		
		/*generate an offspring of population_size/2 individuals,
		 * thus, select population_size/4 pairs for mating*/
		int couple_counter = 0;
		for (int couple_counter = 0; couple_counter < num_couples_pergeneration; couple_counter++){
			std::vector<TSPSolution> curr_couple = curr_population.selectPair();
			TSPCrossover cross_over(curr_couple[0], curr_couple[1], mut_prob);
			std::vector<TSPSolution> curr_oneoffspr = cross_over.generateOffspring();  //current selected couple's offspring
			curr_offspring.push_back(curr_oneoffspr[0]);   //add to current generation's offspring
			curr_offspring.push_back(curr_oneoffspr[1]);			
		}
		
		TSPSolution bestSolCurrGen = curr_population.replacePopulation(curr_offspring);  //replace and get best solution for current generation+offspring
		//after replacePopulation, curr_population is new -> evolution
		
		bestSolutions.push_back(bestSolCurrGen);			
		generation++;
    }
    
    //loop to get best solution
    TSPSolution bestSol = bestSolutions[0];
    for (int gen = 0; gen < bestSolutions.size(); gen++){
		if (TSPSolver::evaluate(bestSolutions[gen], tsp) < TSPSolver::evaluate(bestSol, tsp)){  //we are minimizing
			bestSol = bestSolutions[gen];
		}
	}
	
	return bestSol;
}
