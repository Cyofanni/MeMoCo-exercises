/**
 * @file main.cpp
 * @brief 
 */

#include <stdexcept>
#include <ctime>
#include <sys/time.h>
#include <random>
#include <chrono>

#include "TSPGeneticRunner.h"

// error status and messagge buffer
int status;
char errmsg[255];

int main (int argc, char const *argv[])
{
  try
  {
    if (argc < 7){ 
       throw std::runtime_error("usage: ./main filename.dat pop_size n_of_generations mut_prob(percent) random_or_simanneal sim_ann_max_iter");
    }
    
    /// create the instance (reading data)
    TSP tspInstance;
    tspInstance.read(argv[1]);
    
    int pop_size = atoi(argv[2]);
    int n_of_generations = atoi(argv[3]); 
    double mut_prob = atof(argv[4]);
    bool random_or_simanneal = atoi(argv[5]);
    int sim_ann_max_iter = atoi(argv[6]);
    
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    srand(time(NULL));

    std::uniform_real_distribution<double> distribution (0.0, 100.0);
    
    std::cout << "population size: "<< pop_size << std::endl << "number of generations: " << 
         n_of_generations << std::endl << "mutation probability: " 
         << mut_prob << std::endl << "simulated annealing?: " << random_or_simanneal << std::endl << 
			"simulated annealing max iterations: " <<  sim_ann_max_iter << std::endl;
   		
	/// initialize clocks for running time recording
    ///   two ways:
    ///   1) CPU time (t2 - t1)
    clock_t t1,t2;
    t1 = clock();
    ///   2) wall-clock time (tv2 - tv1)
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);
    
    /*TSPGeneticRunner(TSP tsp, int population_size, uint max_iter_n,  
								   double mut_prob, uint sim_ann_max_iter,
								   bool choose_init, rg, distr)*/			
	TSPGeneticRunner exec(tspInstance, pop_size, n_of_generations, mut_prob,
											random_or_simanneal, sim_ann_max_iter,
											generator, distribution);
	TSPSolution bestSolution = exec.run();
    std::cout << std::endl;
    std::cout << std::endl; 
    std::cout << "*******BEST solution is: " << std::endl; 
    bestSolution.print();
    std::cout << "WITH VALUE: " << TSPSolver::evaluate(bestSolution, tspInstance);
    std::cout << std::endl;
    
    /// final clocks
    t2 = clock();
    gettimeofday(&tv2, NULL);
    
    std::cout << "in " << (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6)) << " seconds (user time)\n";
    std::cout << "in " << (double)(t2-t1) / CLOCKS_PER_SEC << " seconds (CPU time)\n";  
  }
  catch(std::exception& e)
  {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
  } 
  return 0;
}
