#include "TSPCrossover.h"

TSPCrossover::TSPCrossover(const TSPSolution& p1, const TSPSolution& p2){
	parent_1 = p1;
	parent_2 = p2; 
}

std::vector<TSPSolution> TSPCrossover::generateOffspring(){  //implements Partially-Mapped Crossover
	std::vector<TSPSolution> ret_offspring;
	
	int sol_size = parent_1.solutionSize();	
	TSPSolution child_1, child_2;
	child_1.tsp = parent_1.tsp;     //inherit TSP structure
	child_2.tsp = parent_2.tsp;     //inherit TSP structure
	child_1.sequence.reserve(sol_size);
	child_2.sequence.reserve(sol_size);
	
	int fst_cut_ind_rnd = (rand() % sol_size) + 1;  //index for first cut
	int snd_cut_ind_rnd = (rand() % sol_size) + (fst_cut_ind_rnd+1);  //index for first cut
	
	std::map<int, int> index_map;
	//value for nonfixed cells = -1000;
	for (int i = 0; i < child_1.sequence.capacity(); i++){
		child_1.sequence[i] = -1000;
		child_2.sequence[i] = -1000;		
	}
	
	std::vector<int> conflict_zone_1;   //vector against which we check conflicts for child 1
	std::vector<int> conflict_zone_2;   //vector against which we check conflicts for child 2
	
	//Step 1) Copy the elements between the cuts
	for (int i = fst_cut_ind_rnd; i < snd_cut_ind_rnd; i++){
		child_1.sequence[i] = parent_2.sequence[i];
		child_2.sequence[i] = parent_1.sequence[i];
		index_map.insert(std::pair<int,int>(parent_1.sequence[i], child_1.sequence[i]));  //populate map with mappings	
		conflict_zone_1.push_back(child_1.sequence[i]);
		conflict_zone_2.push_back(child_2.sequence[i]);
	}
	
	/*Complete offspring before the first cut*/	
	for (int i = 0; i < fst_cut_ind_rnd; i++){
		std::vector<int>::iterator it_indicator;
	    /*Conflict check for first child*/
	    it_indicator = std::find(conflict_zone_1.begin(), conflict_zone_1.end(), parent_1.sequence[i]);
	    if (it_indicator == conflict_zone_1.end()){     //if current parent's element hadn't be put in the child
		   child_1.sequence[i] = parent_1.sequence[i];    //in this case we can insert the parent value in the corresponding child, at the same position
		}
		else{ //if there was a conflict, use the mapping
			child_1.sequence[i] = index_map.at(parent_1.sequence[i]);
		}
		/*Conflict check for second child*/
		it_indicator = std::find(conflict_zone_2.begin(), conflict_zone_2.end(), parent_2.sequence[i]);
	    if (it_indicator == conflict_zone_2.end()){     //if current parent's element hadn't be put in the child
		   child_2.sequence[i] = parent_2.sequence[i];    //in this case we can insert the parent value in the corresponding child, at the same position
		}
		else{
			child_2.sequence[i] = index_map.at(parent_2.sequence[i]);
		}	
	}
	
	/*Complete offspring after the second cut*/	
	for (int i = snd_cut_ind_rnd; i < sol_size; i++){
		std::vector<int>::iterator it_indicator;
	    /*Conflict check for first child*/
	    it_indicator = std::find(conflict_zone_1.begin(), conflict_zone_1.end(), parent_1.sequence[i]);
	    if (it_indicator == conflict_zone_1.end()){     //if current parent's element hadn't be put in the child
		   child_1.sequence[i] = parent_1.sequence[i];    //in this case we can insert the parent value in the corresponding child, at the same position
		}
		else{   //if there was a conflict, use the mapping
			child_1.sequence[i] = index_map.at(parent_1.sequence[i]);
		}
		/*Conflict check for second child*/
		it_indicator = std::find(conflict_zone_2.begin(), conflict_zone_2.end(), parent_2.sequence[i]);
	    if (it_indicator == conflict_zone_2.end()){     //if current parent's element hadn't be put in the child
		   child_2.sequence[i] = parent_2.sequence[i];    //in this case we can insert the parent value in the corresponding child, at the same position
		}
		else{  //if there was a conflict, use the mapping 
			child_2.sequence[i] = index_map.at(parent_2.sequence[i]);
		}	
	}
	
	ret_offspring.push_back(child_1);
	ret_offspring.push_back(child_2);
	
	return ret_offspring;
}
