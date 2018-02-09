/**
 * @file main.cpp
 * @brief 
 */

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <ctime>
#include <sys/time.h>
#include "cpxmacro.h"
#include "TSPSolver.h"

using namespace std;

// error status and messagge buffer
int status;
char errmsg[BUF_SIZE];

int main (int argc, char const *argv[])
{
	try
	{
		DECL_ENV(env);
		DECL_ENV(env2);
		DECL_PROB(env, lp);
		TSPSolver tspSolver(env, lp);
		Data data;
		data.read(argv[1]);

		/// initialize clocks for running time recording
		///   two ways:
		///   1) CPU time (t2 - t1)
		clock_t t1,t2;
		t1 = clock();
		///   2) wall-clock time (tv2 - tv1)
		struct timeval  tv1, tv2;
		gettimeofday(&tv1, NULL);

		tspSolver.setupProblem(data);

		std::vector<double> x;
		double objval;
		tspSolver.solveProblem(x, objval);
		

		std::cout << "y vars: " << std::endl;
		
		for (unsigned int j = 0; j < x.size()/2; j++) {
			if (j>0 && j%(data.n) == 0) std::cout << std::endl;
			std::cout << setw(7) << x[j] << " ";
		}
		std::cout << std::endl << "x vars: " << std::endl;
		for (unsigned int j = x.size()/2; j < x.size(); j++) {
			if (j>x.size()/2 && j%(data.n) == 0) std::cout << std::endl;
			std::cout << setw(7) << x[j] << " ";
		}
		std::cout << std::endl;

		std::cout << "***OPTIMAL OBJECTIVE VALUE***: " << objval << std::endl;
		
		
		/// final clocks
		t2 = clock();
		gettimeofday(&tv2, NULL);
    
		std::cout << "in " << (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6)) << " seconds (user time)\n";
		std::cout << "in " << (double)(t2-t1) / CLOCKS_PER_SEC << " seconds (CPU time)\n";  		

		//debug
		CHECKED_CPX_CALL(CPXwriteprob, env, lp, "final.lp", NULL);

		// free
		CPXfreeprob(env, &lp);
		CPXcloseCPLEX(&env);
		CPXcloseCPLEX(&env2);
	}
	catch(std::exception& e)
	{
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}
	return 0;
}
