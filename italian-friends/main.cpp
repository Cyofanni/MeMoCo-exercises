/**
 * @file ItalianFriendsJSP.cpp
 * @brief 
 */

#include <cstdio>
#include <iostream>
#include <vector>
#include "cpxmacro.h"

using namespace std;

// error status and messagge buffer
int status;
char errmsg[BUF_SIZE];

// data
const int I = 12;
const int K = 10;
const char nameI[I] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L' };
const char nameK[K] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

const double R[I] = { 0.0, 15.0, 15.0, 60.0, 0.0, 15.0, 15.0, 60.0, 15.0, 60.0, 0.0, 15.0};
const double D[I*K] = {	
  60.0, 2.0, 30.0, 5.0, 2.0, 30.0, 5.0, 5.0, 15.0, 30.0,
  25.0, 3.0, 75.0, 10.0, 2.0, 30.0, 5.0, 2.0, 30.0, 5.0,
  10.0, 5.0, 15.0, 30.0,10.0, 5.0, 15.0, 30.0, 2.0, 30.0,
  1.0, 1.0, 1.0, 90.0, 2.0, 30.0, 1.0, 90.0, 2.0, 30.0,
  60.0, 2.0, 30.0, 5.0, 2.0, 30.0, 5.0, 5.0, 15.0, 30.0,
  25.0, 3.0, 75.0, 10.0, 2.0, 30.0, 5.0, 2.0, 30.0, 5.0,
  10.0, 5.0, 15.0, 30.0,10.0, 5.0, 15.0, 30.0, 2.0, 30.0,
  1.0, 1.0, 1.0, 90.0, 2.0, 30.0, 1.0, 90.0, 2.0, 30.0,
  60.0, 2.0, 30.0, 5.0, 2.0, 30.0, 5.0, 5.0, 15.0, 30.0,
  25.0, 3.0, 75.0, 10.0, 2.0, 30.0, 5.0, 2.0, 30.0, 5.0,
  10.0, 5.0, 15.0, 30.0,10.0, 5.0, 15.0, 30.0, 2.0, 30.0,
  1.0, 1.0, 1.0, 90.0, 2.0, 30.0, 1.0, 90.0, 2.0, 30.0
};
const int S[I*K] = { 
  0, 2, 1, 3, 5, 9, 4, 8, 6, 7,
  2, 1, 0, 6, 5, 7, 8, 3, 4, 9,
  1, 2, 9, 3, 7, 8, 4, 6, 5, 0,
  3, 0, 2, 5, 6, 4, 7, 1, 8, 9,
  0, 2, 1, 3, 5, 9, 4, 8, 6, 7,
  2, 1, 0, 6, 5, 7, 8, 3, 4, 9,
  1, 2, 9, 3, 7, 8, 4, 6, 5, 0,
  3, 0, 2, 5, 6, 4, 7, 1, 8, 9,
  0, 2, 1, 3, 5, 9, 4, 8, 6, 7,
  2, 1, 0, 6, 5, 7, 8, 3, 4, 9,
  1, 2, 9, 3, 7, 8, 4, 6, 5, 0,
  3, 0, 2, 5, 6, 4, 7, 1, 8, 9
 };
double M; //TO BE INITIALIZED!!!
			
const int NAME_SIZE = 512;
char name[NAME_SIZE];
	
void setupLP(CEnv env, Prob lp, int & numVars )
{
	// determine a feasible value for M [ max_{i in I} R_i + sum_{i,k} D_{i,k} ]
	M = 0;
	for (int i = 0; i < I; ++i ) if ( R[i] > M ) M = R[i];
	for (int i = 0; i < I; i++) {
		for (int k = 0; k < K; k++) {
			M += D[i * K + k];
		}
	}

	const int h_init = 0; // first index for h vars
	// add h vars
	for (int i = 0; i < I; i++)
	{
		for (int k = 0; k < K; k++)
		{
			char htype = 'C';
			double obj = 0.0;
			double lb = 0.0;
			double ub = CPX_INFBOUND;
			snprintf(name, NAME_SIZE, "h_%c_%c", nameI[i], nameK[k]);
			char* hname = (char*)(&name[0]);
			CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &htype, &hname );
		}
	}

	// add x vars
	const int x_init = CPXgetnumcols(env, lp); // first index for x vars
	for (int k = 0; k < K; k++)
	{
		for (int i = 0; i < I; i++)
		{
			for (int j = 0; j < I; j++)
			{
				if ( i==j ) continue;
				
				// TODO...DONE
				char xtype = 'B';
				double obj = 0.0;
				double lb = 0.0;
				double ub = 1.0;
				snprintf(name, NAME_SIZE, "x_%c_%c_%c", nameI[i], nameI[j], nameK[k]);
				char* xname = (char*)(&name[0]);
				CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &xtype, &xname );
			}
		}
	}
	// add y var
	const int yIdx = CPXgetnumcols(env, lp); // index of y var
	{
		char ytype = 'C';
		double lb = 0.0;
		double ub = CPX_INFBOUND;
		double obj = 1.0;
		snprintf(name, NAME_SIZE, "y");
		char* yname = (char*)(&name[0]);
		CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &ytype, &yname );
	}
	numVars = CPXgetnumcols(env, lp);
	
	// add finish-time constraints [ y >= h_{i S[i,|K|]} + D_{i S[i,|K|]} ]
	for (int i = 0; i < I; i++)
	{
		std::vector<int> idx(2);
		idx[0] = yIdx;
		idx[1] = h_init + i*K + S[i*K + (K-1)]; // sigma(i,last)
		std::vector<double> coef(2);
		coef[0] = 1.0;
		coef[1] = -1.0;
		char sense = 'G';
		int matbeg = 0;
		CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, idx.size(), &D[i*K + S[i*K + (K-1)]], &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
	}
	
	// add wake-up constraints [ h_{i S[i,0]} >= R_i ]
	//TODO...done
	for (int i = 0; i < I; i++)
	{
		int idx = h_init + i*K + S[i*K + 0]; // h[ i , sigma(i,first) ]
		double coef = 1.0;
		char sense = 'G';
		int matbeg = 0;
		CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, 1, &R[i], &sense, &matbeg, &idx, &coef, 0, 0 );
	}

	// add non-overlapping constraints between consecutive newspapers [ h_{i S[i,l]} >= h_{i S[i,l-1]} + D_{i S[i,l-1]} ] 
	for (int i = 0; i < I; i++)
	{
		for (int l = 1; l < K; l++)
		{
			//TODO...done
			std::vector<int> idx(2);
			idx[0] = h_init + i*K + S[i*K + l ];		// h[ i , sigma(i,l) ]
			idx[1] = h_init + i*K + S[i*K + (l-1) ];	// h[ i , sigma(i,l-1) ]
			std::vector<double> coef(2);
			coef[0] = 1.0;
			coef[1] = -1.0;
			char sense = 'G';
			int matbeg = 0;
			CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, idx.size(), &D[i*K + S[i*K + (l-1)]], &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
		}
	}
	
	// add non-overlapping constraints between people on the same newspaper 
	int xIdx = x_init;
	for (int k = 0; k < K; k++)
	{
		for (int i = 0; i < I; i++)
		{
			for (int j = 0; j < I; j++)
			{
				if ( i==j ) continue;
				
				// 1 [ h_{i k} >= h_{j k]} + D_{j k]} - M x_ijk ]
				std::vector<int> idx(3);
				idx[0] = h_init + i*K + k;
				idx[1] = h_init + j*K + k;
				idx[2] = xIdx;
				std::vector<double> coef(3);
				coef[0] = 1.0;
				coef[1] = -1.0;
				coef[2] = M;
				char sense = 'G';
				int matbeg = 0;
				CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, idx.size(), &D[j*K + k], &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
				
				// 2 [ h_{j k} >= h_{i k]} + D_{i k]} - M (1 -x_ijk) ]
				//TODO...done
				idx[0] = h_init + j*K + k;
				idx[1] = h_init + i*K + k;
				idx[2] = xIdx;
				coef[0] = 1.0;
				coef[1] = -1.0;
				coef[2] = - M;
				sense = 'G';
				matbeg = 0;
				double rhs = D[i*K + k] - M;
				CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
				
				xIdx++; // we have as many constraints as many x variables
					// and the nested loops for constraints adding 
					// follow the same order as the one for x variable adding
			}
		}
	}
	
	// print (debug)
	CHECKED_CPX_CALL( CPXwriteprob, env, lp, "ItalianFriendsJSP.lp", 0 );
}

int main (int argc, char const *argv[])
{
	//TODO...done
	try
	{
		// init
		DECL_ENV( env );
		DECL_PROB( env, lp );
		// setup LP
		int numVars;
		setupLP(env, lp, numVars);
		

		// set parameters for CPLEX engine (here, log mip process info)
		//CHECKED_CPX_CALL( CPXsetintparam, env, CPX_PARAM_MIPDISPLAY, 3);
		CHECKED_CPX_CALL( CPXsetintparam, env, CPX_PARAM_SCRIND, CPX_ON);
		//CHECKED_CPX_CALL( CPXsetintparam, env, CPX_PARAM_MIPINTERVAL, 10);
		//CHECKED_CPX_CALL( CPXsetdblparam, env, CPXPARAM_TimeLimit, 15);
		
		
		// optimize
		double timeStart;
		CHECKED_CPX_CALL( CPXgettime, env, &timeStart );		

		CHECKED_CPX_CALL( CPXmipopt, env, lp );
		
		double timeFinish;
		CHECKED_CPX_CALL( CPXgettime, env, &timeFinish );		
		// print
		double objval;
		CHECKED_CPX_CALL( CPXgetobjval, env, lp, &objval );
		std::cout << "Available objval: " << objval << std::endl;
		std::cout << "Running time: " << timeFinish - timeStart << " seconds" << std::endl;
		int n = CPXgetnumcols(env, lp);
		cout << "check num var: " << n << " == " << numVars << endl;
		if (n != numVars) { throw std::runtime_error(std::string(__FILE__) + ":" + STRINGIZE(__LINE__) + ": " + "different number of variables"); }
		std::vector<double> varVals;
		varVals.resize(n);
		CHECKED_CPX_CALL( CPXgetx, env, lp, &varVals[0], 0, n - 1 );
//		for ( int i = 0 ; i < n ; ++i ) {
//			std::cout << "var in position " << i << " : " << varVals[i] << std::endl;
//  		}
		CHECKED_CPX_CALL( CPXsolwrite, env, lp, "ItalianFriendsJSP.sol" );
		// free
		CPXfreeprob(env, &lp);
		CPXcloseCPLEX(&env);
	}
	catch(std::exception& e)
	{
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}

	return 0;
}
