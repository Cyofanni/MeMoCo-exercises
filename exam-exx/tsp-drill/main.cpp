/**
 * @file main.cpp (tsp-drill)
 * @brief 
 */

#include <cstdio>
#include <iostream>
#include <vector>
#include "cpxmacro.h"
#include "data.h"

using namespace std;

// error status and messagge buffer
int status;
char errmsg[BUF_SIZE];
			
const int NAME_SIZE = 512;
char name[NAME_SIZE];
	
void setupLP(CEnv env, Prob lp, Data data) {
	// add x vars
	for (int i = 0; i < static_cast<int>(data.n); i++) {
		for (int j = 0; j < static_cast<int>(data.n); j++) {
			if (j != i){
				char xtype = 'C';
				double lb = 0.0;
				double ub = data.n;
				snprintf(name, NAME_SIZE, "x_%c_%c", data.node_names[i], data.node_names[j]);
				char* xname = (char*)(&name[0]);
				cout << xname << endl;
				/*Obj function has no 'x_ij' variables, so put zero as coefficient (fifth parameter)"*/
				CHECKED_CPX_CALL(CPXnewcols, env, lp, 1, 0, &lb, &ub, &xtype, &xname);
				/// status =      CPXnewcols (env, lp, ccnt, obj      , lb  , ub, xctype, colname);
			}
		}
	}
	// add y vars 
	for (int i = 0; i < static_cast<int>(data.n); i++) {
		for (int j = 0; j < static_cast<int>(data.n); j++) {
			if (j != i){
				char xtype = 'B';
				double lb = 0.0;
				double ub = 1.0;
				snprintf(name, NAME_SIZE, "y_%c_%c", data.node_names[i], data.node_names[j]);
				char* xname = (char*)(&name[0]);
				cout << xname << endl;
				CHECKED_CPX_CALL(CPXnewcols, env, lp, 1, &(data.time_costs[i*data.n+j]), &lb, &ub, &xtype, &xname);
				/// status =      CPXnewcols (env, lp, ccnt, obj      , lb  , ub, xctype, colname);
			}
		}
	}

    ///////////////////////////////////////////////////////////
    //
    // now variables are stored in the following order
    //  x00 x01 ...   x10 x11 ... #xij #    ... ...   y00   y01   ...   y10   y11     ... #yij      #   ... ...
    // with indexes
    //
    ////////////

	//add constraint on maximum flow coming out of 'ZERO' node (1st constraint from the model)
	//NB: no external loop, it's just a single constraint
        //idx contains the variables in constraint's left side (data.n-1 variables of course)
	{
		std::vector<int> idx(static_cast<int>(data.n)-1);
        	std::vector<double> coef(static_cast<int>(data.n)-1, 1.0);   //coefficients for the variables in the constraints (data.n-1 coefs for data.n-1 vars)
		for (int j = 0; j < static_cast<int>(data.n)-1; j++)
		{
			idx[j] = data.zero_index*(data.n-1) + j; // corresponds to variable x_0j
		}        
		char sense = 'E';
		
        	int matbeg = 0;
		CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &data.n, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);
	} 
    	/// status =      CPXaddrows (        env, lp, colcnt, rowcnt, nzcnt     , righthandside  , sense , rmatbeg, rmatind, rmatval , newcolname, newrowname);
	

	// add incoming/outgoing flow constraints (2nd set of constraints from the model)
	for (int k = 0; k < static_cast<int>(data.n)-1; k++) {
		std::vector<int> idx1(static_cast<int>(data.n)-1);   //variables in the 1st summation
		std::vector<int> idx2(static_cast<int>(data.n)-1);   //variables in the 2nd summation
				
		std::vector<double> coef1(static_cast<int>(data.n)-1, 1.0);  //coefs for the variables in the 1st summation
		std::vector<double> coef2(static_cast<int>(data.n)-1, -1.0);  //coefs for the variables in the 2nd summation
				
		char sense = 'E';
		for (int i = 0; i < static_cast<int>(data.n); i++){
			idx1[i] = i*(data.n-1) + k; // corresponds to variable x_ik
		}
		for (int j = 0; j < static_cast<int>(data.n); j++){
			idx2[j] = k*(data.n-1) + j; // corresponds to variable x_kj
		}
		idx1.insert(idx1.end(), idx2.begin(), idx2.end());  //concatenate the idx's
		coef1.insert(coef1.end(), coef2.begin(), coef2.end());  //concatenate the coef's
		
		int matbeg = 0;
		double righthand = 1;
		CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx1.size(), &righthand, &sense, &matbeg, &idx1[0], &coef1[0], NULL, NULL);
	}

	// add third set of constraints (forall i, sum{i,j} y_ij = 1)
	for (int i = 0 ; i < static_cast<int>(data.n); i++) {
		std::vector<int> idx(static_cast<int>(data.n)-1);
		std::vector<double> coef(static_cast<int>(data.n)-1, 1.0);
		for (int j = 0; j < static_cast<int>(data.n)-1; j++)
		{
		      idx[j] = (data.n*(data.n-1)) + i*(data.n-1) + j; //corresponds to variable y_ij: (data.n*(data.n-1)) is the offset needed to get the y_ij's
		}        
		char sense = 'E';
		
        	int matbeg = 0;
		double righthand = 1;
		CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &righthand, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);
	}

	// add fourth set of constraints (forall j, sum{i,j} y_ij = 1)
	//Like the previous, but doesn't trigger SIGSEGV
	for (int j = 0 ; j < static_cast<int>(data.n); j++) {
		std::vector<int> idx(static_cast<int>(data.n)-1);
		std::vector<double> coef(static_cast<int>(data.n)-1, 1.0);
		for (int i = 0; i < static_cast<int>(data.n)-1; i++)
		{
		      idx[i] = (data.n*(data.n-1)) + i*(data.n-1) + j; //corresponds to variable y_ij: (data.n*(data.n-1)) is the offset needed to get the y_ij's
		}        
		char sense = 'E';
		
        	int matbeg = 0;
		double righthand = 1;
		CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &righthand, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);
	}
	
	// add fifth set of constraints (linking constraints) (x_ij - N y_ij <= 0 -- all variables on the left side!!!)
	for (int i = 0 ; i < static_cast<int>(data.n); ++i ) {
		for (int j = 0 ; j < static_cast<int>(data.n)-1; ++j) {
			std::vector<int> idx(2);
			idx[0] = i*(data.n-1) + j;;        // x var
			idx[1] = (data.n*(data.n-1)) + i*(data.n-1) + j;   // y var  (data.n*(data.n-1)) is the offset needed to get the y_ij's
			std::vector<double> coef(2); 
			coef[0] = 1.0;
			coef[1] = -data.n;
			char sense = 'L';
			double rhs = 0.0;
			int matbeg = 0;
			CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, 2, &rhs, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
		}
	}
}


int main (int argc, char const *argv[]) {
	//try {
		// init
		DECL_ENV(env);
		DECL_PROB(env, lp);
		// setup LP
		Data data;
		data.read(argv[1]);
		setupLP(env, lp, data);
		// optimize
		CHECKED_CPX_CALL(CPXmipopt, env, lp);
		// print
		double objval;
		CHECKED_CPX_CALL(CPXgetobjval, env, lp, &objval);
	/*	std::cout << "Objval: " << objval << std::endl;
		int n = CPXgetnumcols(env, lp);

			
		if (n != 2*(data.n)*(data.n)+1) { throw std::runtime_error(std::string(__FILE__) + ":" + STRINGIZE(__LINE__) + ": " + "different number of variables"); }
	  	std::vector<double> varVals;
		varVals.resize(n);
  		CHECKED_CPX_CALL(CPXgetx, env, lp, &varVals[0], 0, n - 1);
		/// status =      CPXgetx (env, lp, x          , 0, CPXgetnumcols(env, lp)-1);
  		for (int i = 0 ; i < n; ++i) {
  	  		std::cout << "var in position " << i << " : " << varVals[i] << std::endl;
  	  		/// to read variables names the API function ``CPXgetcolname'' may be used (it is rather tricky, see the API manual if you like...)
  	  		/// status = CPXgetcolname (env, lp, cur_colname, cur_colnamestore, cur_storespace, &surplus, 0, cur_numcols-1);
  		}
		CHECKED_CPX_CALL( CPXsolwrite, env, lp, "tsp-drill.sol" );
		// free
		CPXfreeprob(env, &lp);
		CPXcloseCPLEX(&env);*/
	//}
	/*catch(std::exception& e) {
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}*/

	return 0;
}

