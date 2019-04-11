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
	int wall;	//if wall = 1, opening = NULL
	node * opening = NULL;	//leads to higher rank
	node * left = NULL;			//node to the left 
	node * right = NULL;			//node to the right
	int rank;	//which shell this node belongs to
};

class Hexgraph {
	public: 
		Hexgraph();
		void addShell(bool walls[6]);		//accepts a vector of pools to add a shell to the graph	
		//vector<shell *>;					//store shells or merge hexgraph and shell
		int maxRank;							//highest rank
		int findPath();						//finds a path
		vector< vector<node *> >nodes;		//nodes[0] are all nodes in rank 0 shell
};

//initiliazes graph, source node in center with opening to each of rank 0 node
Hexgraph::Hexgraph(){
	//calls add shell
	//increment max rank for each call to add shell
}

//constructs nodes for shell with appropiate connections
void Hexgraph::addShell(bool walls[6]){
	//traverse walls constructing one node per wall with appropiate fields
	//after adding new shell, checking previous shell to set its node opening fields
}

//findPath
//searchs for a path from node[0][0] to highest ranking node
//rules: every node has left and ride field
//		 if a node has an opening filed it connects to a higher ranking node

// Probably BFS
int Hexgraph::findPath(){

}




