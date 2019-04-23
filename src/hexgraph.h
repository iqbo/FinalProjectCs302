#include <vector>
#include <math.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;



struct node {	//each node represents a wall
public:
	int number; //their number on the board easier for seeing positions of each wall
	int wall;	//if wall == true, opening = NULL

	node * opening	= NULL;		//leads to higher rank
	node *  left	= NULL;		//node to the left 
	node *  right	= NULL;		//node to the right

	int rank;	//which shell this node belongs to
};

class Hexgraph {
	public:
		Hexgraph(bool wall1[6], bool wall2[6], bool wall3[6]);
		void addShell(bool *walls);	//accepts a vector of pools to add a shell to the graph
		void checkOpenings();		//checks for possible openings from wall to wall
		int maxRank;			//highest rank
		int findPath();			//finds a path 
		vector< vector<node*> >nodes;	//nodes[0] are all nodes in rank 0 shell
		void print();
};
