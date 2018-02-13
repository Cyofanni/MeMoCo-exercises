#include "TSPGeneticRunner.h"

TSPGeneticRunner::TSPGeneticRunner(TSP tsp, int population_size, uint max_iter_n,
								   double mut_prob, bool choose_init,
								   uint sim_ann_max_iter, const std::default_random_engine& rg,
								   const std::uniform_real_distribution<double>& distr){
	this -> tsp = tsp;
	this -> population_size = population_size;
	this -> max_iter_n = max_iter_n;
	this -> mut_prob = mut_prob;
	this -> choose_init = choose_init;
	this -> sim_ann_max_iter = sim_ann_max_iter;
	this -> rg = rg;
	this -> distr = distr;
}

TSPSolution TSPGeneticRunner::run(){
	TSPPopulation init_population(tsp, population_size, rg, distr, mut_prob);   //initial population
	if (!choose_init){   //chosen random initialization
		init_population.initPopulation_random();
	}
	else{    //chosen Simulated Annealing
		init_population.initPopulation_simAnn(sim_ann_max_iter);
	}

	//std::vector<TSPSolution> get_pop = init_population.getPopulation();

	uint generation = 0;
	int num_couples_pergeneration = floor(population_size / 4.0);
	TSPSolution bestGeneralSolution(tsp);   //best solution overall
	std::vector<TSPSolution> bestSolutions;  //vector containing the best solution for each generation

	TSPPopulation curr_population(tsp, population_size, rg, distr, mut_prob);   //current population, will evolve through generations
	curr_population.copyPop(init_population);	 //copy initial to current


	//int no_improv_counter = 0;
	TSPSolution bestSolCurrGen(tsp);
	//double old_bestSolValue;


	/*main loop through generations*/
	while(generation < max_iter_n){
		if (curr_population.get_dimPop() < 2){
			break;
		}
		/*if (no_improv_counter >= ceil(max_iter_n/10.0) && (generation >= (max_iter_n/3))){
			break;
		}*/

		if (generation > 95*max_iter_n/100){
			curr_population.set_mutProb(mut_prob*3);
		}

		//TSPSolution old_bestSolCurrGen;
		//old_bestSolCurrGen = bestSolCurrGen;

		std::vector<TSPSolution> curr_offspring;    //total offspring of current generation

		/*generate an offspring of population_2*size/3 individuals,
		 * thus, select population_size/3 pairs for mating*/
		for (int couple_counter = 0; couple_counter < num_couples_pergeneration; couple_counter++){
			std::vector<TSPSolution> curr_couple = curr_population.selectPairNT(); //select from current population
			TSPCrossover cross_over(curr_couple[0], curr_couple[1], mut_prob, rg, distr);
			std::vector<TSPSolution> curr_oneoffspr = cross_over.generateOffspring_OC();  //current selected couple's offspring
			curr_offspring.push_back(curr_oneoffspr[0]);   //add to current generation's offspring
			curr_offspring.push_back(curr_oneoffspr[1]);
		}


		bestSolCurrGen = curr_population.replacePopulation(curr_offspring);  //replace and get best solution for current generation+offspring

		/*if (old_bestSolValue == TSPSolver::evaluate(bestSolCurrGen, tsp)){
			no_improv_counter++;
		}
		else{
			no_improv_counter = 0;
		}

		old_bestSolValue = TSPSolver::evaluate(bestSolCurrGen, tsp);*/
		//std::cout << old_bestSolValue << std::endl;
		//after replacePopulation, curr_population is new -> evolution


		/*get_pop = curr_population.getPopulation();
		std::cout << "GENERATION " << generation << "'s" << " CURRENT POPULATION AFTER CROSSOVER AND REPLACEMENT: " << std::endl;
		for (int i = 0; i < get_pop.size(); i++){
			if (i%10 == 0 || i == 0 || i == 1 || i == 5){
				std::cout << i << ") ";
				get_pop[i].print();
				std::cout << "WITH VALUE: " << TSPSolver::evaluate(get_pop[i], tsp);
				std::cout << std::endl;
			}
		}
		std::cout << std::endl;
		std::cout << std::endl;*/

		bestSolutions.push_back(bestSolCurrGen);
		generation++;
	}

	//loop to get best solution
	TSPSolution bestSol = bestSolutions[bestSolutions.size()-1];
	for (uint gen = 0; gen < bestSolutions.size(); gen++){
		if (TSPSolver::evaluate(bestSolutions[gen], tsp) < TSPSolver::evaluate(bestSol, tsp)){  //we are minimizing
			bestSol = bestSolutions[gen];
		}
	}

	return bestSol;
}
