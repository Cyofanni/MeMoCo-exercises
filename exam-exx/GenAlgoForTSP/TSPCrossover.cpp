#include "TSPCrossover.h"

/*implement mutations as simple swap between nodes*/
void TSPCrossover::do_mutation(TSPSolution& individual){
	std::uniform_real_distribution<double> unif(0, 100);
   	std::default_random_engine re;
	double rand_val = unif(re);
	int sol_size = individual.solutionSize();
	if (rand_val < mutation_probability){
		int fstnode_index = (rand() % (sol_size - 3)) + 1;
		int sndnode_index = fstnode_index + 1 + (rand() % (sol_size-2-fstnode_index));
		int swap_tmp = individual.sequence[fstnode_index];
		individual.sequence[fstnode_index] = individual.sequence[sndnode_index];
		individual.sequence[sndnode_index] = swap_tmp;
	}
}

bool TSPCrossover::is_feasible(TSPSolution sol){  //checks for duplicates
	std::sort(sol.sequence.begin(), sol.sequence.end());
	bool found_dup = true;
	int size = sol.solutionSize();
	
	for (int i = 2; i < size-1; i++){   //from i=2 because of the 2 zeroes
		if (sol.sequence[i] == sol.sequence[i+1]){
			found_dup = false;
		}
	}

	return found_dup;
}

TSPCrossover::TSPCrossover(const TSPSolution& p1, const TSPSolution& p2, double mp): parent_1(p1), parent_2(p2){
	int solSize = p1.solutionSize();
	//parent_1.print();
	std::cout << std::endl;
	//parent_2.print(); 
	std::cout << std::endl;
	mutation_probability = mp; 
}

std::vector<TSPSolution> TSPCrossover::generateOffspring(bool& feasible_flag){  //implements Partially-Mapped Crossover
	std::vector<TSPSolution> ret_offspring;
	
	int sol_size = parent_1.solutionSize();	
	TSPSolution child_1(parent_2), child_2(parent_1);   //works with Copy Constructor
	
	int fst_cut_ind_rnd = (rand() % (sol_size-3)) + 1;  //index for first cut
	int snd_cut_ind_rnd = fst_cut_ind_rnd + 1 + (rand() % (sol_size-2-fst_cut_ind_rnd)); //index for first cut
	//std::cout << fst_cut_ind_rnd << " " << snd_cut_ind_rnd  << std::endl;
	
	std::map<int, int> index_map;
	std::vector<int> conflict_zone_1;   //vector against which we check conflicts for child 1
	std::vector<int> conflict_zone_2;   //vector against which we check conflicts for child 2
	//value for still nonfixed cells = -1000		
	for (int i = 1; i < fst_cut_ind_rnd; i++){
		child_1.sequence[i] = -1000;
		child_2.sequence[i] = -1000;	
	}
	
	for (int i = fst_cut_ind_rnd; i < snd_cut_ind_rnd; i++){
		index_map.insert(std::pair<int,int>(parent_1.sequence[i], child_1.sequence[i]));  //populate map with mappings	
		index_map.insert(std::pair<int,int>(child_1.sequence[i], parent_1.sequence[i]));  //populate map with mappings			
		conflict_zone_1.push_back(child_1.sequence[i]);
		conflict_zone_2.push_back(child_2.sequence[i]);
	}
	
	for (int i = snd_cut_ind_rnd; i < sol_size-1; i++){
		child_1.sequence[i] = -1000;
		child_2.sequence[i] = -1000;	
	}	
	//Children map and zones are now correct, with -1000's at right positions
			
	/*Complete offspring before the first cut*/	
	for (int i = 1; i < fst_cut_ind_rnd; i++){
		std::vector<int>::iterator it_indicator;
	    /*Conflict check for first child*/
	    it_indicator = std::find(conflict_zone_1.begin(), conflict_zone_1.end(), parent_1.sequence[i]);
	    if (it_indicator == conflict_zone_1.end()){     //if current parent's element hadn't be put in the child
		 //std::cout << "zone 1 no conflict in pos " << i << ": " << parent_1.sequence[i] << std::endl;
		   child_1.sequence[i] = parent_1.sequence[i];    //in this case we can insert the parent value in the corresponding child, at the same position
		}
		else{ //if there was a conflict, use the mapping
			//std::cout << "zone 1 conflict in pos " << i << ": " << parent_1.sequence[i] << std::endl;
			child_1.sequence[i] = index_map.find(parent_1.sequence[i]) -> second;
		}
		/*Conflict check for second child*/
		it_indicator = std::find(conflict_zone_2.begin(), conflict_zone_2.end(), parent_2.sequence[i]);
	    if (it_indicator == conflict_zone_2.end()){     //if current parent's element hadn't be put in the child
		   //std::cout << "zone 2 no conflict in pos " << i << ": " << parent_2.sequence[i] << std::endl;
		   child_2.sequence[i] = parent_2.sequence[i];    //in this case we can insert the parent value in the corresponding child, at the same position
		}
		else{
			//std::cout << "zone 2 conflict in pos " << i << ": " << parent_2.sequence[i] << std::endl;
			child_2.sequence[i] = index_map.find(parent_2.sequence[i]) -> second;
		}	
	}
	
	/*Complete offspring after the second cut*/	
	for (int i = snd_cut_ind_rnd; i < sol_size-1; i++){
		std::vector<int>::iterator it_indicator;
	    /*Conflict check for first child*/
	    it_indicator = std::find(conflict_zone_1.begin(), conflict_zone_1.end(), parent_1.sequence[i]);
	    if (it_indicator == conflict_zone_1.end()){     //if current parent's element hadn't be put in the child
		   child_1.sequence[i] = parent_1.sequence[i];    //in this case we can insert the parent value in the corresponding child, at the same position
		}
		else{   //if there was a conflict, use the mapping
			child_1.sequence[i] = index_map.find(parent_1.sequence[i]) -> second;
		}
		/*Conflict check for second child*/
		it_indicator = std::find(conflict_zone_2.begin(), conflict_zone_2.end(), parent_2.sequence[i]);
	    if (it_indicator == conflict_zone_2.end()){     //if current parent's element hadn't be put in the child
		   child_2.sequence[i] = parent_2.sequence[i];    //in this case we can insert the parent value in the corresponding child, at the same position
		}
		else{  //if there was a conflict, use the mapping 
			child_2.sequence[i] = index_map.find(parent_2.sequence[i]) -> second;
		}	
	}
	
	/*std::cout << std::endl;
	child_1.print();
	std::cout << "  feasible?: " << is_feasible(child_1);
	std::cout << std::endl;
	child_2.print();
	std::cout << "  feasible?: " << is_feasible(child_2);	
	std::cout << std::endl;	*/
	
	if (is_feasible(child_1) && is_feasible(child_2)){
		feasible_flag = true;
		do_mutation(child_1);
	    do_mutation(child_1);
		ret_offspring.push_back(child_1);
	    ret_offspring.push_back(child_2);
		
		return ret_offspring;
	}
}

/*tries to generate an offspring until a feasible one is obtained*/
std::vector<TSPSolution> TSPCrossover::generateOffspringTrials(){
	bool feasible_obtained = false;
	std::vector<TSPSolution> ret_offspring;
	
	while (feasible_obtained == false){
		ret_offspring = generateOffspring(feasible_obtained);	
	}
	
	for (int i = 0; i < 2; i++){
	//	ret_offspring[i].print();
	//	std::cout << std::endl;
	}
	
	return ret_offspring;
}

