/**
 * @file TSPSolver.cpp
 * @brief
 *
 */

#include "TSPSolver.h"
#include <iostream>
#include <fstream>
#include <cmath>

const double EPS = 1e-5; //tolerance for numeric issues

void Data::read(const char* filename)
{
    std::ifstream in(filename);
    //read data from file
    in >> n; // number of nodes
    std::cout << "n = " << n << std::endl;
    for (int i = 0; i < n; i++) { //for each possible arc
        std::vector<double> row;
        double c;
        for (int j = 0; j < n; j++) {
            in >> c;
            row.push_back(c);
            std::cout << row[j] << "\t";
        }
        costs.push_back(row);
        std::cout << std::endl;
    }
    in.close();
}

void TSPSolver::setupProblem(const Data& data)
{
    // setup initial LP
    double lb_zero = 0.0;
    double ub_binvar = 1.0;
    double ub_cont = CPX_INFBOUND;

    //add y vars [objective function: sum{i,j} C_ij y_ij + ...]
    for (int i = 0; i < data.n; i++) {
        for (int j = 0; j < data.n; j++) {
            char ytype = 'B';
            CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &data.costs[i][j], &lb_zero, &ub_binvar, &ytype, NULL );
        }
    }

    //add x vars
    for (int i = 0; i < data.n; i++)
    {
        for (int k = 0; k < data.n; k++)
        {
            double zero_coef = 0.0;
            char xtype = 'C';
            CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &zero_coef, &lb_zero, &ub_cont, &xtype, NULL );
        }
    }

    const int x_init_index = data.n*data.n;
    std::vector<int> matbeg(data.n, 0);
    std::vector<int> idx(data.n);
    std::vector<double> coefs(data.n);
    std::vector<char> sense(data.n, 'E');
    
    // add source flow constraint [ sum_j:{0,j} x_{0,j} = |N|]
    for (int i = 0; i < data.n; i++) {
        idx[i] = x_init_index + i;
        coefs[i] = 1.0;
    }
    const double rhsn = data.n;
    CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, data.n, &rhsn, &sense[0], &matbeg[0], &idx[0], &coefs[0], NULL, NULL);

    const double rhs = 1.0;
    
    //add flow constraint [ sum_i: (i,k) x_{i,k} - sum_j: (k,j) x_{k,j} = 1 for each k !=0]
    int n_times2 = data.n*2;
    std::vector<int> idx2(n_times2);
    std::vector<double> coefs_2(n_times2);
    std::vector<int> matbeg2(n_times2, 0);
    std::vector<char> sense2(n_times2, 'E');
    for (int k = 1; k < data.n; k++){
        for (int i = 0; i < data.n; i++){
            idx2[i] = x_init_index + i*data.n + k;
            coefs_2[i] = 1.0;
        }
        for (int j = 0; j < data.n; j++){
            idx2[data.n + j] = x_init_index + k*data.n + j;
            coefs_2[data.n + j] = -1.0;
        }
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, n_times2, &rhs, &sense2[0], &matbeg2[0], &idx2[0], &coefs_2[0], NULL, NULL);
    }

    // add visit constraint 1 [ sum_j:{i,j} y_{0,j} = 1 i fixed]
    for (int i = 0; i < data.n; i++) {
        coefs[i] = 1.0;
        for (int j = 0; j < data.n; j++) {
            idx[j] = i*data.n + j;
        }
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, data.n, &rhs, &sense[0], &matbeg[0], &idx[0], &coefs[0], NULL, NULL);
    }

    // add visit constraint 2 [ sum_j:{i,j} y_{0,j} = 1 j fixed]
    for (int j = 0; j < data.n; j++){
        coefs[j] = 1.0;
        for (int i = 0; i < data.n; i++){
            idx[i] = i*data.n + j;
        }
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, data.n, &rhs, &sense[0], &matbeg[0], &idx[0], &coefs[0], NULL, NULL);
    }


    //logic flow constraint [x_{i,j} - |N|*y_{i,j} <= 0  foreach (i,j)]
    char new_sense = 'L';
    std::vector<int> vars(2);
    std::vector<double> new_coefs(2);
    std::vector<int> new_matbeg(2, 0);
    const double rhs0 = 0.0;
    for (int i = 0; i < data.n; i++) {
        for (int j = 0; j < data.n; j++) {
            new_coefs[0] = 1.0;
            vars[0] = x_init_index + i*data.n + j;
            
            new_coefs[1] = -data.n;
            vars[1] = i*data.n + j;
            
            CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, 2, &rhs0, &new_sense, &new_matbeg[0], &vars[0], &new_coefs[0], NULL, NULL);
        }
    }
    
    // debug
    CHECKED_CPX_CALL( CPXwriteprob, env, lp, "initial.lp", NULL );
}

void TSPSolver::solveProblem(std::vector<double>& x, double& objval)
{
    // solve
    CHECKED_CPX_CALL(CPXmipopt, env, lp);
    // get current LP obj value
    CHECKED_CPX_CALL( CPXgetobjval, env, lp, &objval );
    
    int n = CPXgetnumcols(env, lp);
    x.resize(n);
    CHECKED_CPX_CALL( CPXgetx, env, lp, &x[0], 0, n - 1 );
}
