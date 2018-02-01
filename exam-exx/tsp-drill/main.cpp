/**
 * @file main.cpp
 * @brief 
 */

#include <iostream>
#include <iomanip>
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
