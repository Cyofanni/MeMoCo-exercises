/**
 * @file main.cpp
 * @brief 
 */


#include <stdexcept>
#include <ctime>
#include <sys/time.h>

#include "TSPGeneticRunner.h"

// error status and messagge buffer
int status;
char errmsg[255];

#define POP_SIZE 10     //population size


int main (int argc, char const *argv[])
{
  try
  {
    if (argc < 6){ 
       throw std::runtime_error("usage: ./main filename.dat pop_size n_of_generations mut_prob(percent) random_or_simanneal");
    }
    
    /// create the instance (reading data)
    TSP tspInstance;
    tspInstance.read(argv[1]);
    
    int pop_size = atoi(argv[2]);
    int n_of_generations = atoi(argv[3]); 
    double mut_prob = atof(argv[4]);
    bool random_or_simanneal = atoi(argv[5]);
    int sim_ann_max_iter = 10;    //hard-coded
    
    std::cout << "population size: "<< pop_size << std::endl << "number of generations: " << 
         n_of_generations << std::endl << "mutation probability: " 
         << mut_prob << std::endl << "simulated annealing?: " << random_or_simanneal << std::endl;
    
    
    //1st STEP: TEST CROSSOVER
    //create two solutions
    TSPSolution sol1(tspInstance);
    TSPSolution sol2(tspInstance);
    TSPSolver solver;
    srand(time(NULL));
    solver.initRnd(sol1);
    solver.initRnd(sol2);
    sol1.print();
    std::cout << std::endl;
    sol2.print();
    std::cout << std::endl;
    //two random solutions are ready
    TSPCrossover co(sol1, sol2, mut_prob);   
    //CONTRUCTOR WORKS
    std::vector<TSPSolution> ofspr;
    co.generateOffspring();
    /*for (int i = 0; i < ofspr.size(); i++){
		ofspr[i].print();
		std::cout << std::endl;
	}*/
    
    /*TSPGeneticRunner(TSP tsp, int population_size, uint max_iter_n,  
								   double mut_prob, uint sim_ann_max_iter,
								   bool choose_init)*/
	
    
    /// initialize clocks for running time recording
    ///   two ways:
    ///   1) CPU time (t2 - t1)
    clock_t t1,t2;
    t1 = clock();
    ///   2) wall-clock time (tv2 - tv1)
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);
    
    /// create solver class
    //TSPGeneticRunner execObj(tspInstance, pop_size, n_of_generations, mut_prob, sim_ann_max_iter,random_or_simanneal);
    //TSPSolution bestSolution = execObj.run();
     
    /// final clocks
    t2 = clock();
    gettimeofday(&tv2, NULL);
    
    /*std::cout << "BEST solution is: "; 
    bestSolution.print();
    std::cout << "(value : " << TSPSolver::evaluate(bestSolution,tspInstance) << ")\n";
    
    std::cout << "in " << (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6)) << " seconds (user time)\n";
    std::cout << "in " << (double)(t2-t1) / CLOCKS_PER_SEC << " seconds (CPU time)\n";*/
    
  }
  catch(std::exception& e)
  {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
  } 
  return 0;
}
