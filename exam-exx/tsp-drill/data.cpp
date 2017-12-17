/**zero;
 * @file data.cpp
 * @brief 
 *
 */

#include "data.h"
#include <cmath>
#include <iostream>
#include <fstream>

const double EPS = 1e-5; //tolerance for numeric issues

void Data::read(const char* filename)
{
	std::ifstream in(filename);
	// read data from file
	in >> n; //number of nodes
        // # of nodes initialized
	std::cout << "n = " << n << std::endl;
	char name;
	for (int i = 0; i < static_cast<int>(n); i++){
		in >> name;
		node_names.push_back(name);
		std::cout << name << std::endl;
	}
	// node names initialized
	
	double cost;
        for (int i = 0; i < n*(n-1); i++){
		in >> cost ;
		time_costs.push_back(cost);
		std::cout << cost << std::endl;
	}
        //time costs initialized

	in >> zero_index;
        //zero node's index initialized

	zero_name = node_names[zero_index];
        //zero node's name initialized
	std::cout << zero_name << " at position " << zero_index << std::endl;

	in.close();
}
