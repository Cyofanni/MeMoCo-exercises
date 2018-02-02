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
	
	std::vector<TSPSolution> get_pop = init_population.getPopulation();
	std::cout << "INITIAL POPULATION: " << std::endl;	
	for (int i = 0; i < get_pop.size(); i++){
		std::cout << i << ") ";
		get_pop[i].print();
		std::cout << "WITH VALUE: " << TSPSolver::evaluate(get_pop[i], tsp);	
		std::cout << std::endl;
		//std::cout << "SELECTED BIT: " << get_pop[i].selectedForMating;
		//std::cout << std::endl;				
	}
	
		
	int generation = 0;
	int num_couples_pergeneration = ceil(population_size / 4.0);
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
		TSPPopulation copy_for_select(tsp, population_size);  //I can remove solutions from a copy
		copy_for_select.copyPop(curr_population);
		for (int couple_counter = 0; couple_counter < num_couples_pergeneration; couple_counter++){
			std::vector<TSPSolution> curr_couple = copy_for_select.selectPair(); //select and remove from the copy
			TSPCrossover cross_over(curr_couple[0], curr_couple[1], mut_prob);
			std::vector<TSPSolution> curr_oneoffspr = cross_over.generateOffspringTrials();  //current selected couple's offspring
			curr_offspring.push_back(curr_oneoffspr[0]);   //add to current generation's offspring
			curr_offspring.push_back(curr_oneoffspr[1]);			
		}
		
		TSPSolution bestSolCurrGen = curr_population.replacePopulation(curr_offspring);  //replace and get best solution for current generation+offspring
		//after replacePopulation, curr_population is new -> evolution
		
		get_pop = curr_population.getPopulation();
		std::cout << "GENERATION " << generation << "'s" << " CURRENT POPULATION AFTER CROSSOVER AND REPLACEMENT: " << std::endl;	
		for (int i = 0; i < get_pop.size(); i++){
			std::cout << i << ") ";
			get_pop[i].print();
			std::cout << "WITH VALUE: " << TSPSolver::evaluate(get_pop[i], tsp);	
			std::cout << std::endl;
			//std::cout << "SELECTED BIT: " << get_pop[i].selectedForMating;
			//std::cout << std::endl;				
		}
		std::cout << std::endl;	
		std::cout << std::endl;	
		
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
