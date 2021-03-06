/**
* @file TSPSolution.h
* @brief TSP solution
*
*/

#ifndef TSPSOLUTION_H
#define TSPSOLUTION_H

#include <vector>

#include "TSP.h"
#include <ctime>

/**
* TSP Solution representation: ordered sequence of nodes (path representation)
*/
class TSPSolution
{
public:
  std::vector<int> sequence;
  TSP tsp;
  bool selectedForMating;  //used to avoid selecting a solution multiple times
public:
  /** Constructor
  * build a standard solution as the sequence <0, 1, 2, 3 ... n-1, 0>
  * @param tsp TSP instance
  * @return ---
  */
  TSPSolution(const TSP& tsp = TSP()) {  // give default value
    selectedForMating = 0;
    sequence.reserve(tsp.n + 1);
    for (int i = 0; i < tsp.n; i++){
      sequence.push_back(i);
    }
    sequence.push_back(0);
    this -> tsp = tsp;
  }
  /** Copy constructor
  * build a solution from another
  * @param tspSol TSP solution
  * @return ---
  */
  TSPSolution(const TSPSolution& tspSol ){
    sequence.reserve(tspSol.sequence.size());
    for ( uint i = 0; i < tspSol.sequence.size(); ++i ) {
      sequence.push_back(tspSol.sequence[i]);
    }
    this -> tsp = tspSol.tsp;
    this -> selectedForMating = tspSol.selectedForMating;
  }
public:
  /** print method
  * @param ---
  * @return ---
  */
  void print() const{
    for (uint i = 0; i < sequence.size(); i++){
      std::cout << sequence[i] << " ";
    }
  }
  /** assignment method
  * copy a solution into another one
  * @param right TSP solution to get into
  * @return ---
  */
  TSPSolution& operator=(const TSPSolution& right){
    // Handle self-assignment:
    if(this == &right) return *this;
    for ( uint i = 0; i < sequence.size(); i++ ){
      sequence[i] = right.sequence[i];
    }
    this -> tsp = right.tsp;
    this -> selectedForMating = right.selectedForMating;
    return *this;
  }

  bool operator==(const TSPSolution& right) const{
	  bool same = true;
	  for (uint i = 0; i < sequence.size(); i++){
          if (sequence[i] != right.sequence[i]){
			  same = false;
		  }
	  }

	  return same;
  }

  int solutionSize() const{
	  return sequence.size();
  }
};

/*used to sort population in descending order (ascending according to evaluation, we are minimizing!)*/
class TSPSolutionComparator{
	private:
	  TSP tsp;
	public:
	  void set_tsp(const TSP& tsp){  //can't write a constructor, because ambiguity arises with ()
	      this -> tsp = tsp;
	  }
	  /*operator overloading*/
	  bool operator()(const TSPSolution& sol1, const TSPSolution& sol2){
		  double val_left = 0;
		  double val_right = 0;

		  /*rewrite evalute, because of some conflicts*/
		  for (uint i = 0; i < sol1.sequence.size()-1; i++){
		      int from1 = sol1.sequence[i];
		      int to1   = sol1.sequence[i+1];
		      int from2 = sol2.sequence[i];
		      int to2   = sol2.sequence[i+1];

			  val_left += tsp.cost[from1][to1];
		      val_right += tsp.cost[from2][to2];
	      }

		  return (val_left < val_right);   //we are minimizing!

	  }
};

#endif /* TSPSOLUTION_H */
