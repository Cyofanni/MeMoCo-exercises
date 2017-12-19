/**
 * @file data.h
 * @brief 
 *
 */

#ifndef DATA_H
#define DATA_H

#include "cpxmacro.h"
#include <vector>

/**
 * Class that stores the data for a tsp-drill problem
 * It also provides a method for reading an instance from file
 */

class Data
{
public:
	//QUESTION: are these types (e.g. vector) ok for CPlex functions?
	Data() : n(0) {}
	double n; // number of nodes
	std::vector<char> node_names; // nodes' names
	std::vector<double> time_costs;   // time costs to move the drill from i to j, LINEARIZED (just an implementation choice!)
	int zero_index;	//index of the 'zero' node  
	char zero_name; //name for the node at index 'zero_index'
	void read(const char* filename);
};


#endif
