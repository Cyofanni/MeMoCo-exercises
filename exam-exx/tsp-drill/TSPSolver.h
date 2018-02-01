/**
 * @file TSPSolver.h
 * @brief 
 *
 */

#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include "cpxmacro.h"
#include <vector>

/**
 * Class that stores the data for electronic board frame problem
 * It also provides a method for reading an instance from file
 */

class Data
{
public:
	Data() : n(0) {}
	int n;    //#of nodes
	std::vector<std::vector<double> > costs; //costs of arcs
	void read(const char* filename);
};

/**
 * Class that provides the building blocks for solving 
 * the electronic board problem
 */
class TSPSolver
{
protected:
	// cplex data
	Env env;
	Prob lp;	
	
public:
	/** Constructor */
	TSPSolver(Env _env, Prob _lp) : env(_env), lp(_lp) {}
	
	/**
	 * Init cplex problem with instance data
	 */
	void setupProblem(const Data& data);
	
	/**
	 * Solve current mi problem
	 */
	void solveProblem(std::vector<double>& x, double& objval);
};

#endif /* TSPSOLVER_H */
